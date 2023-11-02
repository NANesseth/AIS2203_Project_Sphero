#ifndef AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
#define AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

inline std::string keyboardInput() {
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
            case 'v':
                msg = "video";
                break;
            case 'c':
                msg = "stop_video";
                break;
            case 'g':
                msg = "gas";
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



#endif//AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
