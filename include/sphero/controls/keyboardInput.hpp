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

        void getKeyboardInput(std::atomic<bool>& videoRunning, std::condition_variable &frameCondition, bool &stopflag){
            char key;
            std::string msg;
            const char ESC_KEY = 27;
            key = (char)cv::waitKey(10);

            switch (key) {
                case 'w': message[0] = "drive"; break;
                case 's': message[0] = "drive_reverse"; break;
                case ' ': message[1] = "0"; break;//press space to stop driving
                case 'a':
                    heading = (heading - headingIncrement) % 360;
                    if (heading < 0) heading += 360;  // Correct negative values
                    message[2] = std::to_string(heading);
                    break;
                case 'd':
                    heading = (heading + headingIncrement) % 360;
                    message[2] = std::to_string(heading);
                    break;
                case 'p':
                    speed += speedIncrement;
                    if (speed > maxSpeed) speed = maxSpeed;
                    message[1] = std::to_string(speed);
                    break;
                case 'm':
                    speed -= speedIncrement;
                    if (speed < 0) speed = 0;
                    message[1] = std::to_string(speed);
                    break;
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

    enums::Controller selectController(enums::Controller& controller){//TODO: fix this controller stuff
        char key;
        const char ESC_KEY = 27;

        key = (char)cv::waitKey(10);
        switch (key) {
            case '1': controller = enums::KEYBOARD; break;
            case '2': controller = enums::XBOX; break;
            case ESC_KEY: controller = enums::NOCONTROLLER; break;
            default: break;
        }
        return controller;
    }

    std::string getMessage(){
        auto msg = this->message[0] + "," + this->message[1] + "," + this->message[2];
        return msg;
    }

private:
    int speed = 0;
    static constexpr int speedIncrement = 10;
    static constexpr int maxSpeed = 255;
    int heading = 0;
    static constexpr int headingIncrement = 10;
    std::array<std::string, 3> message = {"move", "0", "0"};

};




#endif // AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
