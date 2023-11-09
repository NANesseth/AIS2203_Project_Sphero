#include "sphero/controls/RobotControls.hpp"
#include "sphero/utils/udpClient.hpp"
#include "sphero/utils/UserInterface.hpp"

RobotControls::RobotControls(UdpClient& client)
    : udpClient(client) {}

void RobotControls::run(){
    UserInterface userInterface(udpClient);
}
