#include "sphero/controls/RobotControls.hpp"

#include <iostream>

void RobotControls::move(char command) {
    switch(command) {
        case 'w':
            motorLForward();
            motorRForward();
            break;
        case 's':
            motorLBackward();
            motorRBackward();
            break;
        case 'a':
            motorLBackward();
            motorRForward();
            break;
        case 'd':
            motorLForward();
            motorRBackward();
            break;
        default:
            std::cout << "Invalid command\n";
    }
}
