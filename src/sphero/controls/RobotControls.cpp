#include "sphero/controls/RobotControls.hpp"
#include <opencv2/opencv.hpp>
#include "boost/asio.hpp"

#include <iostream>


void RobotControls::recieveAndSendInput(boost::asio::ip::udp::socket& serverSocket, boost::asio::ip::udp::endpoint serverTuple) {
    cv::namedWindow("Control Window");
    std::string msg;
    while(true){
            switch(char key = (char) cv::waitKey(0)) {
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
                    break;  // End program
                default:
                    msg = "No Input\n";
                    std::cout << "Invalid command\n";
            }
                serverSocket.send_to(boost::asio::buffer(msg), serverTuple);
                std::cout << "Sent: " << msg << std::endl;
        }
}
