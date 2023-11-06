#ifndef AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
#define AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>
class keyBoardInput(){

    public:
        keyBoardInput(int initialSpeed = 0, int initialHeading = 0)
            : speed(initialSpeed), heading(initialHeading) {
        }


        std::string keyboardInput(std::atomic<bool>& videoRunning, std::condition_variable &frameCondition, bool &stopflag){
        char key;
        std::string msg;
        const char ESC_KEY = 27;
        key = (char)cv::waitKey(10);

        switch (key) {
            case 'w': message[0] = "drive"; break;
            case 's': message[0] = speed = ; break;
            case 'a': message[2] = heading+headingConstant break;
            case 'd': message[2] = "right"; break;
            case 'p': message[1] = speed + speedConstant ;break;//plus speed
            case 'm': message[1] = speed - speedConstant; break;//minus speed
            case 'v':
                if (!videoRunning.load()) {
                    msg = "video";
                    videoRunning.store(true);
                }
                break;
            case 'c':
                if (videoRunning.load()) {
                    message[0] = "stop_video";
                    frameCondition.notify_all(); // Wake up any waiting threads
                    videoRunning.store(false);

                }
                break;
            case 'g': msg = "gas"; break;
            case ESC_KEY:
                std::cout << "ESC key pressed. Exiting.\n";
                message[0] = "exit";
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

private:
    int speed = 0;
    int speedConstant = 10;
    int heading = 0;
    int headingConstant = 10;
    std::array<std::string,int,int> message = ["move",0,0];

};




#endif // AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
