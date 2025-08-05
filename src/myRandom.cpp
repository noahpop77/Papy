#include "myRandom.hpp"

#include <iostream>
#include <random>
#include <string>

#include "dependencies/json.hpp"

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

bool myRandom::getKeysFromJsonObject(std::vector<std::string>& keys, const nlohmann::json& jsonObject) {
    if (jsonObject.empty()) {
        std::cerr << "Error: items JSON is empty!" << std::endl;
        return false;
    }

    if (!jsonObject.is_object()) {
        std::cerr << "Error: 'items' is not a valid JSON object!" << std::endl;
        return false;
    }

    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        if (!it.key().empty()) {
            keys.push_back(it.key());
        } else {
            std::cerr << "Warning: Found an invalid key in the JSON object." << std::endl;
            return false;
        }
    }

    if (keys.empty()) {
        std::cerr << "Error: No keys available in JSON object." << std::endl;
        return false;
    }
  
    return true;
}

bool myRandom::getRandomVectorFromJSON(std::vector<std::string>& participantData, const nlohmann::json& jsonObject, size_t count) {
    // We know we'll need exactly count elements, so reserve space for them
    // to reduce the number of reallocations.
    participantData.reserve(count);

    // We only care about the keys of this JSON object.
    std::vector<std::string> keys;
    bool success = getKeysFromJsonObject(keys, jsonObject);

    // unsure if 
    if (!success) {
        return false;
    }

    // std::random_device randomDevice;
    // std::mt19937 generate(randomDevice());
    std::uniform_int_distribution<> distrib(0, keys.size() - 1);

    // comparison of size_t and int can lead to integer under/overflow from wrapping due to narrowing conversion of signdness 
    for (size_t i = 0; i < count; i++) {
        // size_t here will ensure that no signed conversion will happen
        // while I would imagine that the number of keys would never exceed int_max it could happen and lead to undefined behaviour
        size_t randomIndex = distrib(gen);
        // int index = randomIndex % keys.size();
        participantData.push_back(keys[randomIndex]);
        //participantData.push_back(keys.at(index));
    }

    return true;
}
