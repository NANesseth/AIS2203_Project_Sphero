#ifndef AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP
#define AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP
#include <iostream>

class CameraControl {
private:
    int panPosition = 0;
    int tiltPosition = 0;
    int calibratePan = -6;  //To calibrate the pan IRL

public:
    CameraControl() : panPosition(0), tiltPosition(0) {}

    void setPanPosition(int pan) {

        pan = (pan > 90 ? 90 : (pan < -90 ? -90 : pan));
        panPosition = pan - calibratePan;
    }

    int getPanPosition() const {
        return -panPosition;
    }

    void setTiltPosition(int tilt) {
        tilt = (tilt > 90 ? 90 : (tilt < -45 ? -45 : tilt));

        tiltPosition = tilt;
    }

    int getTiltPosition() const {
        return -tiltPosition;
    }


};

#endif //AIS2203_PROJECT_SPHERO_CAMERACONTROLS_HPP