#ifndef AIS2203_PROJECT_SPHERO_LIVEFRAMEDISPLAY_HPP
#define AIS2203_PROJECT_SPHERO_LIVEFRAMEDISPLAY_HPP

# include "sphero/core/ImageFetcher.hpp"

# include <opencv2/opencv.hpp>
# include <mutex>

// This is a class that can be used to display the newest frame from a camera
// When different image processing techniques take their time to work on the
// frame, this class can be used to display the newest frame without the delays.
// This class is made to run on the main thread.

class LiveFrameDisplay: public Observer {
public:
    void onFrameAvailable(const cv::Mat& frame) override {
        updateNewestFrame(frame);
    }

    void updateNewestFrame(const cv::Mat& frame) {
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        frame.copyTo(newestFrame_);
    }

    void getNewestFrame(cv::Mat& frame){
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        newestFrame_.copyTo(frame);
    }

    bool isNewestFrameEmpty() {
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        return newestFrame_.empty();
    }

private:
    std::mutex newestFrame_mutex_;
    cv::Mat newestFrame_;
};

#endif//AIS2203_PROJECT_SPHERO_LIVEFRAMEDISPLAY_HPP
