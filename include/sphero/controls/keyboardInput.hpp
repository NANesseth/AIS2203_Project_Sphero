#ifndef AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
#define AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>
#include "sphero/utils/enums.hpp"


class KeyboardInput{
    public:
        explicit KeyboardInput(int initialSpeed = 0, int initialHeading = 0)
            : speed(initialSpeed), heading(initialHeading) {
        }

        void getKeyboardInput(std::atomic<bool>& videoRunning, std::condition_variable &frameCondition, enums::Controller& controller){
            char key;
            const char ESC_KEY = 27;
            key = (char)cv::waitKey(10);

            switch (key) {
                case 'w': msg = "drive"; break;
                case 's': msg = "drive_reverse"; break;
                case ' ': speed = 0; break;//press space to stop driving
                case 'a':
                    heading = (heading - headingIncrement) % 360;
                    if (heading < 0) heading += 360;  // Correct negative values
                    break;
                case 'd':
                    heading = (heading + headingIncrement) % 360;
                    break;
                case 'p':
                    speed += speedIncrement;
                    if (speed > maxSpeed) speed = maxSpeed;
                    break;
                case 'm':
                    speed -= speedIncrement;
                    if (speed < 0) speed = 0;
                    break;
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
                case ESC_KEY:
                    msg = "exit";
                    videoRunning.store(false);
                    controller = enums::NOCONTROLLER;
                    break;
                default:
                    if (key != -1) { // -1 corresponds to no key being pressed
                        std::cout << "no command\n";
                        msg = "dont_drive";
                    }
                    break;
            }
    }

    bool selectController(enums::Controller& controller){//TODO: fix this controller stuff
        char key;
        const char ESC_KEY = 27;
        key = (char)cv::waitKey(10);
        switch (key) {
            case '1': controller = enums::KEYBOARD; break;
            case '2': controller = enums::XBOX; break;
            case ESC_KEY: stopflag = true; break;
            default: break;
        }
        return stopflag;
    }

    std::string getMessage(){
        std::string message = msg + "," + std::to_string(speed) + "," + std::to_string(heading);
        return message;
    }

private:
    int speed = 10;
    static constexpr int speedIncrement = 10;
    static constexpr int maxSpeed = 255;
    int heading = 0;
    std::string msg= "empty";
    static constexpr int headingIncrement = 10;
    bool stopflag = false;

};




#endif // AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
