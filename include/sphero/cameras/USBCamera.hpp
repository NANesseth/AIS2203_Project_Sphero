#ifndef AIS2203_PROJECT_SPHERO_USBCAMERA_HPP
#define AIS2203_PROJECT_SPHERO_USBCAMERA_HPP

#include "sphero/core/ImageFetcher.hpp"

#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>


class USBCamera : public ImageFetcher {
public:
    explicit USBCamera(int cameraIndex) : cameraIndex_(cameraIndex) {}

protected:
    void captureLoop() override {
        //cv::VideoCapture capture(cameraIndex_);
        cv::VideoCapture capture(cameraIndex_ + cv::CAP_DSHOW); // Significantly faster initialization

        if (!capture.isOpened()) {
            std::cerr << "Error: Cannot open camera" << std::endl;
            return;
        }

        while (!stop_) {
            cv::Mat frame;
            if (capture.read(frame)) {
                std::lock_guard<std::mutex> lock(mutex_);
                frameQueue_.push(frame);
                // frameAvailable_.notify_one(); // If using condition variables
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep on failed capture
            }
        }
    }

private:
    int cameraIndex_;
};

#endif//AIS2203_PROJECT_SPHERO_USBCAMERA_HPP