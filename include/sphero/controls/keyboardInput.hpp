#include <iostream>
#include <opencv2/opencv.hpp>

// Assuming this is meant to be a header guard,
// ensure that the unique macro is defined at the start of your header file
#ifndef AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
#define AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP

// Use a named constant for the ESC key to improve readability
const char ESC_KEY = 27;

inline std::string keyboardInput() {
    std::string msg;
    while (true) {
        char key = (char)cv::waitKey(20);

        // Check if the key is one of the command keys
        switch(key) {
            case 'w': msg = "forward"; break;
            case 's': msg = "backward"; break;
            case 'a': msg = "left"; break;
            case 'd': msg = "right"; break;
            case 'v': msg = "video"; break;
            case 'c': msg = "stop_video"; break;
            case 'g': msg = "gas"; break;
            case ESC_KEY: // Use named constant for clarity
                std::cout << "ESC key pressed. Exiting.\n";
                return "exit";
            default:
                std::cout << "Invalid command\n";
                continue; // Invalid command, continue the loop
        }
        std::cout<<key<<" is pressed"<<std::endl;
        break; // We have a valid command, so we break the loop
    }
    return msg;
}

// Close the header guard at the end of your header file
#endif // AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP