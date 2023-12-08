//
// Created by Torstein on 08/12/2023.
//

#ifndef AIS2203_PROJECT_SPHERO_AUTONOMOUS_HPP
#define AIS2203_PROJECT_SPHERO_AUTONOMOUS_HPP
#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>
#include "sphero/utils/enums.hpp"
#include "sphero/utils/Json reader.hpp"
#include <nlohmann/json.hpp>

class AutonomousControl {
    public:
        const int getSpeed() const {
            return speed;
        }

        const int getHeading() const {
            return heading;
        }


        bool selectController(enums::Controller& controller){
            using namespace enums;

            char key;
            const char ESC_KEY = 27;
            key = (char)cv::waitKey(10);
            switch (key) {
                case ESC_KEY: stopflag = true; break;
                default: break;
            }
            //std::cout<<"Controller is "<< controller <<std::endl;
            return stopflag;
        }
        std::string getJsonMessageAsString(){
            nlohmann::json message = {
                    {"message", msg},
                    {"speed", speed},
                    {"heading", heading},
                    {"panPosition", panPosition},
                    {"tiltPosition", tiltPosition}
            };
            return message.dump();
        }

        void run(enums::Controller& controller, cv::Mat frame) {
            msg = "drive";
            cv::Mat hsvImage;
            cv::cvtColor(frame, hsvImage, cv::COLOR_BGR2HSV);

            // Define the range for green color
            cv::Scalar lowerGreen(50, 100, 100); // Adjust these values
            cv::Scalar upperGreen(70, 255, 255); // Adjust these values

            // Threshold the image to get only green colors
            cv::Mat greenMask;
            cv::inRange(hsvImage, lowerGreen, upperGreen, greenMask);

            // Find contours
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(greenMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

            // Find the largest contour and its center
            double maxArea = 0;
            cv::Point2f center;
            float radius = 0;
            for (const auto& contour : contours) {
                float area = cv::contourArea(contour);
                if (area > maxArea) {
                    maxArea = area;
                    cv::minEnclosingCircle(contour, center, radius);
                }
            }

            if (maxArea > 0) {
                float ballSize = radius*2;
                float ballPosition = center.x;
                speed = int(70-ballSize);

                std::cout << "Green ball found at: (" << ballPosition << ", " << center.y << ") with size: " << ballSize << " pixels" << std::endl;
                if (ballPosition < 132.5) {
                    heading = heading + headingIncrement;
                }
                else if (ballPosition > 132.5) {
                    heading = heading - headingIncrement;
                }
            }
            else {
                std::cout << "No green ball found." << std::endl;
                heading = heading - headingIncrement;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }






        cv::Mat getFrameWithMarkedBall(){
            return cv::Mat();
        }

    private:

        std::tuple<int,int> ballCoordinates;
        int speed = 0;
        int heading = 0;
        std::string msg= "video_start";
        bool stopflag = false;
        int panPosition = -5;
        int tiltPosition = 0;
        int headingIncrement = 1;
};





#endif//AIS2203_PROJECT_SPHERO_AUTONOMOUS_HPP
