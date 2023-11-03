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

class UserInterface {
private:
    UdpClient& udpClient;
    const std::string windowName = "Control Window";
    std::thread uiThread;
    std::thread networkThread;
    std::atomic<bool> videoRunning = false;

    std::queue<cv::Mat> frameQueue;
    std::mutex queueMutex;
    std::condition_variable frameCondition;

    void uiLoop() { //TODO: split this into smaller parts
        char key;
        std::string msg;
        const char ESC_KEY = 27;
        cv::namedWindow("controlWindow");

        while (true) {
            // Handle input
            key = (char)cv::waitKey(10);
            if (key == ESC_KEY || key == 'v' || key == 'c' || key == 'w' || key == 's' || key == 'a' || key == 'd' || key == 'g') {
                std::cout << key << " is pressed" << std::endl;
            }
            switch (key) {
                case 'w': msg = "forward"; break;
                case 's': msg = "backward"; break;
                case 'a': msg = "left"; break;
                case 'd': msg = "right"; break;
                case 'v':
                    if (!videoRunning.load()) {
                        msg = "video";
                        videoRunning.store(true);
                    }
                    break;
                case 'c':
                    if (videoRunning.load()) {
                        msg = "stop_video";
                        videoRunning.store(false);
                        frameCondition.notify_all(); // Wake up any waiting threads
                    }
                    break;
                case 'g': msg = "gas"; break;
                case ESC_KEY:
                    std::cout << "ESC key pressed. Exiting.\n";
                    msg = "exit";
                    videoRunning.store(false);
                    break;
                default:
                    if (key != -1) { // -1 corresponds to no key being pressed
                        std::cout << "no command\n";
                    }
                    break;
            }
            send_input(msg);

            if (msg == "exit") {
                break;
            }

            msg.clear(); // Clear the message for the next input
        }

        cv::destroyWindow(windowName);
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


public:
    UserInterface(UdpClient& client) : udpClient(client) {
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
            std::cout << "entered networkin" << std::endl;
            while (videoRunning.load()) {
                cv::Mat newFrame = udpClient.receiveFrame();
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    if(frameQueue.size() >= 10){
                        frameQueue.pop();
                    }
                    frameQueue.push(std::move(newFrame));
                    // Notify the display thread that a new frame is available
                    frameCondition.notify_one();
                }
                std::cout << "frame added to queue" << std::endl;
            }
            while (!videoRunning.load()) {
            std::cout << "Video is not running, thread sleeping for 1 second" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }

    cv::Mat getNewFrame(){
        return this->udpClient.receiveFrame();
    }

    void run(){
        uiThread = std::thread(&UserInterface::uiLoop, this);
        networkThread = std::thread(&UserInterface::networking, this);

        while(true) {
            if (videoRunning.load()) {
                cv::Mat frame;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    std::cout << "testr" << std::endl;

                    // Wait for the condition variable to notify that there's a new frame or the video stops running
                    frameCondition.wait(lock, [&]() { return !frameQueue.empty() || !videoRunning; });
                    if (!videoRunning) {
                        break;
                    }
                    // Get the new frame from the queue

                    frame = std::move(frameQueue.front());
                    std::cout << "retrieved frame from queue" << std::endl;
                    frameQueue.pop();
                    std::cout << "testeepoped" << std::endl;

                }
                std::cout << "testeeaweed" << std::endl;
                displayFrame(frame);

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
};

#endif // AIS2203_PROJECT_SPHERO_USERINTERFACE_HPP
