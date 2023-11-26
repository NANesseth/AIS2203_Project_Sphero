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


public:
    void updateJson(const std::string& jsonData) {
        try {
            jsonFile = nlohmann::json::parse(jsonData);
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON parsing error: " << e.what() << '\n';
            return;
        }

        if (jsonFile.contains("battery_level")) {
            battery_level = jsonFile["battery_level"].get<std::string>();
        }
        if (jsonFile.contains("videoRunning")) {
            videoRunning = jsonFile["videoRunning"].get<bool>();
        }
        if (jsonFile.contains("frame_part") && jsonFile.contains("part_number") && jsonFile.contains("total_parts")) {
                part = jsonFile["frame_part"].get<std::string>();
                part_number = jsonFile["part_number"].get<int>();
                total_parts = jsonFile["total_parts"].get<int>();
                if (total_parts != static_cast<int>(frame_parts.size())){
                    frame_parts.resize(total_parts);
                }

                bool all_parts_received = true;
                frame_parts[part_number] = part;

                for (auto& part_: frame_parts) {
                    if (part_.empty()) {
                        all_parts_received = false;
                        break;
                    }
                }
                if (all_parts_received) {
                    frame_s.clear();
                    for (auto& part_: frame_parts) {
                        frame_s += part_;
                    }
                    frame = various::convertStringToFrame(frame_s);
                    frame_parts.clear();
                }
            }
    }

    const cv::Mat getFrame() const {
      return frame;
    }

    const std::string getBatteryLevel() const {
        return battery_level;
    }

};
#endif//AIS2203_PROJECT_SPHERO_JSON_READER_HPP
