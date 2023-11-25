#ifndef AIS2203_PROJECT_SPHERO_BALLTRACKER_HPP
#define AIS2203_PROJECT_SPHERO_BALLTRACKER_HPP

#include "sphero/core/misc.hpp"
#include "sphero/core/ImageFetcher.hpp"

#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>

struct BallTrackerResult {
    cv::Point2f center;
    float radius;
    bool found;
    std::mutex mutex;
};

class BallTracker : public Observer{
public:
    BallTracker(): running_(false) { /* May be smart to initialize default ball colors */
    }

    void onFrameAvailable(const cv::Mat& frame) override {
        // Update newest frame
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        frame.copyTo(newestFrame_);
    }

    ~BallTracker() {
        running_ = false;
        if (trackingThread_.joinable()) {
            trackingThread_.join();
        }
    }

    void setColor(const ColorValues& ballColor) {
        ballColor_ = ballColor;
    }

    ColorValues getColor() const {
        return ballColor_;
    }

    void startTracking() {
        if (trackingThread_.joinable())
            trackingThread_.join();

        running_ = true;
        trackingThread_ = std::thread([this]() {
            this->detectBall();
        });

        trackingThread_.detach();
    }

    void stopTracking() {
        running_ = false;
        if (trackingThread_.joinable()) {
            trackingThread_.join();
        }
    }

    BallTrackerResult getResult() {

//        BallTrackerResult local_result;
//        {
//            std::lock_guard<std::mutex> lock(result_.mutex);
//            local_result = result_;
//        }
//        return local_result;

        std::lock_guard<std::mutex> lock(result_.mutex);
        return result_;
    }

    void detectBall() {
        while (running_) {
            cv::Mat frame;// Obtain the frame from your image source
            {
                std::lock_guard<std::mutex> lock(newestFrame_mutex_);
                newestFrame_.copyTo(frame);
            }

            // Convert to a suitable color space if necessary

            // Apply color thresholding to detect the ball
            cv::Mat mask;
            //            cv::inRange(frame, lowerBound, upperBound, mask);
            cv::inRange(frame,
                        cv::Scalar(ballColor_.B_min, ballColor_.G_min, ballColor_.R_min),
                        cv::Scalar(ballColor_.B_max, ballColor_.G_max, ballColor_.R_max),
                        mask);

            // Find contours
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            // Detect the ball (assuming it's the largest contour)
            float maxRadius = 0;
            cv::Point2f maxCenter;
            for (const auto& contour : contours) {
                float radius;
                cv::Point2f center;
                cv::minEnclosingCircle(contour, center, radius);
                if (radius > maxRadius) {
                    maxRadius = radius;
                    maxCenter = center;
                }
            }

            {
                std::lock_guard<std::mutex> lock(result_.mutex);
                if (maxRadius > 0) {// Ball found
                    result_.center = maxCenter;
                    result_.radius = maxRadius;
                    result_.found = true;
                } else {// Ball not found
                    result_.found = false;
                }
            }
        }
        stopTracking();
    }

private:
    ColorValues ballColor_;
    std::atomic<bool> running_;
    std::thread trackingThread_;

    std::mutex result_mutex_;
    BallTrackerResult result_;

    std::mutex newestFrame_mutex_;
    cv::Mat newestFrame_;
};


#endif//AIS2203_PROJECT_SPHERO_BALLTRACKER_HPP