#ifndef AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
#define AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP

#include "sphero/utils/udpClient.hpp"
#include "sphero/controls/keyboardInput.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>

class UserInterface {
private:
    UdpClient& udpClient;
    const std::string windowName = "Control Window";
    std::thread inputThread;
    std::thread videoThread;
    bool inputRunning = false;
    bool videoRunning = false;

public:
    UserInterface(UdpClient& client) : udpClient(client) {
        cv::namedWindow(windowName);
    }

    ~UserInterface() {
        cv::destroyWindow(windowName);
        stop();
    }

    void startThreads(){
        if (!inputRunning){
            inputThread = std::thread(&UserInterface::input, this->inputThread);
            this -> inputRunning = true;
        }
        if  (!videoRunning){
            videoThread = std::thread(&UserInterface::displayFrame(), this-> videoThread);
            this -> videoRunning = true;
        }
    }

    void stopThreads(){
        if(inputRunning){
            running = false;
            if(inputThread.joinable()){
                inputThread.join();
            }
        }
        if(videoRunning){
            running = false;
            if(videoThread.joinable()){
                videoThread.join();
            }
        }
        std::cout <<"stopped user interface threads"<<std::endl;
    }

    void input() {
        std::string msg;
        while (inputRunning){
            msg = keyboardInput();
            if (msg != "exit") {
                udpClient.sendMessage(msg);
                displayFrame();
            } else {
                std::cout << "Exiting application.\n";
            }
            if (msg == "video"){
                this -> videoRunning = true;
            }
        }
    }

    void displayFrame() {
        cv::Mat frame = udpClient.receiveFrame();
        cv::putText(frame, "Control Message: " + udpClient.getLastMessage(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
        cv::imshow(this -> windowName, frame);
        cv::waitKey(1);
    }
};

#endif // AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
