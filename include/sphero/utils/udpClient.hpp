#ifndef AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP
#define AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <mutex>

#include <variant>
#include "Json reader.hpp"
using namespace cv;

inline std::string getLocalIp() {
    boost::asio::io_service ioService;
    boost::asio::ip::udp::resolver resolver(ioService);
    boost::asio::ip::udp::resolver::query query(boost::asio::ip::host_name(), "");
    boost::asio::ip::udp::resolver::iterator it = resolver.resolve(query);
    boost::asio::ip::udp::endpoint endpoint = *it;
    return endpoint.address().to_string();
}

class UdpClient {
private:
    std::mutex mtx;
    std::unique_ptr<boost::asio::io_service> io_service; // Moved to here
    std::unique_ptr<boost::asio::ip::udp::socket> socket; // Moved to here
    boost::asio::ip::udp::endpoint serverEndpoint;
    boost::array<char, 65536> recv_buf;


public:
    UdpClient(const int port, const boost::asio::ip::udp::endpoint& serverEndpoint)
            : io_service(std::make_unique<boost::asio::io_service>()),
              socket(std::make_unique<boost::asio::ip::udp::socket>(*io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))),
              serverEndpoint(serverEndpoint) {
        std::string ip = getLocalIp();
        std::cout << "Client started at " << ip << ":" << port << "\n";
    }
    void sendMessage(const std::string& message) {
        std::unique_lock<std::mutex> lock(mtx);
        socket->send_to(boost::asio::buffer(message), serverEndpoint);

    }


    std::string receiveData() {
        std::cout<<"entered receive frame"<<std::endl;
        std::unique_lock<std::mutex> lock(mtx);
        boost::array<char, 65536> recv_buf;
        int len = socket->receive_from(boost::asio::buffer(recv_buf), this->serverEndpoint);
        std::string received_data(recv_buf.data(), len);
        //std::cout<<"decoded frame"<<std::endl;
        return received_data;
    }

    //Move to json class later


};

#endif //AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP
