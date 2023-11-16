#include "sphero/cameras/USBCamera.hpp"
#include <opencv2/highgui.hpp>

int main() {
    USBCamera camera(0); // Initialize the USB camera with the device index
    camera.start();

    cv::namedWindow("Camera Feed", cv::WINDOW_AUTOSIZE);

    while (cv::waitKey(1) != 'q') {
        cv::Mat frame;
        if (camera.getFrame(frame)) {
            cv::imshow("Camera Feed", frame);
        }
    }

    camera.stop();
    return 0;
}
