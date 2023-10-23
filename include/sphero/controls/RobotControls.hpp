#ifndef AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
#define AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
#include <boost/asio.hpp>
#include <iostream>

class RobotControls {
public:
    RobotControls(boost::asio::ip::udp::socket& serverSocket,  boost::asio::ip::udp::endpoint serverTuple)
        : serverSocket_(serverSocket), serverTuple_(serverTuple) {}

    void run() {
        std::string msg;
        do {
            msg = keyboardInput();
            if (msg != "exit") {
                sendInput(msg, serverSocket_, serverTuple_);
            } else {
                std::cout << "Exiting application.\n";
            }
        } while (msg != "exit");
    }

    void sendInput(std::string msg , boost::asio::ip::udp::socket& serverSocket, boost::asio::ip::udp::endpoint serverTuple);
    std::string keyboardInput();

private:
    boost::asio::ip::udp::socket& serverSocket_;
    boost::asio::ip::udp::endpoint serverTuple_;
};
#endif //AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP