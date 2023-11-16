#ifndef AIS2203_PROJECT_SPHERO_USBCAMERA_HPP
#define AIS2203_PROJECT_SPHERO_USBCAMERA_HPP

#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>

class USBCamera {
public:
    USBCamera(int cameraIndex = 0) : cameraIndex_(cameraIndex), stop_(false) {}

    ~USBCamera() {
        stop();
    }

    void start() {
        if (!captureThread_ || !captureThread_->joinable()) {
            stop_ = false;
            captureThread_ = std::make_unique<std::thread>(&USBCamera::captureLoop, this);
        }
    }

    void stop() {
        stop_ = true;
        if (captureThread_ && captureThread_->joinable()) {
            captureThread_->join();
        }
    }

    bool getFrame(cv::Mat& frame) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (frameQueue_.empty()) {
            return false;
        }
        frame = frameQueue_.front();
        frameQueue_.pop();
        return true;
    }

private:
    int cameraIndex_;
    std::unique_ptr<std::thread> captureThread_;
    std::mutex mutex_;
    std::condition_variable frameAvailable_;
    std::queue<cv::Mat> frameQueue_;
    std::atomic<bool> stop_;

    void captureLoop() {
        cv::VideoCapture capture(cameraIndex_);
        if (!capture.isOpened()) {
            std::cerr << "Error: Cannot open camera" << std::endl;
            return;
        }

        while (!stop_) {
            cv::Mat frame;
            if (capture.read(frame)) {
                std::lock_guard<std::mutex> lock(mutex_);
                frameQueue_.push(frame);
                frameAvailable_.notify_one();
            }
        }
    }
};

#endif//AIS2203_PROJECT_SPHERO_USBCAMERA_HPP