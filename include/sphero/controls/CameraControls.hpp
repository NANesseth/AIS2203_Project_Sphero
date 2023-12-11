#ifndef AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP
#define AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP
#include <iostream>

//test
class CameraControl {
private:
    int panPosition = 0; // -5 for calibration
    int tiltPosition = 0;

public:
    CameraControl() : panPosition(-5), tiltPosition(0) {}

    void setPanPosition(int pan) {
        pan = (pan > 90 ? 90 : (pan < -90 ? -90 : pan));
//        if (pan > -85){ // to make it start straight and not go too far left.
//            pan -= +5;
//        }
        panPosition = pan;
    }

    int getPanPosition() const {
        return -panPosition;
    }

    void setTiltPosition(int tilt) {
        tilt = (tilt > 90 ? 90 : (tilt < -90 ? -90 : tilt));

        tiltPosition = tilt;
    }

    int getTiltPosition() const {
        return -tiltPosition;
    }


};

#endif //AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP