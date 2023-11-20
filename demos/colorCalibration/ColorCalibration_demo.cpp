#include "sphero/cameras/USBCamera.hpp"
#include "sphero/vision/ColorCalibrator.hpp"
#include "sphero/utils/JsonUtils.hpp"

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
        if (key == 's') {
            saveJson("saved_color_range.json", calibrator.getColorValues());
            std::cout << "Saved color range" << std::endl;

        } else if (key == 'q' || key == 27) {
            break;
        }
    }

    camera.stop();
    return 0;
}
