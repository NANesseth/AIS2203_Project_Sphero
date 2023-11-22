#ifndef AIS2203_PROJECT_SPHERO_JSON_READER_HPP
#define AIS2203_PROJECT_SPHERO_JSON_READER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class JsonReader{
private:
    std::string jsonDump;
    std::string frame_s;
    std::string battery_level;
    nlohmann::json jsonFile;
public:


      void updateJson(std::string& jsonData) {
          jsonFile = nlohmann::json::parse(jsonData);

          if (jsonFile.contains("frame")) {
              frame_s = jsonFile["frame"].get<std::string>();
          }
          if (jsonFile.contains("battery_level")) {
              frame_s = jsonFile["frame"].get<std::string>();
          }
      }
      std::string getFrame() const {
          return frame_s;
      }

      std::string getBatteryLevel() const {
          return battery_level;
      }



};
#endif//AIS2203_PROJECT_SPHERO_JSON_READER_HPP
