#include <iostream>

// project dependencies
#include "dependencies/json.hpp"

#include "mapping.hpp"
#include "myRandom.hpp"
#include "oceanBuilder.hpp"


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

// Static helper function to get a random value from a given JSON file
std::string oceanBuilder::getRandomFromJson() {

    // Collect all values
    std::vector<std::string> values;
    for (auto it = mapping::SPORTS_CARS_JSON.begin(); it != mapping::SPORTS_CARS_JSON.end(); ++it) {
        values.push_back(it.value());
    }

    int randomIndex = myRandom::generateRandomInt(0, values.size() - 1);
    return values[randomIndex]; // Return a random value from the file
}
