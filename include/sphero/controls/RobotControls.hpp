#ifndef AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
#define AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP

#include <iostream>

class RobotControls{
public:
    void move(char command);
private:
    void motorLForward() {
        std::cout << "Motor L forward\n";
        //send forward to server
    }
    void motorRForward() {
        std::cout << "Motor R forward\n";
        //send forward to server
    }
    void motorLBackward() {
        std::cout << "Motor L backward\n";
        //send backward to server
    }
    void motorRBackward() {
        std::cout << "Motor R backward\n";
        //send backward to server
    }
};

#endif //AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
