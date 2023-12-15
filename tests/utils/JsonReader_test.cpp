//
// Created by Torstein on 11/12/2023.
//
#include "sphero/utils/Json reader.hpp"
#include "catch2/catch_test_macros.hpp"
TEST_CASE("Test updateJson") {
    JsonReader jsonReader;
    cv::Mat testFrame = cv::Mat::zeros(100, 100, CV_8UC3);
    std::string frame_as_string;

    nlohmann::json jsonData = {
            {"battery_level", "58%"},
            {"frame_part", frame_as_string},
            {"part_number", 0},
            {"total_parts", 1},
            {"videoRunning", true}
    };
    std::string data = jsonData.dump();
    jsonReader.updateJson(data);
    SECTION("Test updateBatteryLevel") {
        REQUIRE(jsonReader.getBatteryLevel() == "58%");
    }
    SECTION("Test updateVideoRunning") {
        REQUIRE(jsonReader.getVideoRunning() == true);
    }

}
