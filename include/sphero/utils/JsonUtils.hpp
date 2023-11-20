#ifndef AIS2203_PROJECT_SPHERO_JSONUTILS_HPP
#define AIS2203_PROJECT_SPHERO_JSONUTILS_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <iostream>


nlohmann::json loadJson(const std::string& filename);

void saveJson(const std::string& filename, const nlohmann::json& json);

template<typename T>
void saveJson(const std::string& filename, const T& data) {
    nlohmann::json json = toJson(data); // Convert data to JSON
    saveJson(filename, json); // Use the existing saveJson function
}

// Color Values
// =====================================================================================================================

struct ColorValues {
    int min_R, max_R;
    int min_G, max_G;
    int min_B, max_B;
};

nlohmann::json toJson(const ColorValues& colorValues);
ColorValues fromJson(const nlohmann::json& json);



#endif//AIS2203_PROJECT_SPHERO_JSONUTILS_HPP