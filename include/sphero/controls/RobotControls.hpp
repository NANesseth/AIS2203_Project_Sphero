#ifndef AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
#define AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
#include <boost/asio.hpp>
#include <iostream>

class RobotControls{
public:
    RobotControls(boost::asio::ip::udp::socket& serverSocket,  boost::asio::ip::udp::endpoint serverTuple){
        recieveAndSendInput(serverSocket, serverTuple);
    }
    void recieveAndSendInput(boost::asio::ip::udp::socket& serverSocket, boost::asio::ip::udp::endpoint serverTuple);
};
#endif //AIS2203_PROJECT_SPHERO_ROBOTCONTROLS_HPP
