#ifndef AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP
#define AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <nlohmann/json.hpp>
#include <variant>

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

    std::string jsonLoader(const std::string& key, std::string json_str){
        nlohmann::json jsonfile = nlohmann::json::parse(json_str);
        if (jsonfile.contains(key) && jsonfile[key].is_number_integer()){
            return jsonfile[key].get<int>();
        }
        return jsonfile(str);
    }

    UdpClient(const int port, const boost::asio::ip::udp::endpoint& serverEndpoint)
            : io_service(std::make_unique<boost::asio::io_service>()),
              socket(std::make_unique<boost::asio::ip::udp::socket>(*io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))),
              serverEndpoint(serverEndpoint) {
        std::string ip = getLocalIp();
        std::cout << "Client started at " << ip << ":" << port << "\n";
    }

    void sendMessage(const std::string& message) {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout<<"locked thread, sending message"<<std::endl;
        socket->send_to(boost::asio::buffer(message), serverEndpoint);
        std::cout<<"unlocked thread, sent message"<<std::endl;
    }

    std::variant<int,cv::Mat> recieveMessage(int topicToRecieve){
        std::unique_lock<std::mutex> lock(mtx);
        boost::array<char, 65536> recv_buf;
        std::string jsonFile = socket->recieve_from(boost::asio::buffer(recv_buf), this->serverEndpoint);

        int len = socket->receive_from(boost::asio::buffer(recv_buf), this->serverEndpoint);

        std::vector<uchar> decode_buf(recv_buf.begin(), recv_buf.begin() + len);


    }

    cv::Mat receiveFrame() {
        std::cout<<"entered receive frame"<<std::endl;
        std::unique_lock<std::mutex> lock(mtx);
        boost::array<char, 65536> recv_buf;
        int len = socket->receive_from(boost::asio::buffer(recv_buf), this->serverEndpoint);
        //TODO: make a timeout on the function above. if no frame is recieved, this one is stuck forever. currently running on another thread. is that ok?
        // Decode received frame
        std::vector<uchar> decode_buf(recv_buf.begin(), recv_buf.begin() + len);
        cv::Mat frame = cv::imdecode(decode_buf, 1);
        std::cout<<"decoded frame"<<std::endl;
        return frame;
    }

};

#endif //AIS2203_PROJECT_SPHERO_UDPCLIENT_HPP
