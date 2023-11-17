#ifndef AIS2203_PROJECT_SPHERO_OBJECTTRACKER_HPP
#define AIS2203_PROJECT_SPHERO_OBJECTTRACKER_HPP

#include <opencv2/opencv.hpp>

class ObjectTracker {
public:
    ObjectTracker() {
        // Initialize color range for detection
        // These values should be adjusted according to the color of your ball
        lowerColorBound = cv::Scalar(29, 86, 6);
        upperColorBound = cv::Scalar(64, 255, 255);
    }

    void processFrame(const cv::Mat &frame) {
        cv::Mat processedFrame;
        cv::cvtColor(frame, processedFrame, cv::COLOR_BGR2HSV);

        cv::Mat mask;
        cv::inRange(processedFrame, lowerColorBound, upperColorBound, mask);

        // Find contours
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Find the largest contour and assume it is the ball
        double maxArea = 0;
        std::vector<cv::Point> largestContour;
        for (const auto& contour : contours) {
            double area = cv::contourArea(contour);
            if (area > maxArea) {
                maxArea = area;
                largestContour = contour;
            }
        }

        if (!largestContour.empty()) {
            // Compute the center of the contour
            cv::Moments moments = cv::moments(largestContour);
            cv::Point2f center(moments.m10 / moments.m00, moments.m01 / moments.m00);

            // Draw the contour and center on the frame
            cv::drawContours(frame, std::vector<std::vector<cv::Point>>{largestContour}, -1, cv::Scalar(0, 255, 0), 2);
            cv::circle(frame, center, 5, cv::Scalar(0, 0, 255), -1);

            // Update the tracked position (you can use this for further processing)
            trackedPosition = center;
        }

        // For demonstration purposes, show the processed frame
        cv::imshow("Tracked Ball", frame);
        cv::waitKey(1);
    }

private:
    cv::Scalar lowerColorBound, upperColorBound;
    cv::Point2f trackedPosition; // Last known position of the ball
};


#endif//AIS2203_PROJECT_SPHERO_OBJECTTRACKER_HPP