#ifndef AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP
#define AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP

#include "sphero/core/misc.hpp"
#include "sphero/core/ImageFetcher.hpp"

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp> // Should be removed
#include <string>
#include <fstream>
#include <iostream>

// This works as a GUI as of now
class ColorCalibrator: public Observer{
public:

    ColorCalibrator(const std::string& windowName = "Color Calibration")
        : windowName(windowName) {
        cv::namedWindow(windowName);

        cv::createTrackbar("Min Red", windowName, &colorValues_.R_min, 255);
        cv::createTrackbar("Max Red", windowName, &colorValues_.R_max, 255);

        cv::createTrackbar("Min Green", windowName, &colorValues_.G_min, 255);
        cv::createTrackbar("Max Green", windowName, &colorValues_.G_max, 255);

        cv::createTrackbar("Min Blue", windowName, &colorValues_.B_min, 255);
        cv::createTrackbar("Max Blue", windowName, &colorValues_.B_max, 255);
    }

    void onFrameAvailable(const cv::Mat& frame) override {
        processFrame(frame);
    }

    void processFrame(const cv::Mat& frame) {
        cv::Mat mask, result;

        cv::inRange(frame,
                    cv::Scalar(colorValues_.B_min, colorValues_.G_min, colorValues_.R_min),
                    cv::Scalar(colorValues_.B_max, colorValues_.G_max, colorValues_.R_max),
                    mask);

        cv::bitwise_and(frame, frame, result, mask);

        //cv::imshow(windowName, result);
        {
            std::lock_guard<std::mutex> lock(mutex_);
            result.copyTo(newestFrame);
        }

        //cv::waitKey(1);
    }

    [[nodiscard]] ColorValues getColorValues() const {
        return colorValues_;
    }

    void setColorValues(const ColorValues& colorValues) {
        colorValues_ = colorValues;
    }

    cv::Mat getNewestFrame() {
        std::lock_guard<std::mutex> lock(mutex_);
        return newestFrame.clone();
    }

private:
    std::string windowName;

    std::mutex mutex_;
    cv::Mat newestFrame;

    ColorValues colorValues_;
};

#endif//AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP