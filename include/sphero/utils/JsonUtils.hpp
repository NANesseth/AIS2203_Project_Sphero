#ifndef AIS2203_PROJECT_SPHERO_JSONUTILS_HPP
#define AIS2203_PROJECT_SPHERO_JSONUTILS_HPP

#include "sphero/core/misc.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <iostream>


nlohmann::json loadJson(const std::string& filename);

void saveJson(const std::string& filename, const nlohmann::json& json);

template<typename T>
void saveJson(const std::string& filename, const T& data) {
    nlohmann::json json = toJson(data);
    saveJson(filename, json);
}

// Color Values
// =====================================================================================================================

nlohmann::json toJson(const ColorValues& colorValues);
ColorValues fromJson(const nlohmann::json& json);



#endif//AIS2203_PROJECT_SPHERO_JSONUTILS_HPP