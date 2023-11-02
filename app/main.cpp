#include "sphero/controls/RobotControls.hpp"
#include "sphero/utils/udpClient.hpp"
#include "sphero/utils/UserInterface.hpp"
#include <iostream>
#include <threading>


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
    UdpClient udpClient(12346, serverEndpoint);

    // Initialize UserInterface.
    UserInterface userInterface(udpClient);
    userInterface.startThreads();

    return 0;
}