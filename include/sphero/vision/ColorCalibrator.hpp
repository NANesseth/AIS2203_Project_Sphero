#ifndef AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP
#define AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP

#include "sphero/core/misc.hpp"
#include "sphero/core/ImageFetcher.hpp"

//#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp> // Should be removed
#include <string>
#include <fstream>
#include <iostream>

// This works as a GUI as of now
class ColorCalibrator: public Observer {
public:

    ColorCalibrator(const std::string& windowName = "Color Calibration")
        : windowName_(windowName) {
        cv::namedWindow(windowName);

        // HSV trackbars
        cv::createTrackbar("Min Hue", windowName, &colorValues_.H_min, 180);
        cv::createTrackbar("Max Hue", windowName, &colorValues_.H_max, 180);

        cv::createTrackbar("Min Saturation", windowName, &colorValues_.S_min, 255);
        cv::createTrackbar("Max Saturation", windowName, &colorValues_.S_max, 255);

        cv::createTrackbar("Min Value", windowName, &colorValues_.V_min, 255);
        cv::createTrackbar("Max Value", windowName, &colorValues_.V_max, 255);
    }

    void onFrameAvailable(const cv::Mat& frame) override {
        processFrame(frame);
    }

    void processFrame(const cv::Mat& frame) {
        cv::Mat hsvFrame, mask, result;

        // Convert to HSV
        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

        cv::inRange(hsvFrame,
                    cv::Scalar(colorValues_.H_min, colorValues_.S_min, colorValues_.V_min),
                    cv::Scalar(colorValues_.H_max, colorValues_.S_max, colorValues_.V_max),
                    mask);

        cv::bitwise_and(frame, frame, result, mask);
        {
            std::lock_guard<std::mutex> lock(newestFrame_mutex_);
            result.copyTo(newestFrame);
        }
    }

    [[nodiscard]] ColorValues getColorValues() const {
        return colorValues_;
    }

    void setColorValues(const ColorValues& colorValues) {
        colorValues_ = colorValues;
    }

    cv::Mat getNewestFrame() {
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        return newestFrame.clone();
    }

private:
    std::string windowName_;

    std::mutex newestFrame_mutex_;
    cv::Mat newestFrame;

    std::mutex overlay_mutex_;
    cv::Mat overlay_;

    ColorValues colorValues_; // Update this structure to have H, S, and V fields
};


#endif//AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP