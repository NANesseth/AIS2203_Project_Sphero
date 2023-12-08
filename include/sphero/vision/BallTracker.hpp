#ifndef AIS2203_PROJECT_SPHERO_BALLTRACKER_HPP
#define AIS2203_PROJECT_SPHERO_BALLTRACKER_HPP

#include "sphero/core/misc.hpp"
#include "sphero/core/ImageFetcher.hpp"

#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>

// If we want the possibility to track different objects, we can create a class
// ObjectTracker that inherits from Observer and derive BallTracker from it.

// class ObjectTracker : public Observer
//      class BallTracker : public ObjectTracker
//      class QubeTracker : public ObjectTracker etc...

class BallTracker : public Observer{
public:
    BallTracker(): running_(false) { /* May be smart to initialize default ball colors */
    }

    void onFrameAvailable(const cv::Mat& frame) override {
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
        std::lock_guard<std::mutex> lock(result_mutex_);
        return ball_;
    }

//    cv::Point2f getRelativePosition(int screenWidth, int screenHeight) {
//        // calculate screen center
//        cv::Point2f screenCenter(screenWidth / 2, screenHeight / 2);
//
//        // compute the relative position
//        cv::Point2f relativePos = ball_.center - screenCenter;
//
//        return relativePos;
//    }

    cv::Point2f getRelativePosition(int screenWidth, int screenHeight) {
        // calculate screen center
        cv::Point2f screenCenter(screenWidth / 2, screenHeight / 2);

        // compute the relative position
        cv::Point2f relativePos = ball_.center - screenCenter;

        // normalize the coordinates to range -1 to 1
        relativePos.x /= (screenWidth / 2);
        relativePos.y /= (screenHeight / 2);

        return relativePos;
    }

    void detectBall() {
        while (running_) {
            cv::Mat frame;// Obtain the frame from your image source
            {
                std::lock_guard<std::mutex> lock(newestFrame_mutex_);
                newestFrame_.copyTo(frame);
            }

            // Apply color thresholding to detect the ball
            cv::Mat mask;

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
                std::lock_guard<std::mutex> lock(result_mutex_);
                if (maxRadius > 0) {// Ball found
                    ball_.center = maxCenter;
                    ball_.radius = maxRadius;
                    ball_.found = true;
                } else {// Ball not found
                    ball_.found = false;
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
    BallTrackerResult ball_;

    std::mutex newestFrame_mutex_;
    cv::Mat newestFrame_;
};


#endif//AIS2203_PROJECT_SPHERO_BALLTRACKER_HPP