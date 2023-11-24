#ifndef AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP
#define AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP
#include <iostream>
#include <openCV2/opencv.hpp>

class CameraControl {
private:
    int panPosition = 0;
    int tiltPosition = 0;

public:
    CameraControl() : panPosition(0), tiltPosition(0) {}

    void setPanPosition(int pan) {
        pan = (pan > 90 ? 90 : (pan < -90 ? -90 : pan));
        panPosition = pan;
    }

    int getPanPosition() const {
        return -panPosition;  // (-+ 7 for Calibration )Negative to revert inverted pan position
    }

    void setTiltPosition(int tilt) {
        tilt = (tilt > 90 ? 90 : (tilt < -90 ? -90 : tilt));
        tiltPosition = tilt;
    }

    int getTiltPosition() const {
        return -tiltPosition; // (+- 7) for calibration
    }


};

#endif //AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP