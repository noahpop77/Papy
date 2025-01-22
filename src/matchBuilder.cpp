#include "matchBuilder.hpp"

#include <string>
#include <iostream>
#include "dependencies/json.hpp"

#include "keyMapping.hpp"
#include "mapping.hpp"
#include "myRandom.hpp"

using json = nlohmann::json;

json matchBuilder::randomMatch() {
    json matchTemplate = mapping::MATCH_TEMPLATE_JSON;

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

    // Batch fetching random values to save on runtime massively
    std::vector<std::string> participantItems;
    myRandom::getRandomVectorFromJSON(participantItems, keyMapping::MappingTemplate::ITEMS_JSON, 70);

    std::vector<std::string> participantChamp;
    myRandom::getRandomVectorFromJSON(participantChamp, keyMapping::MappingTemplate::CHAMPIONS_JSON, 10);

    std::vector<std::string> participantSummoners;
    myRandom::getRandomVectorFromJSON(participantSummoners, keyMapping::MappingTemplate::SUMMMONERS_JSON, 20);

    std::vector<std::string> participantKeystone;
    myRandom::getRandomVectorFromJSON(participantKeystone, keyMapping::MappingTemplate::KEYSTONES_JSON, 10);

    std::vector<std::string> participantSecondary;
    myRandom::getRandomVectorFromJSON(participantSecondary, keyMapping::MappingTemplate::SECONDARY_RUNES_JSON, 10);

    // Will loop 10 times, once for each participant in game
    for (json& participant : matchTemplate["info"]["participants"]) {

        participant["assists"] = myRandom::generateRandomInt(0, 25);
        participant["deaths"] = myRandom::generateRandomInt(0, 25);
        participant["kills"] = myRandom::generateRandomInt(0, 25);

        participant["goldEarned"] = myRandom::generateRandomInt(1, 20000);
        participant["totalDamageDealtToChampions"] = myRandom::generateRandomInt(1, 90000);
        participant["totalMinionsKilled"] = myRandom::generateRandomInt(1, 200);
        participant["totalAllyJungleMinionsKilled"] = myRandom::generateRandomInt(1, 100);
        participant["totalEnemyJungleMinionsKilled"] = myRandom::generateRandomInt(1, 50);

        participant["champExperience"] = myRandom::generateRandomInt(1, 12576);
        participant["champLevel"] = myRandom::generateRandomInt(1, 18);
        participant["championId"] = myRandom::generateRandomInt(1, 200);

        participant["championName"] = participantChamp.back();
        participantChamp.pop_back();

        for (int i = 0; i < 7; i++){
            std::string key = "item" + std::to_string(i);
            participant[key] = participantItems.back();
            participantItems.pop_back();
        }

        participant["summoner1Id"] = participantSummoners.back();
        participantSummoners.pop_back();
        participant["summoner2Id"] = participantSummoners.back();
        participantSummoners.pop_back();

        participant["perks"]["styles"][0]["selections"][0]["perk"] = participantKeystone.back();
        participantKeystone.pop_back();

        participant["perks"]["styles"][1]["style"] = participantSecondary.back();
        participantSecondary.pop_back();

        // First iteration static placeholder name used for debugging
        if (isFirstIteration) {
            participant["riotIdGameName"] = "bsawatestuser";
            participant["riotIdTagline"] = "test";
            participant["summonerName"] = "bsawatestuser";
            isFirstIteration = false;
        } else {
            participant["riotIdGameName"] = myRandom::generateRandomString(8);
            participant["riotIdTagline"] = myRandom::generateRandomString(3);
            participant["summonerName"] = myRandom::generateRandomString(8);
        }
    }

    return matchTemplate;
}
