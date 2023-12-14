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
    bool videoRunning;
    cv::Mat frame;
    std::string part;

    int part_number;
    int total_parts;
    std::vector<std::string> frame_parts;

    void parseJsonData(const std::string& jsonData) {
        try {
            jsonFile = nlohmann::json::parse(jsonData);
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON parsing error: " << e.what() << '\n';
        }
    }

    void updateBatteryLevel() {
        if (jsonFile.contains("battery_level")) {
            battery_level = jsonFile["battery_level"].get<std::string>();
        }
    }

    void updateVideoRunning() {
        if (jsonFile.contains("videoRunning")) {
            videoRunning = jsonFile["videoRunning"].get<bool>();
        }
    }
    void getDistance(){
        if (jsonFile.contains("distance")){
            std::cout << jsonFile["distance"].get<std::string>() << std::endl;
        }
    }

    void updateFrameParts() {
        if (jsonFile.contains("frame_part") && jsonFile.contains("part_number") && jsonFile.contains("total_parts")) {
            updateFrameAndParts();
        }
    }

    void updateFrameAndParts() {
        part = jsonFile["frame_part"].get<std::string>();
        part_number = jsonFile["part_number"].get<int>();
        total_parts = jsonFile["total_parts"].get<int>();

        resizeFrameParts();
        updatePartNumber();

        if (checkAllPartsReceived()) {
            combinePartsToFrame();
            clearFrameParts();
        }
    }

    void resizeFrameParts() {
        if (total_parts != static_cast<int>(frame_parts.size())){
            frame_parts.resize(total_parts);
        }
    }

    void updatePartNumber() {
        frame_parts[part_number] = part;
    }

    bool checkAllPartsReceived() {
        for (auto& part_: frame_parts) {
            if (part_.empty()) {
                return false;
            }
        }
        return true;
    }

    void combinePartsToFrame() {
        frame_s.clear();
        for (auto& part_: frame_parts) {
            frame_s += part_;
        }
        frame = various::convertStringToFrame(frame_s);
    }

    void clearFrameParts() {
        frame_parts.clear();
    }


public:

    void updateJson(const std::string& jsonData) {
        parseJsonData(jsonData);
        updateBatteryLevel();
        updateVideoRunning();
        updateFrameParts();
    }
    std::string getBatteryLevel() const {
        return battery_level;
    }
    cv::Mat getFrame() const{
        return frame;
    }
    bool getVideoRunning() const{
        return videoRunning;
    }



};
#endif//AIS2203_PROJECT_SPHERO_JSON_READER_HPP
