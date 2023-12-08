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
            cv::Scalar lowerGreen(50, 100, 100);
            cv::Scalar upperGreen(70, 255, 255);


            cv::Mat greenMask;
            cv::inRange(hsvImage, lowerGreen, upperGreen, greenMask);

            // Find contours
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(greenMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);


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

            if (maxArea > 10) {
                float ballSize = radius*2;
                float ballPosition = center.x;
                speed = int(70-ballSize);
                if(speed > maxSpeed){
                    msg = "drive";
                    speed = 70;
                }
                if(speed < -maxSpeed){
                    msg = "drive_reverse";
                    speed = -70;
                }
                if(speed < 0){
                    speed = speed*(-1);
                }

                std::cout << "Green ball found at: (" << ballPosition << ", " << center.y << ") with size: " << ballSize << " pixels" << std::endl;
                if (ballPosition > 132.5) {
                    heading = (heading + headingIncrement) % 360;
                }
                else if (ballPosition < 132.5) {
                    heading = (heading - headingIncrement) % 360;
                    if (heading < 0) heading += 360;
                }
            }
            else {
                std::cout << "No green ball found." << std::endl;
                //heading = (heading + headingIncrement) % 360;
                speed = 0;
            }
        }






        cv::Mat getFrameWithMarkedBall(){
            return cv::Mat();
        }

    private:

        std::tuple<int,int> ballCoordinates;
        int maxSpeed = 100;
        int speed = 0;
        int heading = 0;
        std::string msg= "video_start";
        bool stopflag = false;
        int panPosition = -5;
        int tiltPosition = 0;
        int headingIncrement = 1;
};





#endif//AIS2203_PROJECT_SPHERO_AUTONOMOUS_HPP
