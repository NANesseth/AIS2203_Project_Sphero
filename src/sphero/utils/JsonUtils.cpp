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

nlohmann::json toJson(const ColorValues& colorValues){

    nlohmann::json json;
    json["R_min"] = colorValues.R_min;
    json["R_max"] = colorValues.R_max;
    json["G_min"] = colorValues.G_min;
    json["G_max"] = colorValues.G_max;
    json["B_min"] = colorValues.B_min;
    json["B_max"] = colorValues.B_max;

    return json;
}


ColorValues fromJson(const nlohmann::json& json){

    ColorValues colorValues;
    colorValues.R_min = json["R_min"];
    colorValues.R_max = json["R_max"];
    colorValues.G_min = json["G_min"];
    colorValues.G_max = json["G_max"];
    colorValues.B_min = json["B_min"];
    colorValues.B_max = json["B_max"];

    return colorValues;
}

