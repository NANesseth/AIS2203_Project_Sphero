#include "sphero/cameras/RaspberryCamera.hpp"
#include "sphero/controls/RobotControls.hpp"
#include "sphero/utils/udpClient.hpp"
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;

    // Initialize asio service.
    boost::asio::io_service ioService;
    boost::asio::ip::udp::resolver resolver(ioService);

    // Resolve server endpoint using IP and port from the serverTpl.
    std::string serverIP = "10.25.46.172";
    std::string serverPort = "12395";
    boost::asio::ip::udp::endpoint serverEndpoint = *resolver.resolve(boost::asio::ip::udp::v4(), serverIP, serverPort);

    // Create a UDP client.
    auto socket = startUdpClient(12346);

    // Initialize RobotControls.
    RobotControls robotControls(*socket, serverEndpoint);

    return 0;
}