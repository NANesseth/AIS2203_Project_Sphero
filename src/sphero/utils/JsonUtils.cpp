#include "sphero/utils/JsonUtils.hpp"

#include <fstream>


void saveJson(const std::string& filename, const nlohmann::json& json){

    std::ofstream file;
    file.open(filename);
    file << json;
    file.close();
}

nlohmann::json loadJson(const std::string& filename){
    // TODO: Exception handling (file not found, etc.)
    std::ifstream file;
    file.open(filename);
    nlohmann::json json;
    file >> json;
    file.close();

    return json;
}

// Color Values
// =====================================================================================================================

nlohmann::json toJson(const ColorValues& colorValues){

    nlohmann::json json;
    json["min_R"] = colorValues.min_R;
    json["max_R"] = colorValues.max_R;
    json["min_G"] = colorValues.min_G;
    json["max_G"] = colorValues.max_G;
    json["min_B"] = colorValues.min_B;
    json["max_B"] = colorValues.max_B;

    return json;
}


ColorValues fromJson(const nlohmann::json& json){

    ColorValues colorValues;
    colorValues.min_R = json["min_R"];
    colorValues.max_R = json["max_R"];
    colorValues.min_G = json["min_G"];
    colorValues.max_G = json["max_G"];
    colorValues.min_B = json["min_B"];
    colorValues.max_B = json["max_B"];

    return colorValues;
}