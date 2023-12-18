#ifndef AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP
#define AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP

#include "sphero/core/misc.hpp"
#include "sphero/core/ImageFetcher.hpp"
#include "sphero/core/ImageProcessor.hpp"


// This works as a GUI as of now
class ColorCalibrator: public ImageProcessor {
public:

    ColorCalibrator(const std::string& windowName = "Color Calibration")
        : windowName_(windowName), ImageProcessor() {
        cv::namedWindow(windowName);

        // HSV trackbars
        cv::createTrackbar("Min Hue", windowName, &colorValues_.H_min, 180);
        cv::createTrackbar("Max Hue", windowName, &colorValues_.H_max, 180);

        cv::createTrackbar("Min Saturation", windowName, &colorValues_.S_min, 255);
        cv::createTrackbar("Max Saturation", windowName, &colorValues_.S_max, 255);

        cv::createTrackbar("Min Value", windowName, &colorValues_.V_min, 255);
        cv::createTrackbar("Max Value", windowName, &colorValues_.V_max, 255);
    }

    void processImage () override {

        // Could be replaced with a condition variable instead of a busy wait
        while(running_){
            if(!newestFrame_.empty()){
                cv::Mat frame, hsvFrame, mask, result;
                {
                    std::lock_guard<std::mutex> lock(newestFrame_mutex_);
                    newestFrame_.copyTo(frame);
                }

                cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

                cv::inRange(hsvFrame,
                            cv::Scalar(colorValues_.H_min, colorValues_.S_min, colorValues_.V_min),
                            cv::Scalar(colorValues_.H_max, colorValues_.S_max, colorValues_.V_max),
                            mask);

                mask.copyTo(newestMask_);
            }
        }
    }

    [[nodiscard]] ColorValues getColorValues() const {
        return colorValues_;
    }

    void setColorValues(const ColorValues& colorValues) {
        colorValues_ = colorValues;
    }

    void getNewestMask(cv::Mat& mask){
        newestMask_.copyTo(mask);
    }

    bool isNewestFrameEmpty() {
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        return newestMask_.empty();
    }

private:
    ColorValues colorValues_;
    std::string windowName_;

    cv::Mat newestMask_;
};


#endif//AIS2203_PROJECT_SPHERO_COLORCALIBRATOR_HPP