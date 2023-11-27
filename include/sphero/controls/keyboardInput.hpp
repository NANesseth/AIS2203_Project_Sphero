#ifndef AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
#define AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>
#include "sphero/utils/enums.hpp"
#include "sphero/controls/CameraControls.hpp"
#include "sphero/utils/Json reader.hpp"
#include <nlohmann/json.hpp>



class KeyboardInput{
    public:
        explicit KeyboardInput( int initialSpeed = 0, int initialHeading = 0)
    : speed(initialSpeed), heading(initialHeading) {
        }

    enums::Action interpretKey(int key) {
            using namespace enums;
            const char ESC_KEY = 27;
            switch (key) {
                case 'w': return Action::Drive;
                case 's': return Action::DriveReverse;
                case ' ': return Action::Stop;
                case 'a': return Action::TurnLeft;
                case 'd': return Action::TurnRight;
                case 'p': return Action::IncreaseSpeed;
                case 'm': return Action::DecreaseSpeed;
                case 'v': return Action::StartVideo;
                case 'c': return Action::StopVideo;
                case 'i': return Action::CameraUp;
                case 'k': return Action::CameraDown;
                case 'j': return Action::CameraLeft;
                case 'l': return Action::CameraRight;
                case ESC_KEY: return Action::Exit;
                default: return Action::None;
            }
        }
    const int getSpeed() const {
        return speed;
    }

    const int getHeading() const {
        return heading;
    }

    void performAction(enums::Action action, enums::Controller& controller){
        using namespace enums;
        switch(action){


            case Action::Drive: msg = "drive"; break;
            case Action::DriveReverse: msg = "drive_reverse"; break;
            case Action::Stop: speed = 0; break;
            case Action::TurnLeft:
                heading = (heading - headingIncrement) % 360;
                if (heading < 0) heading += 360;
                break;
            case Action::TurnRight:
                heading = (heading + headingIncrement) % 360;
                break;
            case Action::IncreaseSpeed:
                speed += speedIncrement;
                if (speed > maxSpeed) speed = maxSpeed;
                break;
            case Action::DecreaseSpeed:
                speed -= speedIncrement;
                if (speed < 0) speed = 0;
                break;
            case Action::StartVideo:
                    msg = "start_video";
                break;
            case Action::StopVideo:
                    msg = "stop_video";
                break;
            case Action::CameraUp:
                tiltPosition -= tiltIncrement;
                cameraControl.setTiltPosition(tiltPosition);
                break;
            case Action::CameraDown:
                tiltPosition += tiltIncrement;
                cameraControl.setTiltPosition(tiltPosition);
                break;
            case Action::CameraLeft:
                panPosition += panIncrement;
                cameraControl.setPanPosition(panPosition);
                break;
            case Action::CameraRight:
                panPosition -= panIncrement;
                cameraControl.setPanPosition(panPosition);
                break;
            case Action::Exit:
                msg = "exit";
                controller = NOCONTROLLER;
                break;
            case Action::None:
            default:
                if (msg == "start_video"){
                    msg = "no input";
                }
                break;
        }
    }

    bool selectController(enums::Controller& controller){//TODO: fix this controller stuff
        using namespace enums;

        char key;
        const char ESC_KEY = 27;
        key = (char)cv::waitKey(10);
        switch (key) {
            case '1': controller = KEYBOARD; break;
            case '2': controller = XBOX; break;
            case ESC_KEY: stopflag = true; break;
            default: break;
        }
        //std::cout<<"Controller is "<< controller <<std::endl;
        return stopflag;
    }

    std::string getJsonMessageAsString(){
            nlohmann::json message = {
                    {"message", msg},
                    {"speed", speed},
                    {"heading", heading},
                    {"panPosition", panPosition},
                    {"tiltPosition", tiltPosition}
            };
            return message.dump();
    }

private:
    int speed = 10;
    static constexpr int speedIncrement = 10;
    static constexpr int maxSpeed = 255;
    static constexpr int panIncrement = 10;
    static constexpr int tiltIncrement = 10;
    static constexpr int headingIncrement = 10;

    int heading = 0;
    std::string msg= "empty";
    bool stopflag = false;
    int panPosition = -5;
    int tiltPosition = 0;
    CameraControl cameraControl;
};

#endif // AIS2203_PROJECT_SPHERO_KEYBOARDINPUT_HPP
