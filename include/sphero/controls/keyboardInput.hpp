#ifndef AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
#define AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>

std::string keyboardInput(std::atomic<bool>& videoRunning, std::condition_variable &frameCondition, bool &stopflag){
    char key;
    std::string msg;
    const char ESC_KEY = 27;
    key = (char)cv::waitKey(10);
    if (key == ESC_KEY || key == 'v' || key == 'c' || key == 'w' || key == 's' || key == 'a' || key == 'd' || key == 'g') {
        std::cout << key << " is pressed" << std::endl;
    }
    switch (key) {
        case 'w': msg = "forward"; break;
        case 's': msg = "backward"; break;
        case 'a': msg = "left"; break;
        case 'd': msg = "right"; break;
        case 'p': msg = "+,10"; break;//plus speed
        case 'm': msg = "-,10"; break;//minus speed
        case 'v':
            if (!videoRunning.load()) {
                msg = "video";
                videoRunning.store(true);
            }
            break;
        case 'c':
            if (videoRunning.load()) {
                msg = "stop_video";
                frameCondition.notify_all(); // Wake up any waiting threads
                videoRunning.store(false);

            }
            break;
        case 'g': msg = "gas"; break;
        case ESC_KEY:
            std::cout << "ESC key pressed. Exiting.\n";
            msg = "exit";
            videoRunning.store(false);
            stopflag = true;
            break;
        default:
            if (key != -1) { // -1 corresponds to no key being pressed
                std::cout << "no command\n";
            }
            break;
    }
    return msg;
}


#endif // AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
