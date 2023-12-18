#ifndef AIS2203_PROJECT_SPHERO_IMAGEFETCHER_HPP
#define AIS2203_PROJECT_SPHERO_IMAGEFETCHER_HPP

#include <opencv2/opencv.hpp>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>


class Observer {
public:
    virtual void onFrameAvailable(const cv::Mat& frame) = 0;
    virtual ~Observer() = default;

    static void blurFrame(cv::Mat& frame, int value = 9){
        cv::GaussianBlur(frame, frame, cv::Size(value, value), 0);
    }
};


class ImageFetcher {
public:
    ~ImageFetcher() {
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

    void addFrame(const cv::Mat& frame) {
        std::unique_lock<std::mutex> lock(mutex_);
        frameQueue_.push(frame);
    }

    void addObserver(Observer* observer) {
        std::lock_guard<std::mutex> lock(observers_mutex_);
        observers_.emplace_back(observer);
    }

    void removeObserver(Observer* observer) {
        std::lock_guard<std::mutex> lock(observers_mutex_);
        observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }



protected:
    virtual void captureLoop() = 0; //

    std::queue<cv::Mat> frameQueue_;
    std::mutex mutex_;
    std::atomic<bool> stop_{false};
    std::unique_ptr<std::thread> captureThread_;

    std::mutex observers_mutex_;
    std::vector<Observer*> observers_;


    void notifyObservers() {
        std::vector<Observer*> copyObservers;
        {
            std::lock_guard<std::mutex> lock(observers_mutex_);
            copyObservers = observers_;
        }

        cv::Mat frame;
        getFrame(frame);

        for (auto& observer : copyObservers) {
            observer->onFrameAvailable(frame);
        }
    }
};


#endif//AIS2203_PROJECT_SPHERO_IMAGEFETCHER_HPP