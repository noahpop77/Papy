#pragma once

#include <string>

// project dependencies
#include "dependencies/json.hpp"

class lighthouseBuilder {
public:
    // Function to generate a random match based on the template
    static nlohmann::json randomLighthouse();

private:
    // Internal function to get a random key from a JSON file
    static std::string getRandomFromJson(std::string mappingTemplate);
    static std::string formatDateISO(time_t t);
    static void generateTwoDatesOneOrThreeYearsApart(std::string& dateA, std::string& dateB);
};
