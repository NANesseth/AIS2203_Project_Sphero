#ifndef AIS2203_PROJECT_SPHERO_JSONUTILS_HPP
#define AIS2203_PROJECT_SPHERO_JSONUTILS_HPP

#include <nlohmann/json.hpp>
#include <string>


void saveJson(const std::string& filename, const nlohmann::json& json);
nlohmann::json loadJson(const std::string& filename);

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