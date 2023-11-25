#ifndef AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
#define AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP

#include "sphero/utils/udpClient.hpp"
#include "sphero/controls/keyboardInput.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "sphero/utils/enums.hpp"
#include "sphero/utils/DisplayBuilder.hpp"
#include "sphero/controls/XboxInput.hpp"
#include "Json reader.hpp"
#include "Various.hpp"
#include <typeinfo>

class UserInterface {
public:
    explicit UserInterface(UdpClient& client) : udpClient(client), controller(enums::Controller::NOCONTROLLER) {
        cv::namedWindow(windowName);
    }
    ~UserInterface() {
        if (uiThread.joinable()) {
            uiThread.join();
        }
        if (receiverThread.joinable()) {
            receiverThread.join();
        }
        if (sendingThread.joinable()) {
            sendingThread.join();
        }
        cv::destroyWindow(windowName);
    }

    void receiving(){
        JsonReader jsonReader;
        std::string data;
        while (true){
            data = udpClient.receiveData();
            jsonReader.updateJson(data);
            jsonQueue.push(jsonReader);
            dataCondition.notify_all();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void sending(){
        while(true){

            if (!messageToSend.empty()){
                std::cout<<"message not empty"<<std::endl;
                udpClient.sendMessage(messageToSend);
            }
            else{
                std::cout<<"message empty"<<std::endl;

            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void run() {
        uiThread = std::thread(&UserInterface::uiLoop, this);
        receiverThread = std::thread(&UserInterface::receiving, this);
        sendingThread = std::thread(&UserInterface::sending, this);

        cv::Mat frame;
        auto hasData = [this]() { return !jsonQueue.empty(); };

        while (true) {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                dataCondition.wait(lock, hasData);

                // Process the data from the queue
                while (!jsonQueue.empty()) {

                    data = jsonQueue.front();
                    jsonQueue.pop();

                    frame = data.getFrame();
                    try{
                        displayFrame(frame);
                    }
                    catch(std::exception& e){
                        std::cout<<"error when displaying frame: " << e.what()<<std::endl;
                    }
                }
                // Print a message when new data is available
                std::cout << "New data received and processed." << std::endl;
            }

        }
    }

private:
    UdpClient& udpClient;
    const std::string windowName = "Control Window";
    std::thread uiThread;
    std::thread receiverThread;
    std::thread sendingThread;

    CameraControl cameraControl;
    JsonReader data;
    std::queue<JsonReader> jsonQueue;
    std::mutex queueMutex;
    std::condition_variable dataCondition;

    DisplayBuilder displayBuilder;
    enums::Controller controller;

    std::string messageToSend;


    void uiLoop() {
        KeyboardInput kbInput(cameraControl);
        CXBOXController xboxController(1);
        bool stopflag = false;
        std::string message;
        using namespace enums;

        while (!stopflag){
            std::cout<<controller<<std::endl;
            if (this -> controller == KEYBOARD){
                displayBuilder.buildKeyboardMenu();
                int key;
                cv::waitKey(1);
                while (this->controller == KEYBOARD) {
                    // Handle input
                    key = cv::waitKey(1);
                    Action action = kbInput.interpretKey(key);
                    kbInput.performAction(action,  this->controller);//skal returnere en string som e slik: "msg,speed,heading"
                    messageToSend = kbInput.getJsonMessageAsString();
                    std::cout<<messageToSend<<std::endl;

                }
                displayBuilder.destroyWindow();
            }
            else if (this -> controller == XBOX){
                displayBuilder.buildXboxMenu();
                cv::waitKey(1);
                while(this-> controller == XBOX){
                    // TODO: implement xbox controller
                    xboxController.run(data, this->controller);
                    // Get the message from controller
                    messageToSend = xboxController.getJsonMessageAsString();

                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                displayBuilder.destroyWindow();
            }
            else{
                displayBuilder.buildMainMenu();
                cv::waitKey(1);
                while(this->controller == NOCONTROLLER && !stopflag){
                    stopflag = kbInput.selectController(this->controller);
                }
                displayBuilder.destroyWindow();
                }
            }
        }

    void displayFrame(cv::Mat& frame) {
            int fps = 30;
        std::cout << "display"<<std::endl;
        if (!frame.empty()) {
            cv::resize(frame, frame, cv::Size(640, 480));
            cv::imshow(windowName, frame);
            cv::waitKey(1);
        } else {
            std::cerr << "Empty or invalid frame received.\n";
        }
        //sleep approx 1/fps seconds
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/fps));
    }
};

#endif // AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
