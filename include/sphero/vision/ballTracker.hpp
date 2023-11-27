#ifndef AIS2203_PROJECT_SPHERO_BALLTRACKER_HPP
#define AIS2203_PROJECT_SPHERO_BALLTRACKER_HPP

#include "sphero/core/misc.hpp"
#include "sphero/core/ImageFetcher.hpp"

#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>


struct BallTrackerResult {
    cv::Point2f center{};
    float radius{};
    bool found{};
    //std::mutex mutex;
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
        std::lock_guard<std::mutex> lock(result_mutex_);
        return result_;
    }

    void detectBall() {
        float minRadius = 5;
        float radius, maxRadius;
        cv::Point2f maxCenter;
        double area, perimeter, circularity, circularityThreshold = 0.85;

        while (running_) {
            cv::Mat frame;// Obtain the frame from your image source
            {
                std::lock_guard<std::mutex> lock(newestFrame_mutex_);
                newestFrame_.copyTo(frame);
            }
            blurFrame(frame);

            // Apply color thresholding to detect the ball
            cv::Mat mask;

            cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);

            cv::inRange(frame,
                        cv::Scalar(ballColor_.H_min, ballColor_.S_min, ballColor_.V_min),
                        cv::Scalar(ballColor_.H_max, ballColor_.S_max, ballColor_.V_max),
                        mask);

            // Find contours
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            // Detect the ball (assuming it's the largest contour)
            maxRadius = 0;
            for (const auto& contour : contours) {
                radius = 0;
                cv::Point2f center;
                cv::minEnclosingCircle(contour, center, radius);

                // Continue if radius is lower than minimum radius
                if (radius < minRadius) {
                    continue;
                }

                // Calculate area and perimeter
                area = cv::contourArea(contour);
                perimeter = cv::arcLength(contour, true);

                // Calculate circularity
                circularity = 4 * CV_PI * (area / (perimeter * perimeter));

                // Only consider the contour if the circularity is high (closer to 1)
                // circularityThreshold = 0.85; // This value can be adjusted based on your needs
                if (radius > maxRadius && circularity > circularityThreshold) {
                    maxRadius = radius;
                    maxCenter = center;
                }
            }

            {
                std::lock_guard<std::mutex> lock(result_mutex_);
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