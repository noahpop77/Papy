#include "myRandom.hpp"

#include <string>
#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());

int myRandom::generateRandomInt(int min, int max) {
    // This one can't be static because it depends on the inputs to the function.
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

std::string myRandom::generateRandomString(size_t length) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::uniform_int_distribution<> distrib(0, chars.size() - 1);

    std::string randomStr;
    for (size_t i = 0; i < length; ++i) {
        randomStr += chars[distrib(gen)];
    }
    return randomStr;
}

std::string myRandom::generateRandomNumberString(size_t length) {
    const std::string chars = "0123456789";
    static std::uniform_int_distribution<> distrib(0, chars.size() - 1);

    std::string randomStr;
    for (size_t i = 0; i < length; ++i) {
        randomStr += chars[distrib(gen)];
    }
    return randomStr;
}

bool myRandom::getRandomBool() {
    // Define the distribution for 0 or 1
    static std::uniform_int_distribution<> distrib(0, 1);

    // Return true if random number is 1, else false
    return distrib(gen) == 1;
}
