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
        bool selectController(enums::Controller& controller){
            using namespace enums;

            char key;
            const char ESC_KEY = 27;
            key = (char)cv::waitKey(10);
            switch (key) {
                case ESC_KEY:
                    controller = Controller::NOCONTROLLER;
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
            float size1 = 10;
            float size2 =20;
            float size3 = 50;
            float size4 =90;
            cv::Mat hsvImage;
            cv::cvtColor(frame, hsvImage, cv::COLOR_BGR2HSV);
            cv::Rect boundingRect;

            int width = frame.size().width;
            int height = frame.size().height;
            int centerX = width / 2;
            int centerY = height / 2;

            cv::Scalar lower(170, 120, 70); // Lower boundary for RED color
            cv::Scalar upper(180, 255, 255); // Upper boundary for RED color


            cv::Mat greenMask;
            cv::inRange(hsvImage, lower, upper, greenMask);

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

                float ballSize = radius*2;
                int ballPositionX = int(ballcenter.x);
                int ballPositionY = int(ballcenter.y);

                //fuzzy speedlogic
                if(ballSize < size1){
                    msg = "drive";
                    speed = 40;
                }
                else if(ballSize < size2 && ballSize < size3){
                    msg = "drive";
                    speed = 30;
                }
                else if(ballSize < size3 && ballSize < size4){
                    msg = "drive";
                    speed = 40;
                }
                else if(ballSize > size4){
                    msg = "drive_reverse";
                    speed = 20;
                }


                updateControls(ballcenter.x, ballcenter.y, centerX, centerY, tiltPosition, panPosition);

                // Use a proportional constant to control the rate of change
                float proportionalConstant = 0.05f;
                headingIncrement = abs(panPosition)*proportionalConstant;

                //std::cout << "Red ball found at: (" << ballPositionX << ", " << ballcenter.y << ") with size: " << ballSize << " pixels" << std::endl;
                if (panPosition < 0) {
                    heading = (heading + headingIncrement) % 360;
                }
                else if (panPosition > 0) {
                    heading = heading - headingIncrement;
                    if (heading < 0) heading += 360;
                }
            }
            else {
                //std::cout << "No red ball found." << std::endl;
                //scan();
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


    private:
        std::tuple<int,int> ballCoordinates;
        int speed = 0;
        int heading = 0;
        std::string msg;
        bool stopflag = false;
        int panPosition = -5;
        int tiltPosition = 0;
        int headingIncrement = 1;
        float oldErrorX = 0;
        float oldErrorY = 0;
        float integralErrorX = 0;
        float integralErrorY = 0;


        void updateControls(float ballX, float ballY, float centerX, float centerY, int &cameraTilt, int &cameraPan) {
            std::cout<<"ballX: "<<ballX<<" ballY: "<<ballY<<" centerX: "<<centerX<<" centerY: "<<centerY<<std::endl;
            float threshold = 5.0f;
            float Kp_tilt = 0.052f;
            float Ki_tilt = 0.00f;
            float Kd_tilt = 0.13f;

            float Kp_pan = 0.05f;
            float Ki_pan = 0.00f;
            float Kd_pan = 0.13f;

            float errorX = centerX - ballX;
            float errorY = centerY - ballY;

            if(std::abs(errorX) < threshold && std::abs(errorY) < threshold) {
                return;
            }

            float dErrorX = errorX - oldErrorX;
            float dErrorY = errorY - oldErrorY;

            integralErrorX += errorX;
            integralErrorY += errorY;

            // PID control for tilt and pan
            cameraTilt -= round((Kp_tilt * errorY) + (Ki_tilt * integralErrorY) + (Kd_tilt * dErrorY));
            cameraPan  += round((Kp_pan * errorX) + (Ki_pan * integralErrorX) + (Kd_pan * dErrorX));
            if (cameraTilt > 45) cameraTilt = 45;
            oldErrorX = errorX;
            oldErrorY = errorY;

            std::cout<<"tilt: "<<cameraTilt<<" pan: "<<cameraPan<<std::endl;

        }
};





#endif//AIS2203_PROJECT_SPHERO_AUTONOMOUS_HPP
