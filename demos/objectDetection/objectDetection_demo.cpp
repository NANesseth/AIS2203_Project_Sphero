#include "sphero/cameras/USBCamera.hpp"
#include "sphero/vision/ColorCalibrator.hpp" // remove
#include "sphero/utils/JsonUtils.hpp"

class GUI_test : public Observer {
public:
    void onFrameAvailable(const cv::Mat& frame) override {
        updateNewestFrame(frame);
    }

    void updateNewestFrame(const cv::Mat& frame) {
        std::lock_guard<std::mutex> lock(latestFrame_mutex_);
        frame.copyTo(latestFrame_);
    }

    void showFrame(const cv::Mat& frame) {
        cv::imshow("Object Detection", frame);
    }

    bool getNewestFrame(cv::Mat& frame){
        std::lock_guard<std::mutex> lock(latestFrame_mutex_);
        if (latestFrame_.empty()) {
            return false;
        }
        latestFrame_.copyTo(frame);
        return true;
    }

private:
    std::mutex latestFrame_mutex_;
    cv::Mat latestFrame_;
};


int main() {
    USBCamera camera(0);

    GUI_test gui;
    camera.addObserver(&gui);

    camera.start();

    cv::Mat frame;

    // Wait for first frame (consider condition variable)
    while(!camera.getFrame(frame));
    std::cout << "ImageFetcher: Got first frame" << std::endl;

    while(!gui.getNewestFrame(frame));
    std::cout << "Observer: Got first frame" << std::endl;

    while (true) { // or until some condition is met
        if (gui.getNewestFrame(frame)) {
            cv::imshow("Object Detection", frame);
            //gui.showFrame(frame);
            if (cv::waitKey(1) >= 0) break; // Allow the window to refresh, and break if user hits a key
        }

        char key = static_cast<char>(cv::waitKey(1));
        if (key == 'q' || key == 27) {
            break;
        }
    }

    camera.stop();
    camera.removeObserver(&gui);
    return 0;
}