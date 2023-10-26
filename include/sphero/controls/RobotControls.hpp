#ifndef AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
#define AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
#include <boost/asio.hpp>
#include <iostream>
#include "sphero/utils/udpClient.hpp"

class RobotControls {

private:
    UdpClient& udpClient;

public:
    RobotControls(UdpClient& client);
    void run();
    std::string keyboardInput();


};
#endif //AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP