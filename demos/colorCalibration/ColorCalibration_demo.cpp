#include "sphero/cameras/USBCamera.hpp"
#include "sphero/vision/ColorCalibrator.hpp"
#include "sphero/utils/JsonUtils.hpp"

int main() {
    USBCamera camera(0);
    ColorCalibrator calibrator;
    camera.addObserver(&calibrator);

    camera.start();

    cv::Mat frame;

    // Wait for first frame (consider condition variable)
    while(!camera.getFrame(frame));

    while (true) {
        // To show the frame on main thread
        frame = calibrator.getNewestFrame();
        calibrator.blurFrame(frame);
        cv::imshow("Color Calibration", frame);

        char key = static_cast<char>(cv::waitKey(1));
        if (key == 's') {
            saveJson("saved_color_range.json", calibrator.getColorValues());
            std::cout << "Saved color range" << std::endl;

        }
        else if (key == 'q' || key == 27) {
            break;
        }
    }

    camera.stop();
    camera.removeObserver(&calibrator);
    return 0;
}
