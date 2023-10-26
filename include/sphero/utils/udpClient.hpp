#ifndef AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP
#define AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

inline std::string getLocalIp() {
    boost::asio::io_service ioService;
    boost::asio::ip::udp::resolver resolver(ioService);
    boost::asio::ip::udp::resolver::query query(boost::asio::ip::host_name(), "");
    boost::asio::ip::udp::resolver::iterator it = resolver.resolve(query);
    boost::asio::ip::udp::endpoint endpoint = *it;
    return endpoint.address().to_string();
}
class UdpClient {
        std::unique_ptr<boost::asio::ip::udp::socket> socket;
        boost::asio::ip::udp::endpoint serverEndpoint;
    public:
        UdpClient(int port, const boost::asio::ip::udp::endpoint& serverEndpoint)
            : socket(startUdpClient(port)), serverEndpoint(serverEndpoint) {}


std::unique_ptr<boost::asio::ip::udp::socket> startUdpClient(int port) {
    auto io_service = std::make_unique<boost::asio::io_service>();
    auto socket = std::make_unique<boost::asio::ip::udp::socket>(*io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));
    std::string ip = getLocalIp();
    std::cout << "Client started at " << ip << ":" << port << "\n";

    return socket;
}

void sendMessage(const std::string& message) {
    socket->send_to(boost::asio::buffer(message), serverEndpoint);
}

};

#endif //AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP