#ifndef AIS2203_PROJECT_SPHERO_IMAGEFETCHER_HPP
#define AIS2203_PROJECT_SPHERO_IMAGEFETCHER_HPP


#include <opencv2/opencv.hpp>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

class ImageFetcher {
public:
    virtual ~ImageFetcher() {
        stop();
    }

    void start() {
        if (!captureThread_ || !captureThread_->joinable()) {
            stop_ = false;
            captureThread_ = std::make_unique<std::thread>(&ImageFetcher::captureLoop, this);
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

protected:
    virtual void captureLoop() = 0; // Pure virtual function

    std::queue<cv::Mat> frameQueue_;
    std::mutex mutex_;
    std::atomic<bool> stop_{false};
    std::unique_ptr<std::thread> captureThread_;
};


#endif//AIS2203_PROJECT_SPHERO_IMAGEFETCHER_HPP