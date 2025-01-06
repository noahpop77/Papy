#include "oceanBuilder.hpp"

#include <iostream>

#include "mapping.hpp"
#include "myRandom.hpp"

// project dependencies
#include "dependencies/json.hpp"

using json = nlohmann::json;

json oceanBuilder::randomOcean() {
    json oceanTemplate;
    try {
        oceanTemplate = json::parse(mapping::OCEAN_PAYLOAD_TEMPLATE);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
        // Handle the error or return early
    }

    oceanTemplate["overrides"]["business_unit"]["name"] = "Papy Test";
    oceanTemplate["overrides"]["application"]["name"] = myRandom::generateRandomString(7);
    oceanTemplate["overrides"]["project"]["name"] = getRandomFromJson();

    return oceanTemplate;
}

// Internal function to remove the first and last characters of a string
std::string oceanBuilder::dropFirstAndLast(const std::string& str) const {
    if (str.length() > 2) { return str.substr(1, str.length() - 2); }
    return ""; // Return an empty string if the input is too short
}

// Static helper function to get a random value from a given JSON file
std::string oceanBuilder::getRandomFromJson() {
    json items = json::parse(mapping::SPORTS_CARS);

    // Collect all values
    std::vector<std::string> values;
    for (auto it = items.begin(); it != items.end(); ++it) {
        values.push_back(it.value());
    }

    // Generate a random index
    int randomIndex = myRandom::generateRandomInt(0, values.size() - 1);

    return values[randomIndex]; // Return a random value from the file
}
