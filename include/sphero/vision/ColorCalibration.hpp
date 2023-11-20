#ifndef AIS2203_PROJECT_SPHERO_COLORCALIBRATION_HPP
#define AIS2203_PROJECT_SPHERO_COLORCALIBRATION_HPP

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
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
        cv::createTrackbar("Min Red", windowName, &minRed, 255);
        cv::createTrackbar("Max Red", windowName, &maxRed, 255);
        cv::createTrackbar("Min Green", windowName, &minGreen, 255);
        cv::createTrackbar("Max Green", windowName, &maxGreen, 255);
        cv::createTrackbar("Min Blue", windowName, &minBlue, 255);
        cv::createTrackbar("Max Blue", windowName, &maxBlue, 255);
    }

    void processFrame(const cv::Mat& frame) {
        cv::Mat mask, result;
        cv::inRange(frame, cv::Scalar(minBlue, minGreen, minRed), cv::Scalar(maxBlue, maxGreen, maxRed), mask);
        cv::bitwise_and(frame, frame, result, mask);

        cv::imshow(windowName, result);
        cv::waitKey(1);
    }

    void saveColorRange(const std::string& filename) {
        nlohmann::json json;

        json["min_color"] = {minRed, minGreen, minBlue};
        json["max_color"] = {maxRed, maxGreen, maxBlue};

        std::ofstream file(filename);
        if (file.is_open()) {
            file << json.dump(4); // 4 is for pretty printing
            file.close();
            std::cout << "Color range saved to " << filename << std::endl;
        } else {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
        }
    }

    // Function to get the current color range values
    cv::Scalar getMinColor() const {
        return cv::Scalar(minBlue, minGreen, minRed);
    }

    cv::Scalar getMaxColor() const {
        return cv::Scalar(maxBlue, maxGreen, maxRed);
    }

private:
    std::string windowName;
    int minRed = 0, maxRed = 255;
    int minGreen = 0, maxGreen = 255;
    int minBlue = 0, maxBlue = 255;
};

#endif//AIS2203_PROJECT_SPHERO_COLORCALIBRATION_HPP