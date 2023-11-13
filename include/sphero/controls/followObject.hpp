//
// Created by Torst on 13/11/2023.
//

#ifndef AIS2203_PROJECT_SPHERO_FOLLOWOBJECT_HPP
#define AIS2203_PROJECT_SPHERO_FOLLOWOBJECT_HPP
//make autonomous something here.
//want to make color video first
#include <opencv2/opencv.hpp>

class FollowObject{
public:
    void processFrame(cv::Mat& frame){

    }

private:
    int xPos;
    int yPos;
    int colorLow[3] = {40,40,40};
    int colorHigh[3] = {80,255,255};

};

#endif //AIS2203_PROJECT_SPHERO_FOLLOWOBJECT_HPP
