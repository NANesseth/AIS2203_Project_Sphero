#include "sphero/cameras/PCCamera.hpp"
#include "sphero/utils/JsonUtils.hpp"
#include "sphero/vision/ColorCalibrator.hpp"
#include "sphero/vision/LiveFrameDisplay.hpp"

int main() {
    PCCamera camera;

    LiveFrameDisplay gui;
    camera.addObserver(&gui);

    ColorCalibrator calibrator;
    camera.addObserver(&calibrator);

    camera.start();

    cv::Mat frame, mask, result;

    // Wait for first frame (consider condition variable)
    while(!camera.getFrame(frame));
    while(gui.isNewestFrameEmpty());

    calibrator.start();
    while(calibrator.isNewestFrameEmpty());

    while (true) {
        gui.getNewestFrame(frame);
        calibrator.getNewestMask(mask);

        try{

            result = cv::Mat::zeros(frame.size(), frame.type());
            cv::bitwise_and(frame, frame, result, mask);
            cv::imshow("Color Calibration", result);
        }
        catch(std::exception &e){
            std::cout << "Error: " << e.what() << std::endl;
            cv::imshow("Color Calibration", frame);
        }

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
