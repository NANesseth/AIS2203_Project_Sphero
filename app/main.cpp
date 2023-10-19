#include "sphero/cameras/RaspberryCamera.hpp"
#include "sphero/controls/RobotControls.hpp"

#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    RobotControls robotControls;
    
    char command;
    while (std::cin >> command) {
        robotControls.move(command);
    }
    return 0;
}
