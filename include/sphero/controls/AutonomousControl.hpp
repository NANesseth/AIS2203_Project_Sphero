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
            cv::Rect boundingRect;

            int width = frame.size().width;
            int height = frame.size().height;
            int centerX = width / 2;
            int centerY = height / 2;

            cv::Scalar lower(50, 100, 100); // Lower boundary for GREEN color
            cv::Scalar upper(70, 255, 255); // Upper boundary for GREEN color

            cv::Mat greenMask;
            cv::inRange(hsvImage, lower, upper, greenMask);

            // Find contours
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(greenMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);


            double maxArea = 0;
            cv::Point2f ballcenter;
            float radius = 0;
            for (const auto& contour : contours) {
                float area = cv::contourArea(contour);
                if (area > maxArea) {
                    maxArea = area;
                    cv::minEnclosingCircle(contour, ballcenter, radius);
                    boundingRect = cv::boundingRect(contour);
                }
            }

            if (maxArea > 10) {

                // Draw green box around object
                cv::rectangle(frame, boundingRect.tl(), boundingRect.br(), cv::Scalar(0, 255, 0));
                frame.copyTo(markedFrame);

                float ballSize = radius*2;
                int ballPositionX = int(ballcenter.x);
                int ballPositionY = int(ballcenter.y);
                speed = (70-ballSize);
                if(speed > maxSpeed){
                    msg = "drive";
                    speed = 15;
                }
                if(speed < -maxSpeed){
                    msg = "drive_reverse";
                    speed = 15;
                }
                if(speed < 0){
                    speed = speed*(-1);
                }


                updateControls(ballcenter.x, ballcenter.y, centerX, centerY, tiltPosition, panPosition);

                // Use a proportional constant to control the rate of change
                float proportionalConstant = 0.05f;
                headingIncrement = abs(panPosition)*proportionalConstant;

                std::cout << "Red ball found at: (" << ballPositionX << ", " << ballcenter.y << ") with size: " << ballSize << " pixels" << std::endl;
                if (panPosition < 0) {
                    heading = (heading + headingIncrement) % 360;
                }
                else if (panPosition > 0) {
                    heading = heading - headingIncrement;
                    if (heading < 0) heading += 360;
                }
            }
            else {
                std::cout << "No red ball found." << std::endl;
                scan();
                //heading = (heading + headingIncrement) % 360;
                speed = 0;
            }
        }


        void scan()
        {
            static int scanDirection = 1;

            if (panPosition > 70) {
                tiltPosition = 0;
                scanDirection = -1;
            } else if (panPosition < -70) {
                tiltPosition = 0;
                scanDirection = 1;
            }

            panPosition += scanDirection;

        }



        cv::Mat getFrameWithMarkedBall(){
            return markedFrame;
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
        cv::Mat markedFrame;

        float oldErrorX = 0;
        float oldErrorY = 0;
        float integralErrorX = 0;
        float integralErrorY = 0;


        void updateControls(float ballX, float ballY, float centerX, float centerY, int &cameraTilt, int &cameraPan) {
            std::cout<<"ballX: "<<ballX<<" ballY: "<<ballY<<" centerX: "<<centerX<<" centerY: "<<centerY<<std::endl;

            float Kp_tilt = 0.03f; // Proportional constant for tilt
            float Ki_tilt = 0.00f; // Integral constant for tilt
            float Kd_tilt = 0.2f; // Derivative constant for tilt

            float Kp_pan = 0.03f; // Proportional constant for pan 0.03
            float Ki_pan = 0.001f; // Integral constant for pan
            float Kd_pan = 0.09f; // Derivative constant for pan 0.0008

            float errorX = centerX - ballX; // Calculate error in X
            float errorY = centerY - ballY; // Calculate error in Y
            std::cout<<"errorX: "<<errorX<<" errorY: "<<errorY<<std::endl;

            if(std::abs(errorX) < 30 && std::abs(errorY) < 30) {
                return;
            }

            float dErrorX = errorX - oldErrorX; // Derivative of error for X
            float dErrorY = errorY - oldErrorY; // Derivative of error for Y

            integralErrorX += errorX; // Integral of error for X
            integralErrorY += errorY; // Integral of error for Y

            // PID control for tilt and pan
            cameraTilt -= round((Kp_tilt * errorY) + (Ki_tilt * integralErrorY) + (Kd_tilt * dErrorY));
            cameraPan  += round((Kp_pan * errorX) + (Ki_pan * integralErrorX) + (Kd_pan * dErrorX));

            oldErrorX = errorX; //Save current error for next calculation
            oldErrorY = errorY; //Save current error for next calculation

            std::cout<<"tilt: "<<cameraTilt<<" pan: "<<cameraPan<<std::endl;

            // Clamp values to range
            if(cameraTilt > 90) {
                cameraTilt = 90;
            } else if(cameraTilt < -90) {
                cameraTilt = -90;
            }

            if(cameraPan > 90) {
                cameraPan = 90;
            } else if(cameraPan < -90) {
                cameraPan = -90;
            }
        }
};





#endif//AIS2203_PROJECT_SPHERO_AUTONOMOUS_HPP
