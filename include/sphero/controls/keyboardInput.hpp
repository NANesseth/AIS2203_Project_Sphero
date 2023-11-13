#ifndef AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
#define AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>
#include "sphero/utils/enums.hpp"
#include "sphero/controls/CameraControls.hpp"


class KeyboardInput{
    public:
        explicit KeyboardInput(int initialSpeed = 0, int initialHeading = 0)
            : speed(initialSpeed), heading(initialHeading) {
        }

        void getKeyboardInput(std::atomic<bool>& videoRunning, std::condition_variable &frameCondition, enums::Controller& controller){
            int key;
            const char ESC_KEY = 27;
            const int ARROW_UP = 0x260000;
            const int ARROW_DOWN = 0x270000;
            const int ARROW_LEFT = 0x250000;
            const int ARROW_RIGHT = 0x280000;
            key = cv::waitKey(0) & 0xFF;

            switch (key) {
                case 'w': msg = "drive"; break;
                case 's': msg = "drive_reverse"; break;
                case ' ': speed = 0; break; //press space to stop driving
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
                case ARROW_UP: // arrow up
                    std::cout << "Up key pressed\n";
                    cameraControl.setTiltPosition(cameraControl.getTiltPosition() + tiltIncrement);
                    break;
                case ARROW_DOWN: // arrow down
                    cameraControl.setTiltPosition(cameraControl.getTiltPosition() - tiltIncrement);
                    break;
                case ARROW_LEFT: // arrow left
                    std::cout << "Left key pressed\n";
                    cameraControl.setPanPosition(cameraControl.getPanPosition() - panIncrement);
                    break;
                case ARROW_RIGHT: // arrow right
                    std::cout << "Right key pressed\n";
                    cameraControl.setPanPosition(cameraControl.getPanPosition() + panIncrement);
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
        std::string message = msg + "," + std::to_string(speed) + "," + std::to_string(heading) + "," + std::to_string(cameraControl.getPanPosition()) + "," + std::to_string(cameraControl.getTiltPosition());
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
    static constexpr int panIncrement = 10;
    static constexpr int tiltIncrement = 10;
    CameraControl cameraControl;
};




#endif // AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
