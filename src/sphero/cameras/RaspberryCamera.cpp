//
// Created by nikol on 02.10.2023.
//

#include "sphero/cameras/RaspberryCamera.hpp"
#include "sphero/utils/udpClient.hpp"
#include "sphero/utils/UserInterface.hpp"

RaspberryCamera::RaspberryCamera(UdpClient& client)
    : udpClient(client) {}



void RaspberryCamera::run(){
    UserInterface userInterface(udpClient);
}
