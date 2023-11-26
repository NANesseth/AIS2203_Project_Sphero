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
    size_t bytes_transferred;


public:
    UdpClient(const int port, const boost::asio::ip::udp::endpoint& serverEndpoint)
            : io_service(std::make_unique<boost::asio::io_service>()),
              socket(std::make_unique<boost::asio::ip::udp::socket>(*io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))),
              serverEndpoint(serverEndpoint) {
        std::string ip = getLocalIp();
        std::cout << "Client started at " << ip << ":" << port << "\n";
    }

    void sendMessage(const std::string& message) {
        boost::asio::ip::udp::endpoint endpointCopy;
        std::string messageCopy;

        {
            std::unique_lock<std::mutex> lock(mtx);
            endpointCopy = serverEndpoint;
            messageCopy = message;
        }
        //std::cout<<"messageCopy: "<< messageCopy.size()<<std::endl;

        //std::cout<<"endpointCopy: "<< endpointCopy<<std::endl;


        try {
            //std::cout << "sending: " << messageCopy.size() << "bytes" << std::endl;
            bytes_transferred = socket->send_to(boost::asio::buffer(messageCopy), endpointCopy);
        }
        catch (const boost::system::system_error& ex) {
            std::cout << "Error sending message: " << ex.what() << "\n";
            return;
        }

        std::cout << "Bytes sent: " << bytes_transferred << "\n";
    }

    std::string receiveData() {
        //std::cout<<"entered receive data"<<std::endl;
        boost::asio::ip::udp::endpoint endpointCopy;
        {
            std::unique_lock<std::mutex> lock(mtx);
            endpointCopy = serverEndpoint;

        }

        int len = socket->receive_from(boost::asio::buffer(recv_buf), endpointCopy);
        std::string received_data(recv_buf.data(), len);
        //std::cout<<"decoded frame"<<std::endl;
        return received_data;
    }

    //Move to json class later


};

#endif //AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP
