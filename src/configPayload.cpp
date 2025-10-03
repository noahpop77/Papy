#include <iostream>
#include <fstream>
#include <regex>
// project dependencies
#include "dependencies/json.hpp"

#include "mapping.hpp"
#include "myRandom.hpp"
#include "configPayload.hpp"


using json = nlohmann::json;
json configPayload::basePayload;

json configPayload::readConfig() {
    
    loadPayload();

    json parsedConfig = basePayload;
    replacePlaceholders(parsedConfig);
    
    return parsedConfig;
}

// Static helper function to get a random value from a given JSON file
void configPayload::replacePlaceholders(nlohmann::json& jsonObject) {

    static const std::regex numberRegex(R"(\{NUMBER-(\d+)\})");
    static const std::regex stringRegex(R"(\{STRING-(\d+)\})");

    if (jsonObject.is_object()) {
        for (auto& [key, value] : jsonObject.items()) {
            replacePlaceholders(value);
        }
    } else if (jsonObject.is_array()) {
        for (auto& item : jsonObject) {
            replacePlaceholders(item);
        }
    } else if (jsonObject.is_string()) {
        std::string val = jsonObject.get<std::string>();
        std::smatch match;

        if (std::regex_match(val, match, numberRegex)) {
            int length = std::stoi(match[1].str());
            jsonObject = myRandom::generateRandomNumberString(length);
        } else if (std::regex_match(val, match, stringRegex)) {
            int length = std::stoi(match[1].str());
            jsonObject = myRandom::generateRandomString(length);
        }
    }

}

void configPayload::loadPayload() {
    if (!basePayload.is_null()) return;

    std::ifstream file("payload.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open payload.json" << std::endl;
        return;
    }

    try {
        file >> basePayload;
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
    }
}