#ifndef AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
#define AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP

#include "sphero/utils/udpClient.hpp"
#include "sphero/controls/keyboardInput.hpp"

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

    void receiving(){
        JsonReader jsonReader;
        std::string data;
        while (true){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            data = udpClient.receiveData();
            jsonReader.updateJson(data);
            jsonQueue.push(jsonReader);
            dataCondition.notify_all();
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
                    //std::cout<<"element removed from sendQueue"<<"\n";
                }
            }

            if (!message.empty()){
                //std::cout<<"sending"<<"\n";
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

        while (true) {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                dataCondition.wait(lock, hasData);

                while (!jsonQueue.empty()) {
                    data = jsonQueue.front();
                    jsonQueue.pop();

                    frame = data.getFrame();
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

    CameraControl cameraControl;
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
            //std::cout << "message to send: " << message << std::endl;
            sendCondition.notify_all();
        }
    }

    void uiLoop() {
        KeyboardInput kbInput;
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
                    // Get the message from controller
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

                RaspberryCamera camera;
                BallTracker tracker;
                camera.addObserver(&tracker);

                // Initializing ball tracker
                try{
                    tracker.setColor(fromJson(loadJson("saved_color_range.json")));
                }
                catch (std::exception& e){
                    std::cout << "Error: " << e.what() << std::endl;
                    std::cout << "Using default color range, consider running color calibration first" << std::endl;
                    tracker.setColor(ColorValues{250, 255, 112, 200, 69, 134});
                }

                BallTrackerResult ball;
                cv::Point2f screenCenter;
                {
                    std::unique_lock<std::mutex> frameLock(frameMutex);
                    cv::Mat frame = latestFrame.clone();
                    screenCenter = cv::Point2f(frame.cols / 2, frame.rows / 2);
                }
                RobotControlValues control;

                cv::Mat currentFrame;
                while (this->controller == AUTO) {
                    {
                        std::unique_lock<std::mutex> frameLock(frameMutex);
                        currentFrame = latestFrame.clone();
                    }

                    camera.addFrame(currentFrame);
                    // Ball tracker is automatically notified

                    ball = tracker.getResult();
                    std::cout<<"debug1"<<std::endl;

                    control.setObjectHeading(ball, screenCenter);
                    control.setObjectSpeed(ball, screenCenter);

                    message = control.getJsonMessageAsString();
                    std::cout<<"debug2"<<std::endl;
                    std::unique_lock<std::mutex> lock(sendMutex);//bruk ditte til å sende data til roboten.
                    pushMessage(message);
                    std::cout<<"debug3"<<std::endl;
                }
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
        //std::cout << "display"<<std::endl;
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
