#include "matchBuilder.hpp"

#include <iostream>
#include <string>

#include "dependencies/json.hpp"
#include "mapping.hpp"
#include "myRandom.hpp"

using json = nlohmann::json;

// TODO: Inspect if its best practice to hardcode path to matchTemplate
// static
json matchBuilder::randomMatch() {
    json matchTemplate;
    try {
        matchTemplate = json::parse(mapping::MATCH_TEMPLATE_JSON_MINIFIED);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
        // Handle the error or return early
    }

    // TODO: JSON payload can be parsed but structure is not validated

    matchTemplate["info"]["gameCreation"] = 9466848;
    matchTemplate["info"]["gameDuration"] = myRandom::generateRandomInt(900, 2700);
    matchTemplate["info"]["gameEndTimestamp"] = 9466848;
    matchTemplate["info"]["gameStartTimestamp"] = 9466848;

    matchTemplate["metadata"]["matchId"] = "TEST1_" + myRandom::generateRandomNumberString(10);

    // Randomly determine win/loss for the different teams
    bool firstSetWin = myRandom::getRandomBool();
    bool secondSetWin = !firstSetWin;
    int participantIndex = 0;
    for (json& participant : matchTemplate["info"]["participants"]) {
        if (participantIndex < 5) {
            participant["win"] = firstSetWin;
        } else {
            participant["win"] = secondSetWin;
        }
        participantIndex++;
    }

    // Used in assigning the first user to bsawatestuser#test
    bool isFirstIteration = true;

    // Will loop 10 times, once for each participant in game
    for (json& participant : matchTemplate["info"]["participants"]) {
        // Randomly generate kills, assists, and other stats
        participant["assists"] = myRandom::generateRandomInt(0, 25);
        participant["deaths"] = myRandom::generateRandomInt(0, 25);
        participant["kills"] = myRandom::generateRandomInt(0, 25);

        // Randomize gold, damage, minion kills, etc.
        participant["goldEarned"] = myRandom::generateRandomInt(1, 20000);
        participant["totalDamageDealtToChampions"] = myRandom::generateRandomInt(1, 90000);
        participant["totalMinionsKilled"] = myRandom::generateRandomInt(1, 200);
        participant["totalAllyJungleMinionsKilled"] = myRandom::generateRandomInt(1, 100);
        participant["totalEnemyJungleMinionsKilled"] = myRandom::generateRandomInt(1, 50);

        // Randomizes Champ related data
        participant["champExperience"] = myRandom::generateRandomInt(1, 12576);
        participant["champLevel"] = myRandom::generateRandomInt(1, 18);
        participant["championId"] = myRandom::generateRandomInt(1, 200);
        participant["championName"] = getRandomFromJson(mapping::CHAMPSIONS_JSON);

        // Randomizes items purchased
        participant["item0"] = getRandomFromJson(mapping::ITEMS_JSON);
        participant["item1"] = getRandomFromJson(mapping::ITEMS_JSON);
        participant["item2"] = getRandomFromJson(mapping::ITEMS_JSON);
        participant["item3"] = getRandomFromJson(mapping::ITEMS_JSON);
        participant["item4"] = getRandomFromJson(mapping::ITEMS_JSON);
        participant["item5"] = getRandomFromJson(mapping::ITEMS_JSON);
        participant["item6"] = getRandomFromJson(mapping::ITEMS_JSON);

        // Randomizes summoner spells
        participant["summoner1Id"] = getRandomFromJson(mapping::SUMMMONERS_JSON);
        participant["summoner2Id"] = getRandomFromJson(mapping::SUMMMONERS_JSON);

        // Randomizes keystone rune and secondary school
        participant["perks"]["styles"][0]["selections"][0]["perk"] = getRandomFromJson(mapping::KEYSTONES_JSON);
        participant["perks"]["styles"][1]["style"] = getRandomFromJson(mapping::SECONDARY_RUNES_JSON);

        // Random Summoner Name and Riot Account data
        if (isFirstIteration) {
            participant["riotIdGameName"] = "bsawatestuser";
            participant["riotIdTagline"] = "test";
            participant["summonerName"] = "bsawatestuser";
            isFirstIteration = false; // Ensure only the first iteration uses predefined strings
        } else {
            participant["riotIdGameName"] = myRandom::generateRandomString(8);
            participant["riotIdTagline"] = myRandom::generateRandomString(3);
            participant["summonerName"] = myRandom::generateRandomString(8);
        }
    }

    return matchTemplate;
}

// Internal function to remove the first and last characters of a string
std::string matchBuilder::dropFirstAndLast(const std::string& str) const {
    if (str.length() > 2) { return str.substr(1, str.length() - 2); }
    return ""; // Return an empty string if the input is too short
}

// Static helper function to get a random key from a given JSON file
std::string matchBuilder::getRandomFromJson(const std::string& jsonString) {
    json items;

    try {
        items = json::parse(jsonString);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON Parse Error in getRandomFromJson: " << e.what() << std::endl;
        return "";
    }

    // Ensure the JSON is not empty
    if (items.empty()) {
        std::cerr << "Error: items JSON is empty!" << std::endl;
        return "";
    }

    // Collect all keys
    std::vector<std::string> keys;
    for (auto it = items.begin(); it != items.end(); ++it) {
        keys.push_back(it.key());
    }

    // Check if the keys vector is empty
    if (keys.empty()) {
        std::cerr << "Error: No keys available in JSON object." << std::endl;
        return "";
    }

    int randomIndex = myRandom::generateRandomInt(0, keys.size() - 1);

    return keys[randomIndex]; // Return a random key from the file
}
