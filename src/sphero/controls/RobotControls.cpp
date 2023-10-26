#include "sphero/controls/RobotControls.hpp"
#include <opencv2/opencv.hpp>
#include "boost/asio.hpp"
#include "sphero/utils/udpClient.hpp"

#include <iostream>

RobotControls::RobotControls(UdpClient& client)
    : udpClient(client) {}

void RobotControls::run(){
    std::string msg;
    do {
        msg = keyboardInput();
        if (msg != "exit") {
            udpClient.sendMessage(msg);
        } else {
            std::cout << "Exiting application.\n";
        }
    } while (msg != "exit");
}


std::string RobotControls::keyboardInput() {
    cv::namedWindow("Control Window");
    std::string msg;
    while (true) {
        switch(char key = (char)cv::waitKey(0)) {
            case 'w':
                msg = "forward";
                break;
            case 's':
                msg = "backward";
                break;
            case 'a':
                msg = "left";
                break;
            case 'd':
                msg = "right";
                break;
            case 27:  // ESC key
                std::cout << "ESC key pressed. Exiting.\n";
                return "exit";
            default:
                std::cout << "Invalid command\n";
                continue;  // Invalid command, continue the loop
        }
        break;  // We have a valid command, so we break the loop
    }
    return msg;
}
