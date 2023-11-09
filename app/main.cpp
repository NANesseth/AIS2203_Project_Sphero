#include "sphero/sphero.hpp"
#include <iostream>

int main() {
    try {
        // Initialize asio service.
        boost::asio::io_service ioService;
        boost::asio::ip::udp::resolver resolver(ioService);

        // Resolve server endpoint using IP and port from the serverTpl.
        std::string serverIP = "10.25.46.172"; //10.25.46.172
        std::string serverPort = "12395";
        boost::asio::ip::udp::endpoint serverEndpoint = *resolver.resolve(boost::asio::ip::udp::v4(), serverIP, serverPort);

        // Create a UDP client.
        UdpClient udpClient(12346, serverEndpoint);

        // Initialize UserInterface.
        UserInterface userInterface(udpClient);
        userInterface.run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Program completed successfully." << std::endl;
    return 0;
}
