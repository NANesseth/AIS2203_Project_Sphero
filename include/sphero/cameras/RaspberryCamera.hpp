#ifndef AIS2203_PROJECT_SPHERO_RASPBERRYCAMERA_HPP
#define AIS2203_PROJECT_SPHERO_RASPBERRYCAMERA_HPP
#include <boost/asio.hpp>
#include <iostream>
#include "sphero/utils/udpClient.hpp"
#include "sphero/controls/keyboardInput.hpp"
#include "opencv2/opencv.hpp"

class RaspberryCamera {

private:
    UdpClient& udpClient;

public:
    RaspberryCamera(UdpClient& client);
    void run();
};

#endif //AIS2203_PROJECT_SPHERO_RASPBERRYCAMERA_HPP