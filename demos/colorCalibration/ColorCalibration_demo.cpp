#include "sphero/vision/ColorCalibration.hpp"
#include "sphero/cameras/USBCamera.hpp"

int main() {
    USBCamera camera(0);
    ColorCalibrator calibrator;

    camera.start();

    cv::Mat frame;
    while (true) {
        if (camera.getFrame(frame)) {
            calibrator.processFrame(frame);
        }

        char key = static_cast<char>(cv::waitKey(1));
        if (key == 'q' || key == 27) {
            break;
        }
    }

    camera.stop();
    return 0;
}
