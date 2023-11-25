#ifndef AIS2203_PROJECT_SPHERO_JSON_READER_HPP
#define AIS2203_PROJECT_SPHERO_JSON_READER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "Various.hpp"

class JsonReader{
private:
    std::string jsonDump;
    std::string frame_s;
    std::string battery_level;
    nlohmann::json jsonFile;
    cv::Mat frame;
    bool videoRunning;

public:
    void updateJson(const std::string& jsonData) {
        try {
            jsonFile = nlohmann::json::parse(jsonData);
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON parsing error: " << e.what() << '\n';
            return;
        }

        if (jsonFile.contains("frame")) {
            frame_s = jsonFile["frame"].get<std::string>();
            frame = various::convertStringToFrame(frame_s);
        }
        if (jsonFile.contains("battery_level")) {
            battery_level = jsonFile["battery_level"].get<std::string>();
        }
        if (jsonFile.contains("videoRunning")) {
            videoRunning = jsonFile["videoRunning"].get<bool>();
        }
    }

    cv::Mat getFrame() const {
      return frame_s;
    }

    std::string getBatteryLevel() const {
      return battery_level;
}
    bool isVideoRunning() const {
          return videoRunning;
    }


};
#endif//AIS2203_PROJECT_SPHERO_JSON_READER_HPP
