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

public:
    CXBOXController(int playerNumber);
    XINPUT_STATE GetState();
    bool IsConnected();
    void Vibrate(int leftVal = 0, int rightVal = 0);
    bool videoRunning = false;
    std::array<std::string, 3> message = {"drive", "0", "0"};



    // Remove the parameters from the getLeftJoystickX method
    short getLeftJoystickX();

    // Add the getLeftJoystickY
    short getLeftJoystickY();

    //add the getRightJoystickX
    short getRightJoystickX();

    //add the getRightJoystickY
    short getRightJoystickY();

    // Add the rightTrigger
    unsigned char getRightTrigger();

    // Add the leftTrigger
    unsigned char getLeftTrigger();

    //Get message
    std::string getMessage(){
        auto msg = this->message[0] + "," + this->message[1] + "," + this->message[2];
        return msg;
    }
};

void drive(float speed, float heading, bool stopflag, std::vector<std::string>& message){
    message.clear(); // clear messages
    if (stopflag || (speed == 0)) {
        message.push_back("drive");
        message.push_back("0");
        message.push_back("0");
    } else {
        message.push_back("drive");
        message.push_back(std::to_string(speed));
        message.push_back(std::to_string(heading));
    }
}

float mapJoystickToSteering(int joystickX, int joystickY) {
    float steering = 0;
    if (joystickX > 0) {
        steering = ((float)joystickX / 32767) * 180;  //her tenke ej mapping frå -180 til 180 grade heading 360 blir for mykje.
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

CXBOXController::CXBOXController(int playerNumber)
{
    // Set the Controller Number
    _controllerNum = playerNumber - 1;
}

XINPUT_STATE CXBOXController::GetState()
{
    // Zeroize the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    XInputGetState(_controllerNum, &_controllerState);

    return _controllerState;
}

bool CXBOXController::IsConnected()
{
    // Zeroize the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    DWORD Result = XInputGetState(_controllerNum, &_controllerState);

    if (Result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
    // Create a Vibration State
    XINPUT_VIBRATION Vibration;

    // Zeroize the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    // Vibrate the controller
    XInputSetState(_controllerNum, &Vibration);
}
// Method for the left joystick readings
short CXBOXController::getLeftJoystickX() {
    return _controllerState.Gamepad.sThumbLX;
}
short CXBOXController::getLeftJoystickY(){
    return _controllerState.Gamepad.sThumbLY;
}
// Method for the right joystick readings
short CXBOXController::getRightJoystickX(){
    return _controllerState.Gamepad.sThumbRX;
}
short CXBOXController::getRightJoystickY(){
    return _controllerState.Gamepad.sThumbRY;
}
// Method for the right trigger reading
BYTE CXBOXController::getRightTrigger(){
    return _controllerState.Gamepad.bRightTrigger;
}
// Left trigger reading
BYTE CXBOXController::getLeftTrigger(){
    return _controllerState.Gamepad.bLeftTrigger;
}



CXBOXController* Player1;

void run(){
    Player1 = new CXBOXController(1);
    std::vector<std::string> message;

    while(true)
    {
        if(Player1->IsConnected())
        {
            if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
            {
                Player1->videoRunning = true;
                std::cout << "video";
                message[0] = "video";
            }

            if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
            {
                Player1->videoRunning = false;
                std::cout << "stop_video";
                message[0] = "stop_video";
            }


            if(Player1->IsConnected())
            {
                // to clear previous drive commands
                message.clear();

                float heading = 0;
                float speed = 0;
                bool stopflag = false;

                // Map the joystick X and Y to steering
                heading = mapJoystickToSteering(Player1->getLeftJoystickX(), Player1->getLeftJoystickY());

                // Map the R trigger to acceleration
                float acceleration = mapTriggerToAcceleration(Player1->getRightTrigger());

                // Map the L trigger to deceleration
                float deceleration = mapTriggerToAcceleration(Player1->getLeftTrigger());

                // Deduct deceleration from acceleration to get the final speed
                speed = acceleration - deceleration;

                // Drive using the mapped speed and steering
                drive(speed, heading, stopflag, message);

                message.push_back(Player1->getMessage());
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

#endif//AIS2203_PROJECT_SPHERO_XBOXINPUT_HPP