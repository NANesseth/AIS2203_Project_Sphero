#ifndef AIS2203_PROJECT_SPHERO_MISC_HPP
#define AIS2203_PROJECT_SPHERO_MISC_HPP

#include <opencv2/opencv.hpp>

struct ColorValues {

    int H_min = 0;   // Hue minimum
    int S_min = 0;   // Saturation minimum
    int V_min = 0;   // Value minimum

    int H_max = 180; // Hue maximum (in OpenCV, Hue range is 0-180)
    int S_max = 255; // Saturation maximum
    int V_max = 255; // Value maximum

};



#endif//AIS2203_PROJECT_SPHERO_MISC_HPP