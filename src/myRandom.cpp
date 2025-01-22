#include "myRandom.hpp"

#include <iostream>
#include <random>
#include <string>

#include "keyMapping.hpp"

// Constructor to initialize the random number generator with a seed.
// These are declared static so there is only one instance across all objects
// of the myRandom class.
thread_local std::random_device myRandom::rd;
thread_local std::mt19937 myRandom::gen(myRandom::rd());

int myRandom::generateRandomInt(int min, int max) {
    static std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

std::string myRandom::generateRandomString(size_t length) {
    constexpr char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    constexpr size_t chars_len = sizeof(chars) - 1;
    
    static std::uniform_int_distribution<> distrib(0, chars_len - 1);

    std::string randomStr;
    randomStr.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        randomStr += chars[distrib(gen)];
    }
    return randomStr;
}

std::string myRandom::generateRandomNumberString(size_t length) {
    constexpr char chars[] = "0123456789";
    constexpr size_t chars_len = sizeof(chars) - 1;
    
    static std::uniform_int_distribution<> distrib(0, chars_len - 1);

    std::string randomStr;
    randomStr.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        randomStr += chars[distrib(gen)];
    }
    return randomStr;
}

bool myRandom::getRandomBool() {
    static std::uniform_int_distribution<> distrib(0, 1);

    // Return true if random number is 1, else false
    return distrib(gen) == 1;
}

bool myRandom::getRandomVectorFromJSON(std::vector<std::string>& participantData, keyMapping::MappingTemplate mappingTemplate, size_t count) {
    // We know we'll need exactly count elements, so reserve space for them
    // to reduce the number of reallocations.
    participantData.reserve(count);

    // We only care about the keys of this JSON object.
    std::vector<std::string>* keys = keyMapping::getKeysForJsonTemplate(mappingTemplate);
    if (!keys || keys->empty()) {
        std::cerr << "Error: Could not get keys for JSON template." << std::endl;
        if (!keys) {
            std::cerr << "Error: keys is null." << std::endl;
        } else if (keys->empty()) {
            std::cerr << "Error: keys is empty." << std::endl;
        }

        return false;
    }

    std::uniform_int_distribution<> distrib(0, keys->size() - 1);

    for (int i = 0; i < count; i++) {
        int randomIndex = distrib(gen);
        participantData.push_back((*keys)[randomIndex]);
    }

    return true;
}
