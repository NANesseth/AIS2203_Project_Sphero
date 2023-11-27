#include "sphero/utils/JsonUtils.hpp"

#include <fstream>


void saveJson(const std::string& filename, const nlohmann::json& json){

    std::ofstream file;
    file.open(filename);
    file << json.dump(4);
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

nlohmann::json toJson(const ColorValues& colorValues) {
    nlohmann::json json;
    json["H_min"] = colorValues.H_min;
    json["H_max"] = colorValues.H_max;
    json["S_min"] = colorValues.S_min;
    json["S_max"] = colorValues.S_max;
    json["V_min"] = colorValues.V_min;
    json["V_max"] = colorValues.V_max;

    return json;
}



ColorValues fromJson(const nlohmann::json& json) {
    ColorValues colorValues;
    colorValues.H_min = json["H_min"];
    colorValues.H_max = json["H_max"];
    colorValues.S_min = json["S_min"];
    colorValues.S_max = json["S_max"];
    colorValues.V_min = json["V_min"];
    colorValues.V_max = json["V_max"];

    return colorValues;
}


