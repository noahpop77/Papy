#include <iostream>

// project dependencies
#include "dependencies/json.hpp"

#include "mapping.hpp"
#include "myRandom.hpp"
#include "lighthouseBuilder.hpp"

using json = nlohmann::json;

json lighthouseBuilder::randomLighthouse() {
    json lighthouseTemplate;
    try {
        lighthouseTemplate = json::parse(mapping::LIGHTHOUSE_PAYLOAD_TEMPLATE);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
        // Handle the error or return early
    }

    std::string dateA, dateB;
    lighthouseBuilder::generateTwoDatesOneOrThreeYearsApart(dateA, dateB);

    lighthouseTemplate["clientStatus"] = lighthouseBuilder::getRandomFromJson(mapping::CLIENT_STATUS);;
    lighthouseTemplate["clientName"] = myRandom::generateRandomString(7);
    lighthouseTemplate["numberOfLicenses"] = myRandom::generateRandomNumberString(3);
    lighthouseTemplate["licenseType"] = lighthouseBuilder::getRandomFromJson(mapping::LICENSE_TYPE);
    lighthouseTemplate["licenseTermStart"] = dateA;
    lighthouseTemplate["licenseTermEnd"] = dateB;

    return lighthouseTemplate;
}

// Static helper function to get a random value from a given JSON file
std::string lighthouseBuilder::getRandomFromJson(std::string mappingTemplate) {
    json items = json::parse(mappingTemplate);

    // Collect all values
    std::vector<std::string> values;
    for (auto it = items.begin(); it != items.end(); ++it) {
        values.push_back(it.value());
    }

    int randomIndex = myRandom::generateRandomInt(0, values.size() - 1);
    return values[randomIndex]; // Return a random value from the file
}

std::string formatDateISO(time_t t, const std::string& fixedTime = "18:30:00.000Z") {
    std::tm* tm_ptr = std::gmtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y-%m-%dT") << fixedTime;
    return oss.str();
}

void lighthouseBuilder::generateTwoDatesOneOrThreeYearsApart(std::string& dateA, std::string& dateB) {
    // Date range for Date A (Jan 1, 2010 to Dec 31, 2030)
    std::tm start_tm = {};
    start_tm.tm_year = 110;
    start_tm.tm_mon = 0;
    start_tm.tm_mday = 1;

    std::tm end_tm = {};
    end_tm.tm_year = 130;
    end_tm.tm_mon = 11;
    end_tm.tm_mday = 31;

    time_t start_time = std::mktime(&start_tm);
    time_t end_time = std::mktime(&end_tm);

    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate Date A randomly in range (leave 3 years margin at end)
    std::uniform_int_distribution<time_t> distA(start_time, end_time - 3 * 365 * 24 * 3600);
    time_t timeA = distA(gen);

    // Convert timeA back to tm to add years
    std::tm tmA = *std::gmtime(&timeA);

    // Randomly choose 1 or 3 years to add
    std::uniform_int_distribution<int> distYears(0, 1);
    int yearsToAdd = distYears(gen) == 0 ? 1 : 3;

    tmA.tm_year += yearsToAdd;

    // Normalize dateB
    time_t timeB = std::mktime(&tmA);

    // Format both dates
    dateA = formatDateISO(timeA);
    dateB = formatDateISO(timeB);
}
