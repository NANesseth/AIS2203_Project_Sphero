#ifndef AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
#define AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
#include <boost/asio.hpp>
#include <iostream>
#include "sphero/utils/udpClient.hpp"
#include "sphero/controls/keyboardInput.hpp"

class RobotControls {


public:
    RobotControls(UdpClient& client);
    void run();


private:
    UdpClient& udpClient;


};
#endif //AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP