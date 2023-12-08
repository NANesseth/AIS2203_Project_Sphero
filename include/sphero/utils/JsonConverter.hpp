#ifndef AIS2203_PROJECT_SPHERO_JSONCONVERTER_HPP
#define AIS2203_PROJECT_SPHERO_JSONCONVERTER_HPP

#include "nlohmann/json.hpp"
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>


// =================================================================================================
// [!!!]        This abstract class is not in use, and has been replaced
// [!!!]        with a simpler approach using structs and free functions
// =================================================================================================



// Parent class for all json converters
// =================================================================================================

template<typename T>
class JsonConverter {
public:
    virtual T read(const nlohmann::json& json) = 0;
    virtual std::string write(const T& data) = 0;

    void save(const std::string& filename, const nlohmann::json& json){
        std::ofstream file(filename);

        if(!file.is_open()){
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
        }

        // TODO: Handle error in writing (throw exception or something)
        file << json.dump(4);
        file.close(); // Will close anyway when 'file' goes out of scope
        std::cout << "Data saved to " << filename << std::endl;
    }

    void save(const std::string& filename, const T& data){

        save(filename, write(data));
    }

    T load(const std::string& filename){
        std::ifstream file(filename);

        if (!file.is_open()){
            std::cerr << "Unable to open file for reading: " << filename << std::endl;
            // Handle error (throw exception or return a default T object)
        }

        nlohmann::json json;
        try {
            file >> json;
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            // TODO: Handle error in parsing (throw exception or return a default T object)
        }
        return read(json);
    }
};


// Color Limits
// =================================================================================================

struct ColorLimits {
    int minRed = 0, maxRed = 255;
    int minGreen = 0, maxGreen = 255;
    int minBlue = 0, maxBlue = 255;
};

class ColorLimitsJsonConverter : public JsonConverter<ColorLimits> {
public:
    ColorLimits read(const nlohmann::json& json) override{
        //nlohmann::json j = nlohmann::json::parse(json);
        ColorLimits limits;
        limits.minRed = json["min_color"][0];
        limits.minGreen = json["min_color"][1];
        limits.minBlue = json["min_color"][2];
        limits.maxRed = json["max_color"][0];
        limits.maxGreen = json["max_color"][1];
        limits.maxBlue = json["max_color"][2];
        return limits;
    }
    std::string write(const ColorLimits& data) override;

    void save(const std::string& filename, const ColorLimits& data) override;
};

#endif//AIS2203_PROJECT_SPHERO_JSONCONVERTER_HPP