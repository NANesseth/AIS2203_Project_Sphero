#include "sphero/cameras/USBCamera.hpp"
#include "sphero/vision/ObjectTracker.hpp"
#include <opencv2/highgui.hpp>

int main() {
    USBCamera camera(0);
    camera.start();

    cv::namedWindow("Camera Feed", cv::WINDOW_AUTOSIZE);

    while (cv::waitKey(1) != 'q') {
        cv::Mat frame;
        if (camera.getFrame(frame)) {
            cv::imshow("Camera Feed", frame);
        }
    }

    // In your main function or class where you handle the camera
    ObjectTracker tracker;
    std::thread trackingThread([&]() {
        cv::Mat frame;
        while (true) { // Replace with a proper condition to stop the thread
            if (camera.getFrame(frame)) {
                tracker.processFrame(frame);
            }
        }
    });

    // Remember to join this thread when your application exits


    camera.stop();
    return 0;
}
