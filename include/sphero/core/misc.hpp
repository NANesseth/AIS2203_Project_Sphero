#ifndef AIS2203_PROJECT_SPHERO_MISC_HPP
#define AIS2203_PROJECT_SPHERO_MISC_HPP

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <string>

struct ColorValues {

    int H_min = 0;   // Hue minimum
    int S_min = 0;   // Saturation minimum
    int V_min = 0;   // Value minimum

    int H_max = 180; // Hue maximum (in OpenCV, Hue range is 0-180)
    int S_max = 255; // Saturation maximum
    int V_max = 255; // Value maximum

};


struct BallTrackerResult {
    cv::Point2f center{};
    float radius{};
    bool found{};
};


// Not fully implemented, but planned to be used for translating the ball position to robot control values
struct RobotControlValues {
    int heading = 0;
    int speed = 10;
    std::string msg = "start_video";
    int panPosition = 0;
    int tiltPosition = 0;

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


    void setObjectHeading(BallTrackerResult ball, cv::Point2f screenCenter){
        if (ball.found) {
            // Calculate the heading, 0 degrees is straight ahead
            // positive is to the right, negative to the left

            heading = static_cast<int>(std::round((ball.center.x - screenCenter.x) / screenCenter.x * 45));
        } else {
            heading = 0;
        }
    }

    void setObjectSpeed(BallTrackerResult ball, cv::Point2f screenCenter){
        if (ball.found) {
            // Calculate the speed, 0 is stopped, 255 is full speed
            // The larger the ball appears, the slower the robot moves
            // use the ball radius, compare it to the total size of the screen

            speed = static_cast<int>(std::round((screenCenter.x - ball.radius) / screenCenter.x * 100));

        } else {
            speed = 0;
        }
    }
};



#endif//AIS2203_PROJECT_SPHERO_MISC_HPP