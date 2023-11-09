#ifndef AIS2203_PROJECT_SPHERO_XBOXINPUT_HPP
#define AIS2203_PROJECT_SPHERO_XBOXINPUT_HPP
#include <windows.h>
#include <XInput.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>
#pragma comment(lib, "XInput.lib")

// XBOX Controller Class Definition
class CXBOXController
{
private:
    XINPUT_STATE _controllerState;
    int _controllerNum;
    int speed = 0;
    int heading = 0;
    static constexpr int maxSpeed = 255;
    bool videoRunning = false;

public:
    std::array<std::string, 3> message = {"drive", "0", "0"};

    CXBOXController(int playerNumber) {
        _controllerNum = playerNumber - 1;
    }

    XINPUT_STATE GetState() {
        ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
        XInputGetState(_controllerNum, &_controllerState);
        return _controllerState;
    }

    bool IsConnected() {
        ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
        DWORD Result = XInputGetState(_controllerNum, &_controllerState);

        if (Result == ERROR_SUCCESS) {
            return true;
        } else {
            return false;
        }
    }

    void Vibrate(int leftVal = 0, int rightVal = 0) {
        XINPUT_VIBRATION Vibration;
        ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));
        Vibration.wLeftMotorSpeed = leftVal;
        Vibration.wRightMotorSpeed = rightVal;
        XInputSetState(_controllerNum, &Vibration);
    }

    short getLeftJoystickX() {
        return _controllerState.Gamepad.sThumbLX;
    }

    short getLeftJoystickY() {
        return _controllerState.Gamepad.sThumbLY;
    }

    short getRightJoystickX() {
        return _controllerState.Gamepad.sThumbRX;
    }

    short getRightJoystickY() {
        return _controllerState.Gamepad.sThumbRY;
    }

    BYTE getRightTrigger() {
        return _controllerState.Gamepad.bRightTrigger;
    }

    BYTE getLeftTrigger() {
        return _controllerState.Gamepad.bLeftTrigger;
    }

    std::string getMessage(){
        auto msg = this->message[0] + "," + this->message[1] + "," + this->message[2];
        return msg;
    }

    void drive(float speed, float heading, bool stopflag, std::array<std::string,3>& message){
        if (stopflag || (speed == 0)) {
            message[0] = "drive";
            message[1] = "0";
            message[2] = "0";
        } else {
            message[0] = "drive";
            message[1] = std::to_string(speed);
            message[2] = std::to_string(heading);
        }
    }

    float mapJoystickToSteering(int joystickX, int joystickY) {
        float steering = 0;
        if (joystickX > 0) {
            steering = ((float)joystickX / 32767) * 180;
        } else if (joystickX < 0) {
            steering = ((float)joystickX / 32768) * 180;
        } else if (joystickY > 0) {
            steering = ((float)joystickY / 32767) * 180;
        } else if (joystickY < 0) {
            steering = ((float)joystickY / 32768) * 180;
        }
        return steering;
    }

    float mapTriggerToAcceleration(BYTE trigger) {
        float speed = 0;
        if (trigger > 0) {
            speed = (float)trigger / 255;
        }
        return speed;
    }
    void run(){
        while(true)
        {
            if(IsConnected())
            {
                if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
                {
                    videoRunning = true;
                    std::cout << "video";
                    message[0] = "video";
                }

                if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
                {
                    videoRunning = false;
                    std::cout << "stop_video";
                    message[0] = "stop_video";
                }


                if(IsConnected())
                {
                    float heading = 0;
                    float speed = 0;
                    bool stopflag = false;

                    // Map the joystick X and Y to steering
                    heading = mapJoystickToSteering(getLeftJoystickX(), getLeftJoystickY());

                    // Map the R trigger to acceleration
                    float acceleration = mapTriggerToAcceleration(getRightTrigger());

                    // Map the L trigger to deceleration
                    float deceleration = mapTriggerToAcceleration(getLeftTrigger());

                    // Deduct deceleration from acceleration to get the final speed
                    speed = acceleration - deceleration;

                    // Drive using the mapped speed and steering
                    drive(speed, heading, stopflag, message);

                    getMessage();
                }
            }
            else
            {
                std::cout << "\n\tERROR! PLAYER 1 - XBOX 360 Controller Not Found!\n";
                std::cout << "Press Any Key To Exit.";
                std::cin.get();
                break;
            }
        }
    }
};





#endif//AIS2203_PROJECT_SPHERO_XBOXINPUT_HPP