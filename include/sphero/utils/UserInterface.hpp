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
    std::thread videoThread;
    bool videoRunning = false;


public:
    UserInterface(UdpClient& client) : udpClient(client) {
        cv::namedWindow(windowName);
    }

    ~UserInterface() {
        cv::destroyWindow(windowName);
        stopThreads();
    }

    void startThreads(){
        if  (!videoRunning){
            videoThread = std::thread(&UserInterface::displayFrame, this);
            std::cout<<"video thread started"<<std::endl;
            this -> videoRunning = true;
        }
    }

    void stopThreads(){
        std::cout<<"STOP THREADS"<<std::endl;
        if(videoRunning){
            videoRunning = false;
            if(videoThread.joinable()){
                videoThread.join();
            }
        }
        std::cout <<"stopped user interface threads"<<std::endl;
    }

    void input() {
        std::string msg;
        while(true) {
            msg = keyboardInput();
            if (msg != "exit") {
                udpClient.sendMessage(msg);
                std::cout<<msg<<" sent"<<std::endl;
            } else {
                std::cout << "Exiting application.\n";
                this -> videoRunning = false;
                break;
            }
            if (msg == "video") {
                this->videoRunning = true;
            }
        }
    }

    void displayFrame() {
        std::cout<<"entered display frame"<<"\n";
        cv::namedWindow("Video window");
        while (videoRunning) {
            std::cout<<"does the code stop here?"<<"\n";
            cv::Mat frame = udpClient.receiveFrame();
            std::cout<<"or here?"<<std::endl;
            if (!frame.empty()) {
                std::cout<<"Frame is not empty"<<std::endl;
                cv::putText(frame, "Control Message: " + udpClient.getLastMessage(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
                std::cout<<"text is put"<<std::endl;
                cv::imshow("Video window", frame); // Corrected window name
                std::cout<<"frame displayed"<<std::endl;
                if (cv::waitKey(1) == 27) { // Break the loop if the user presses 'Esc'
                    stopThreads();
                    break;
                }
                std::cout<<"imshow done"<<std::endl;
            }else{
                std::cout<<"Frame is empty"<<std::endl;
            }
        }
        cv::destroyWindow("Video window"); // Clean up after the loop finishes
    }
};

#endif // AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
