#include "sphero/cameras/PCCamera.hpp"
#include "sphero/vision/ColorCalibrator.hpp" // remove
#include "sphero/utils/JsonUtils.hpp"
#include "sphero/vision/BallTracker.hpp"
#include "sphero/vision/LiveFrameDisplay.hpp"

// It is recommended to run the ColorCalibration_demo first to get the color range of the ball.
// If not, the default color range will be used.

int main() {
    PCCamera camera;

    LiveFrameDisplay gui;
    camera.addObserver(&gui);

    BallTracker tracker;
    camera.addObserver(&tracker);

    camera.start();

    // Move this to the constructor of BallTracker
    try{
        tracker.setColor(fromJson(loadJson("saved_color_range.json")));
    }
    catch (std::exception& e){
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "Using default color range, consider running color calibration first" << std::endl;
        tracker.setColor(ColorValues{67, 239, 83, 196, 21, 84}); // weak green
    }

    cv::Mat frame;

    // Wait for first frame (consider condition variable)
    while(!camera.getFrame(frame));
    std::cout << "ImageFetcher: Got first frame" << std::endl;

    while(gui.isNewestFrameEmpty());
    std::cout << "Observer: Got first frame" << std::endl;

    tracker.start();
    BallTrackerResult ball;

    // Merge (main)
    //cv::Point2f screenCenter(frame.cols / 2, frame.rows / 2);
    //cv::Point2f relativePosition;

    while (true) {
        gui.getNewestFrame(frame);
        ball = tracker.getResult(); // Make sure how the tracker gets the frame

        if (ball.found){
            // Draw a circle at 'result.center' with a radius of 'result.radius'
            cv::circle(frame, ball.center, ball.radius, cv::Scalar(0, 250, 0), 2);

            // Merge (main)
            //relativePosition = ball.center - screenCenter;
            //relativePosition = tracker.getRelativePosition(frame.cols, frame.rows);
            //std::cout << "Relative position: " << relativePosition << std::endl;
        }
        cv::imshow("Object Detection", frame);

        char key = static_cast<char>(cv::waitKey(1));
        if (key == 'q' || key == 27) {
            break;
        }
    }

    tracker.stop();

    camera.stop();
    camera.removeObserver(&gui);

    return 0;
}