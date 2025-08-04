#pragma once

#include <string>

// project dependencies
#include "dependencies/json.hpp"

class configPayload {
public:
    // Function to generate a random match based on the template
    static nlohmann::json readConfig();

private:
    static nlohmann::json basePayload;
    static void loadPayload();
    // Internal function to get a random key from a JSON file
    static void replacePlaceholders(nlohmann::json& jsonObject);
};
