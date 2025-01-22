#pragma once

#include <random>
#include <string>

#include "keyMapping.hpp"

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

    static bool getRandomVectorFromJSON(std::vector<std::string>& participantData, keyMapping::MappingTemplate mappingTemplate, size_t count);

private:
    static thread_local std::random_device rd;  // Seed source
    static thread_local std::mt19937 gen;
};
