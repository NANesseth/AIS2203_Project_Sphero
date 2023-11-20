#ifndef AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP
#define AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP

#include "sphero/core/misc.hpp"

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp> // Should be removed
#include <string>
#include <fstream>
#include <iostream>

class ColorCalibrator {
public:

    ColorCalibrator(const std::string& windowName = "Color Calibration")
        : windowName(windowName) {
        cv::namedWindow(windowName);

        // Create trackbars for color change
        // Initial min and max values can be adjusted as needed

        cv::createTrackbar("Min Red", windowName, &colorValues_.R_min, 255);
        cv::createTrackbar("Max Red", windowName, &colorValues_.R_max, 255);

        cv::createTrackbar("Min Green", windowName, &colorValues_.G_min, 255);
        cv::createTrackbar("Max Green", windowName, &colorValues_.G_max, 255);

        cv::createTrackbar("Min Blue", windowName, &colorValues_.B_min, 255);
        cv::createTrackbar("Max Blue", windowName, &colorValues_.B_max, 255);

    }

    void processFrame(const cv::Mat& frame) {
        cv::Mat mask, result;

        cv::inRange(frame,
                    cv::Scalar(colorValues_.B_min, colorValues_.G_min, colorValues_.R_min),
                    cv::Scalar(colorValues_.B_max, colorValues_.G_max, colorValues_.R_max),
                    mask);

        cv::bitwise_and(frame, frame, result, mask);

        cv::imshow(windowName, result);
        cv::waitKey(1);
    }

    [[nodiscard]] ColorValues getColorValues() const {
        return colorValues_;
    }

    void setColorValues(const ColorValues& colorValues) {
        colorValues_ = colorValues;
    }

private:
    std::string windowName;

    ColorValues colorValues_;
};

#endif//AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP