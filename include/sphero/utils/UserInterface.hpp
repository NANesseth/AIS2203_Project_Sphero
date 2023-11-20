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

class UserInterface {
public:
    explicit UserInterface(UdpClient& client) : udpClient(client), controller(enums::Controller::NOCONTROLLER) {
        cv::namedWindow(windowName);
    }
    ~UserInterface() {
        videoRunning.store(false);
        if (uiThread.joinable()) {
            uiThread.join();
        }
        if (networkThread.joinable()) {
            networkThread.join();
        }
        cv::destroyWindow(windowName);
    }

    void send_input(std::string& input) {
        if (!input.empty()){
            if (input != "exit") {
                udpClient.sendMessage(input);
                std::cout<<input<<" sent"<<std::endl;
                if (input == "video") {
                    videoRunning.store(true);
                }
            } else {
                std::cout << "Exiting application.\n";
                videoRunning.store(false);
            }
        }
    }

    void networking() {
        while(true) {//TODO: make this smarter. need a stopflag
            std::cout << "entered networking" << std::endl;
            cv::Mat frame;
            while (videoRunning.load()) {
                std::cout << "loaded video running" << std::endl;
                auto result = udpClient.jsonReader("frame");
                if (std::holds_alternative<cv::Mat>(result)) {
                    frame = std::get<cv::Mat>(result);
                }
                else{
                    std::cout<<"jsonReader failed"<<std::endl;
                }

                std::unique_lock<std::mutex> lock(queueMutex);
                std::cout << "locked mutex net" << std::endl;
                if(frameQueue.size() >= 10){
                    frameQueue.pop();
                }
                frameQueue.push(std::move(frame));
                std::cout << "frame added to queue" << std::endl;
                // Notify the display thread that a new frame is available
                frameCondition.notify_one();
                std::cout << "notified frame condition" << std::endl;

                std::cout << "frame added to queue" << std::endl;
            }
            while (!videoRunning.load()) {
            std::cout << "Video is not running, thread sleeping for 1 second" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }


    void run(){
        uiThread = std::thread(&UserInterface::uiLoop, this);
        networkThread = std::thread(&UserInterface::networking, this);

        while(true) {
            if (videoRunning.load()) {
                cv::Mat frame;
                {
                    std::cout << "entered display mut next" << std::endl;
                    std::unique_lock<std::mutex> lock(queueMutex);
                    std::cout << "locked mutex display" << std::endl;
                    // Wait for the condition variable to notify that there's a new frame or the video stops running
                    frameCondition.wait(lock, [&]() { return !frameQueue.empty() || !videoRunning; });
                    if (!videoRunning) {
                        break;
                    }
                    std::cout << "got frame from queue" << std::endl;

                    frame = std::move(frameQueue.front());
                    std::cout << "retrieved frame from queue" << std::endl;
                    frameQueue.pop();

                }
                std::cout << "displaying frame" << std::endl;
                displayFrame(frame);
                std::cout << "frame displayed" << std::endl;
            }
        }

        if (uiThread.joinable()) {
            uiThread.join();
        }
        if (networkThread.joinable()) {
            networkThread.join();
        }
        std::cout<<"threads joined"<<std::endl;
    }

private:
    UdpClient& udpClient;
    const std::string windowName = "Control Window";
    std::thread uiThread;
    std::thread networkThread;
    std::atomic<bool> videoRunning = false;

    std::queue<cv::Mat> frameQueue;
    std::mutex queueMutex;
    std::condition_variable frameCondition;

    DisplayBuilder displayBuilder;
    enums::Controller controller;

    CameraControl cameraControl;




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
                    key = cv::waitKey(0);
                    Action action = kbInput.interpretKey(key);
                    kbInput.performAction(action, videoRunning, frameCondition,  this->controller);//skal returnere en string som e slik: "msg,speed,heading"
                    message = kbInput.getJsonMessageAsString();
                    std::cout<<"Controller is "<< controller <<std::endl;
                    send_input(message);
                }
                displayBuilder.destroyWindow();
            }
            else if (this -> controller == XBOX){
                displayBuilder.buildXboxMenu();
                cv::waitKey(1);
                while(this-> controller == XBOX){
                    // TODO: implement xbox controller
                    xboxController.run(videoRunning, frameCondition, this->controller);
                    // Get the message from controller
                    message = xboxController.getJsonMessageAsString();

                    send_input(message);
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                displayBuilder.destroyWindow();

            }
            else{
                displayBuilder.buildMainMenu();
                cv::waitKey(1);
                while(this->controller == NOCONTROLLER && !stopflag){
                    stopflag = kbInput.selectController(this->controller);
                    std::cout<<"controller is "<< controller <<std::endl;
                }
                displayBuilder.destroyWindow();
                }
            }
        }

    void displayFrame(cv::Mat& frame) {
        std::cout << "display"<<std::endl;
        if (!frame.empty()) {
            cv::imshow(windowName, frame);
            cv::waitKey(1);
        } else {
            std::cerr << "Empty or invalid frame received.\n";
        }
    }
};

#endif // AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
