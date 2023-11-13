#ifndef AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP
#define AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP
#include <iostream>
#include <openCV2/opencv.hpp>

class CameraControl {
private:
    int panPosition = 0;
    int tiltPosition = 0;
    std::string msg = "empty";

public:
    CameraControl() : panPosition(0), tiltPosition(0) {}

    void setPanPosition(int pan) {
        panPosition = pan;
    }

    int getPanPosition() const {
        return panPosition;
    }

    void setTiltPosition(int tilt) {
        tiltPosition = tilt;
    }

    int getTiltPosition() const {
        return tiltPosition;
    }


};

#endif //AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP