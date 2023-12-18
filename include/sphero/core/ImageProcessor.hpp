#ifndef AIS2203_PROJECT_SPHERO_IMAGEPROCESSOR_HPP
#define AIS2203_PROJECT_SPHERO_IMAGEPROCESSOR_HPP

# include "sphero/core/ImageFetcher.hpp"

// For the types of observers that does more work and needs to be run in a separate thread
class ImageProcessor: public Observer {
public:
    ImageProcessor(): running_(false) {}

    void onFrameAvailable(const cv::Mat& frame) override {
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        frame.copyTo(newestFrame_);
    }

    ~ImageProcessor() override {
        running_ = false;
        if (processingThread_.joinable()) {
            processingThread_.join();
        }
    }

    void start() {
        if (processingThread_.joinable())
            processingThread_.join();

        running_ = true;
        processingThread_ = std::thread([this]() {
            this->processImage();
        });

        processingThread_.detach();
    }

    void stop() {
        running_ = false;
        if (processingThread_.joinable()) {
            processingThread_.join();
        }
    }

protected:
    virtual void processImage() = 0;

    std::mutex newestFrame_mutex_;
    cv::Mat newestFrame_;

    std::atomic<bool> running_;
    std::thread processingThread_;
};


#endif//AIS2203_PROJECT_SPHERO_IMAGEPROCESSOR_HPP
