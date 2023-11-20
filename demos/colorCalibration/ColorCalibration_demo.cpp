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
            // Save the color range to a file when 's' is pressed
            //calibrator.saveColorRange("saved_color_range.json");

            saveJson("saved_color_range.json", calibrator.getMinColor(), calibrator.getMaxColor());


        } else if (key == 'q' || key == 27) {
            break;
        }
    }

    camera.stop();
    return 0;
}
