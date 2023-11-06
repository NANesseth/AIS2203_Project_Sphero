#ifndef AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
#define AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>
class KeyBoardInput{

    public:
        KeyBoardInput(int initialSpeed = 0, int initialHeading = 0)
            : speed(initialSpeed), heading(initialHeading) {
        }

        std::string KeyboardInput(std::atomic<bool>& videoRunning, std::condition_variable &frameCondition, bool &stopflag){
        char key;
        std::string msg;
        const char ESC_KEY = 27;
        key = (char)cv::waitKey(10);

        switch (key) {
            case 'w': message[0] = "drive"; break;
            case 's': message[0] =  "drive_reverse"; break;
            case 'a': message[2] = heading - headingIncrement break;
            case 'd': message[2] = heading + headingIncrement; break;
            case 'p': message[1] = speed + speedIncrement;break;//plus speed
            case 'm': message[1] = speed - speedIncrement; break;//minus speed
            case ' ': message[1] = 0;                   break;//press space to stop driving
            case 'v':
                if (!videoRunning.load()) {
                    message[0] = "video";
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
            case ESC_KEY:
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
    }

    std::string getMessage(){
        message = this->msg[0] + "," + this->msg[1] + "," + this->msg[2];
        return message;
    }

private:
    int speed = 0;
    int speedIncrement = 10;
    int heading = 0;
    int headingIncrement = 10;
    std::array<std::string, 3> message = {"move", "0", "0"};

};




#endif // AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
