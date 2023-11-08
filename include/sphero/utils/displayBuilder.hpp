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
        addText("F1 for how to to something", cv::Point(5, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("l1 for plapla", cv::Point(5, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        text_y=0;
        try {
            cv::imshow(windowName, frame);
            cv::waitKey(1);
        } catch (const cv::Exception& e) {
            std::cerr << "OpenCV error occurred: " << e.what() << std::endl;
        }
    }

    void destroyWindow(){
        cv::destroyWindow(this->windowName);
    }

    void buildKeyboardMenu() {
        this -> windowName = "Keyboard Menu";
        // Reset y position for each menu build
        try {
            cv::imshow(windowName, frame);

        } catch (const cv::Exception& e) {
            std::cerr << "OpenCV error occurred: " << e.what() << std::endl;
        }
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
        addText("ESC - exit", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        text_y = 0;
        cv::waitKey(1);

    }

    void buildMainMenu(){
        this-> windowName = "Main Menu";
        frame = blackFrame;
        addText("1 for keyboard controls", cv::Point(5, this->text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        addText("2 for Xbox controls", cv::Point(5, text_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        text_y=0;
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
    cv::Mat blackFrame = cv::Mat::zeros(100,100,CV_8UC3);
    cv::Mat frame = cv::Mat::zeros(100,100,CV_8UC3);
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
