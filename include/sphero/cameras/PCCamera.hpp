#ifndef AIS2203_PROJECT_SPHERO_PCCAMERA_HPP
#define AIS2203_PROJECT_SPHERO_PCCAMERA_HPP

#include "sphero/core/ImageFetcher.hpp"

#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>

// This class can be used for any camera recognized
// by the computer, even a builtin laptop webcam

class PCCamera: public ImageFetcher {
public:
    explicit PCCamera(int cameraIndex=0) : cameraIndex_(cameraIndex) {}

protected:
    void captureLoop() override {
        cv::VideoCapture capture(cameraIndex_ + cv::CAP_DSHOW);

        if (!capture.isOpened()) {
            std::cerr << "Error: Cannot open camera" << std::endl;
            return;
        }

        while (!stop_) {
            cv::Mat frame;

             if (capture.read(frame)) {
                 addFrame(frame);
                 notifyObservers();

             } else {
                 std::this_thread::sleep_for(std::chrono::milliseconds(10));
             }
//            if (capture.read(frame)) {
//                std::lock_guard<std::mutex> lock(mutex_);
//                frameQueue_.push(frame);
//                // frameAvailable_.notify_one(); // If using condition variables
//                notifyObservers();
//            } else {
//                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep on failed capture
//            }
        }
    }

private:
    int cameraIndex_;
};

#endif//AIS2203_PROJECT_SPHERO_PCCAMERA_HPP