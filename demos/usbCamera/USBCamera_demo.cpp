#include "sphero/sphero.hpp"
#include <opencv2/highgui.hpp>

int main() {
    USBCamera camera; // Initialize the USB camera. Assumes camera at index 0.
    camera.start();   // Start capturing

    cv::namedWindow("Camera Feed", cv::WINDOW_AUTOSIZE); // Create a window

    while (true) {
        cv::Mat frame;
        if (camera.getFrame(frame)) { // Try to get a new frame from the camera
            cv::imshow("Camera Feed", frame); // Display the frame
        }

        // Check for 'q' key to quit the loop
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    camera.stop(); // Stop capturing before exiting
    return 0;
}
