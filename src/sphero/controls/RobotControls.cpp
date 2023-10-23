#include "sphero/controls/RobotControls.hpp"
#include <opencv2/opencv.hpp>
#include "boost/asio.hpp"

#include <iostream>

void RobotControls::sendInput(std::string msg , boost::asio::ip::udp::socket& serverSocket, boost::asio::ip::udp::endpoint serverTuple) {
    serverSocket.send_to(boost::asio::buffer(msg), serverTuple);
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
