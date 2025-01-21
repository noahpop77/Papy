#pragma once

#include <random> 
#include <string>

#include "dependencies/json.hpp"

class myRandom {
public:

    // Generates a random integer between min and max (inclusive)
    static int generateRandomInt(int min, int max);

    // Generates a random string of a specified length containing letters and digits
    static std::string generateRandomString(size_t length);

    // Generates a random numeric string of a specified length
    static std::string generateRandomNumberString(size_t length);

    // Generates a random boolean (true or false)
    static bool getRandomBool();

    static bool getRandomVectorFromJSON(std::vector<std::string>& participantData,  const nlohmann::json& jsonObject, const int& count);
    
private:
    static std::random_device rd;  // Seed source
    static std::mt19937 gen;
};
