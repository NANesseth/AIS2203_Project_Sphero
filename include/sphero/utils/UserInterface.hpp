#ifndef AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
#define AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP

#include "sphero/utils/udpClient.hpp"
#include "sphero/controls/keyboardInput.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

class UserInterface {
private:
    UdpClient& udpClient;
    const std::string windowName = "Control Window";

public:
    UserInterface(UdpClient& client) : udpClient(client) {
        cv::namedWindow(windowName);
    }

    ~UserInterface() {
        cv::destroyWindow(windowName);
    }

    void run() {
        std::string msg;
        do {
            msg = keyboardInput();
            if (msg != "exit") {
                udpClient.sendMessage(msg);
                displayFrame();
            } else {
                std::cout << "Exiting application.\n";
            }
        } while (msg != "exit");
    }

    void displayFrame() {
        cv::Mat frame = udpClient.receiveFrame();
        cv::putText(frame, "Control Message: " + udpClient.getLastMessage(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
        cv::imshow(windowName, frame);
        cv::waitKey(1);
    }
};

#endif // AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP