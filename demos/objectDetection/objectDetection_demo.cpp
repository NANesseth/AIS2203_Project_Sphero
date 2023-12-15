#include "sphero/cameras/USBCamera.hpp"
#include "sphero/utils/JsonUtils.hpp"
#include "sphero/vision/BallTracker.hpp"
#include "sphero/vision/ColorCalibrator.hpp"// remove

class GUI: public Observer {
public:
    void onFrameAvailable(const cv::Mat& frame) override {
        updateNewestFrame(frame);
    }

    void updateNewestFrame(const cv::Mat& frame) {
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        frame.copyTo(newestFrame_);
    }

    void getNewestFrame(cv::Mat& frame){
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        newestFrame_.copyTo(frame);
    }

    bool isNewestFrameEmpty() {
        std::lock_guard<std::mutex> lock(newestFrame_mutex_);
        return newestFrame_.empty();
    }

private:
    std::mutex newestFrame_mutex_;
    cv::Mat newestFrame_;
};


int main() {
    USBCamera camera;

    GUI gui;
    camera.addObserver(&gui);

    BallTracker tracker;
    camera.addObserver(&tracker);

    camera.start();

    // Initializing ball tracker
    try{
        tracker.setColor(fromJson(loadJson("saved_color_range3e.json")));
    }
    catch (std::exception& e){
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "Using default color range, consider running color calibration first" << std::endl;
        tracker.setColor(ColorValues{0, 10, 100, 255, 100, 255});//GREEN
    }

    cv::Mat frame;

    // Wait for first frame (consider condition variable)
    while(!camera.getFrame(frame));
    std::cout << "ImageFetcher: Got first frame" << std::endl;

    while(gui.isNewestFrameEmpty());
    std::cout << "Observer: Got first frame" << std::endl;

    tracker.startTracking();
    BallTrackerResult ball;

    cv::Point2f screenCenter(frame.cols / 2, frame.rows / 2);
    cv::Point2f relativePosition;

    while (true) {
        gui.getNewestFrame(frame);
        ball = tracker.getResult();

        if (ball.found){
            cv::circle(frame, ball.center, ball.radius, cv::Scalar(0, 250, 0), 2);
            //relativePosition = ball.center - screenCenter;
            relativePosition = tracker.getRelativePosition(frame.cols, frame.rows);
            std::cout << "Relative position: " << relativePosition << std::endl;
        }
        cv::imshow("Object Detection", frame);


        char key = static_cast<char>(cv::waitKey(1));
        if (key == 'q' || key == 27) {
            break;
        }
    }

    tracker.stopTracking();

    camera.stop();
    camera.removeObserver(&gui);

    return 0;
}