#ifndef AIS2203_PROJECT_SPHERO_RASPBERRYCAMERA_HPP
#define AIS2203_PROJECT_SPHERO_RASPBERRYCAMERA_HPP

#include "sphero/core/ImageFetcher.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include "sphero/utils/udpClient.hpp"
#include "sphero/controls/keyboardInput.hpp"
#include "opencv2/opencv.hpp"

// Not implemented yet

class RaspberryCamera : public ImageFetcher {

public:
    explicit RaspberryCamera() = default;

protected:

    void captureLoop() override {

        while(!stop_){
            // notify observers on new frame
            cv::Mat frame;
            if (getFrame(frame)) {
                notifyObservers();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }


};

#endif //AIS2203_PROJECT_SPHERO_RASPBERRYCAMERA_HPP