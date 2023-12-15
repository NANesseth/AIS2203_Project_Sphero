#ifndef AIS2203_PROJECT_SPHERO_XBOXINPUT_HPP
#define AIS2203_PROJECT_SPHERO_XBOXINPUT_HPP
#include "nlohmann/json.hpp"
#include "sphero/controls/CameraControls.hpp"
#include <XInput.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>
#include <windows.h>
#include "sphero/utils/Json reader.hpp"
#include "sphero/utils/enums.hpp"

#pragma comment(lib, "XInput.lib")

class CXBOXController
{
private:
    XINPUT_STATE _controllerState;
    int _controllerNum;
    int speed = 0;
    int heading = 0;
    static constexpr int maxSpeed = 100;
    std::string msg = "empty";

    CameraControl cameraControl;


public:

    CXBOXController(int playerNumber)
        : _controllerNum(playerNumber - 1), cameraControl() {
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


    std::string getJsonMessageAsString(){
        nlohmann::json message = {
                {"message", msg},
                {"speed", speed},
                {"heading", heading},
                {"panPosition", cameraControl.getPanPosition()},
                {"tiltPosition", cameraControl.getTiltPosition()}
        };
        return message.dump();
    }

    int mapJoystickToSteering(int joystickX, int joystickY) {
        const float maxTurnRate = 5; // Can be tuned for finer control
        float turnRate;

        // Calculate turnRate based on how far the joystick is pushed to the side
        if (joystickX > 10000) {
            float normalizedX = ((float)joystickX - 10000) / (32767 - 10000);
            std::cout << "normalizedX: " << normalizedX << std::endl;
            turnRate = maxTurnRate * normalizedX;
            std::cout << "turnRate: " << turnRate << std::endl;
            heading += turnRate;
        }
        else if (joystickX < -10000) {
            float normalizedX = ((float)joystickX + 10000) / (32768 - 10000);
            std::cout << "normalizedX2: " << normalizedX << std::endl;
            turnRate = maxTurnRate * normalizedX;
            heading += turnRate;
        }

        // Keep heading between 0 and 360
        heading = heading >= 0 ? heading % 360 : (360 + (heading % 360)) % 360;

        return heading;
    }

    float mapTriggerToRtrigger(BYTE trigger) {
        if (trigger > 10) {
            msg = "drive";
            speed = (BYTE)trigger;
            if (speed > maxSpeed) speed = maxSpeed;
        }
        else {
            speed = 0;
        }
        return speed;
    }
    float mapTriggerToLtrigger(BYTE trigger) {
        if (trigger > 0) {
            msg = "drive_reverse";
            speed = (BYTE) trigger;
            if (speed > maxSpeed) speed = maxSpeed;
        }
        return speed;
    }

    void updateCameraControl() {
        int panPositionScaled = scaleJoystickValue(getRightJoystickX());
        int tiltPositionScaled = scaleJoystickValue(getRightJoystickY());
        cameraControl.setPanPosition(panPositionScaled);
        cameraControl.setTiltPosition(tiltPositionScaled);
    }

    // Scale joystick values from [-32767, 32767] to [-100, 100]
    int scaleJoystickValue(int val) {
        double scaledVal = (val / 32767.0) * 100.0;
        return static_cast<int>(scaledVal);
    }

    void run(enums::Controller& controller){
            if(IsConnected())
            {
                if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
                {
                    std::cout << "start_video";
                    msg = "start_video";
                    }
                }

                if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
                {
                        msg = "stop_video";
                }
                if(GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START){
                    msg = "exit";
                    controller = enums::NOCONTROLLER;
                }


                if(IsConnected())
                {
                    bool stopflag = false;
                    // Map joystick X and Y to steering
                    this->heading = mapJoystickToSteering(getLeftJoystickX(), getLeftJoystickY());

                    float acceleration = mapTriggerToRtrigger(getRightTrigger());



                    // Map L trigger to deceleration
                    float deceleration = mapTriggerToLtrigger(getLeftTrigger());
                    // Map R trigger to acceleration
                    if(acceleration > deceleration){
                        speed  = mapTriggerToRtrigger(getRightTrigger());
                    }
                    else{
                        speed = mapTriggerToLtrigger(getLeftTrigger());
                    }
                    updateCameraControl();

                }
                else
                {
                    std::cout << "\n\tERROR! PLAYER 1 - XBOX 360 Controller Not Found!\n";
                    std::cout << "Press Any Key To Exit.";
                    std::cin.get();
                }

    }
};





#endif//AIS2203_PROJECT_SPHERO_XBOXINPUT_HPP