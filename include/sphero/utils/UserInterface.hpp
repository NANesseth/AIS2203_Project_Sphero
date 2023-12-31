#ifndef AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
#define AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP

#include "sphero/utils/udpClient.hpp"
#include "sphero/controls/keyboardInput.hpp"
#include "sphero/controls/AutonomousControl.hpp"

#include "sphero/cameras/RaspberryCamera.hpp"
#include "sphero/vision/BallTracker.hpp"
#include "sphero/utils/JsonUtils.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
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

    void receiving() {
        JsonReader jsonReader;
        std::string data;
        while (true) {
            data = udpClient.receiveData();
            jsonReader.updateJson(data);
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if (jsonQueue.size() >= 1) {
                    jsonQueue.pop();
                }
                jsonQueue.push(jsonReader);
            }
            dataCondition.notify_all();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void sending() {
        while (true) {
            std::string message;
            {
                std::unique_lock<std::mutex> lock(sendMutex);
                sendCondition.wait(lock, [this](){ return !sendQueue.empty(); });
                if (!sendQueue.empty()) {
                    message = sendQueue.front();
                    sendQueue.pop();
                }
            }

            if (!message.empty()){
                udpClient.sendMessage(message);
            }
            else {
                std::cout << "message empty" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

        }
    }

    void run() {
        uiThread = std::thread(&UserInterface::uiLoop, this);
        receiverThread = std::thread(&UserInterface::receiving, this);
        sendingThread = std::thread(&UserInterface::sending, this);

        cv::Mat frame;
        auto hasData = [this]() { return !jsonQueue.empty(); };
        int batteryLevel;
        int distanceToObject;

        while (true) {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                dataCondition.wait(lock, hasData);

                while (!jsonQueue.empty()) {
                    data = jsonQueue.front();
                    jsonQueue.pop();

                    distanceToObject = data.getDistance();
                    batteryLevel = data.getBatteryLevel();
                    frame = data.getFrame();
                    cv::putText(frame, "Battery level: " + std::to_string(batteryLevel) + "%", cv::Point(10, 10), cv::FONT_HERSHEY_SIMPLEX, 0.40, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
                    cv::putText(frame, "Distance: " + std::to_string(distanceToObject), cv::Point(10, 25), cv::FONT_HERSHEY_SIMPLEX, 0.40, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
                    {
                        std::unique_lock<std::mutex> frameLock(frameMutex);
                        latestFrame = frame.clone(); // Update the latest frame
                    }

                    try {

                        displayFrame(frame);
                    } catch(std::exception& e) {
                        std::cout << "error when displaying frame: " << e.what() << std::endl;
                    }
                }
            }
        }
    }

private:
    UdpClient& udpClient;
    const std::string windowName = "Control Window";
    std::thread uiThread;
    std::thread receiverThread;
    std::thread sendingThread;

    JsonReader data;
    std::queue<JsonReader> jsonQueue;
    std::mutex queueMutex;
    std::condition_variable dataCondition;

    DisplayBuilder displayBuilder;
    enums::Controller controller;

    std::queue<std::string> sendQueue;
    std::condition_variable sendCondition;
    std::mutex sendMutex;
    std::string messageToSend;

    cv::Mat latestFrame;
    std::mutex frameMutex;

    int MAX_QUEUE_SIZE = 1;

    void pushMessage(const std::string& message) {
        if (!message.empty()) {
            if (sendQueue.size() >= MAX_QUEUE_SIZE) {
                sendQueue.pop();
            }
            sendQueue.push(message);
            sendCondition.notify_all();
        }
    }

    void uiLoop() {
        KeyboardInput kbInput;
        CXBOXController xboxController(1);
        AutonomousControl autoControl;
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

                    key = cv::waitKey(1);
                    Action action = kbInput.interpretKey(key);
                    kbInput.performAction(action,  this->controller);
                    message = kbInput.getJsonMessageAsString();

                    std::unique_lock<std::mutex> lock(sendMutex);
                    pushMessage(message);
                }
                displayBuilder.destroyWindow();
            }
            else if (this -> controller == XBOX) {
                displayBuilder.buildXboxMenu();
                cv::waitKey(1);
                while (this->controller == XBOX) {

                    xboxController.run(controller);
                    message = xboxController.getJsonMessageAsString();
                    {
                        std::unique_lock<std::mutex> lock(sendMutex);
                        pushMessage(message);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));

                }
                displayBuilder.destroyWindow();
            }

            else if (this -> controller == AUTO){
                displayBuilder.buildAutonomousDrivingMenu();
                cv::waitKey(1);
                {
                    std::unique_lock<std::mutex> frameLock(frameMutex);
                    cv::Mat frame = latestFrame.clone();
                }
                cv::Mat currentFrame;
                while (this->controller == AUTO) {
                    {
                        std::unique_lock<std::mutex> frameLock(frameMutex);
                        currentFrame = latestFrame.clone();
                    }
                    autoControl.run(this->controller, currentFrame);
                    autoControl.selectController(this->controller);
                    message = autoControl.getJsonMessageAsString();

                    std::unique_lock<std::mutex> lock(sendMutex);
                    pushMessage(message);
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
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
            int fps = 144;
        if (!frame.empty()) {
            cv::resize(frame, frame, cv::Size(640, 480));
            cv::imshow(windowName, frame);
            cv::waitKey(1);
        } else {
            std::cerr << "Empty or invalid frame received.\n";
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));

    }
};

#endif // AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
