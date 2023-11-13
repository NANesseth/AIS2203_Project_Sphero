//
// Created by Torst on 06/11/2023.
//

#ifndef AIS2203_PROJECT_SPHERO_DISPLAYBUILDER_HPP
#define AIS2203_PROJECT_SPHERO_DISPLAYBUILDER_HPP
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

class DisplayBuilder{
public:
    // Constructor with an optional window name
    DisplayBuilder(const std::string& name = "Display Window") : windowName(name){

    }

    // Destructor
    ~DisplayBuilder() {
        cv::destroyWindow(windowName);
    }

    void buildXboxMenu() {
        this -> windowName = "Xbox Controller Menu";
        frame = blackFrame;


        addText("RT - Drive forward", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("Lt - Drive Backwards", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("Left joystick for steering", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("Right joystick for camera control", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("A - Video ON", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("B - Video OFF", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("ESC - exit to Main Menu", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));

        text_y=15;
        try {
            cv::imshow(windowName, frame);
            cv::waitKey(1);
        } catch (const cv::Exception& e) {
            std::cerr << "OpenCV error occurred: " << e.what() << std::endl;
        }
    }

    void destroyWindow(){
        blackFrame = cv::Mat::zeros(windowWidth,windowHeight,CV_8UC3);
        frame = cv::Mat::zeros(windowWidth,windowHeight,CV_8UC3);
        cv::imshow(windowName, blackFrame);
        cv::waitKey(1);
        cv::destroyWindow(this->windowName);
    }

    void buildKeyboardMenu() {
        this -> windowName = "Keyboard Menu";
        frame = blackFrame;

        addText("w - forward", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("s - reverse", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("a - left", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("d - right", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("p - increase speed", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("m - decrease speed", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("space - stop", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("v - start video", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("c - stop video", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("i - Control camera UP", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("k - Control camera DOWN", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("j - Control camera LEFT", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("l - Control camera RIGHT", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("ESC - exit to Main Menu", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        text_y = 15;
        try {
            cv::imshow(windowName, frame);
            cv::waitKey(1);
        } catch (const cv::Exception& e) {
            std::cerr << "OpenCV error occurred: " << e.what() << std::endl;
        }
    }

    void buildMainMenu(){
        this-> windowName = "Main Menu";
        frame = blackFrame;

        addText("1 for keyboard controls", cv::Point(5, this->text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("2 for Xbox controls", cv::Point(5, this->text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        text_y=15;
        try {
            cv::imshow(windowName, frame);
            cv::waitKey(1);
        } catch (const cv::Exception& e) {
            std::cerr << "OpenCV error occurred: " << e.what() << std::endl;
        }
    }

private:
    const int lineDistance = 15;
    int text_y = lineDistance;
    int windowWidth = 500;
    int windowHeight = 500;
    cv::Mat blackFrame = cv::Mat::zeros(windowWidth,windowHeight,CV_8UC3);
    cv::Mat frame = cv::Mat::zeros(windowWidth,windowHeight,CV_8UC3);
    std::string windowName;

    // Method to add text to the frame
    void addText(const std::string& text, const cv::Point& org, int fontFace, double fontScale, const cv::Scalar& color, int thickness = 1, int lineType = cv::LINE_8) {
        if (!frame.empty()) {
            cv::putText(frame, text, org, fontFace, fontScale, color, thickness, lineType);
            text_y += lineDistance;
        }
    }
};

#endif//AIS2203_PROJECT_SPHERO_DISPLAYBUILDER_HPP
