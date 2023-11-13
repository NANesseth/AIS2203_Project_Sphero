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
        pan = (pan > 180 ? 180 : (pan < 0 ? 0 : pan));
        panPosition = pan;
    }

    int getPanPosition() const {
        return panPosition;
    }

    void setTiltPosition(int tilt) {
        tilt = (tilt > 180 ? 180 : (tilt < 0 ? 0 : tilt));
        tiltPosition = tilt;
    }

    int getTiltPosition() const {
        return tiltPosition;
    }


};

#endif //AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP