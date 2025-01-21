#include "keyMapping.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "dependencies/json.hpp"

#include "mapping.hpp"

namespace keyMapping {

namespace {

bool getKeysFromJsonObject(std::vector<std::string> &keys, const nlohmann::json &jsonObject) {
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
            std::cerr << "Warning: Found an invalid key in the JSON object."
                                << std::endl;
            return false;
        }
    }

    if (keys.empty()) {
        std::cerr << "Error: No keys available in JSON object." << std::endl;
        return false;
    }

    return true;
}

}  // namespace

static std::vector<std::string> keystonesJsonKeys;
static std::vector<std::string> secondaryRunesJsonKeys;
static std::vector<std::string> summonersJsonKeys;
static std::vector<std::string> itemsJsonKeys;
static std::vector<std::string> championsJsonKeys;

void preProcessJsonToKeys() {
    getKeysFromJsonObject(keystonesJsonKeys, mapping::KEYSTONES_JSON);
    getKeysFromJsonObject(secondaryRunesJsonKeys, mapping::SECONDARY_RUNES_JSON);
    getKeysFromJsonObject(summonersJsonKeys, mapping::SUMMMONERS_JSON);
    getKeysFromJsonObject(itemsJsonKeys, mapping::ITEMS_JSON);
    getKeysFromJsonObject(championsJsonKeys, mapping::CHAMPIONS_JSON);
}

std::vector<std::string>* getKeysForJsonTemplate(MappingTemplate mappingTemplate) {
    switch (mappingTemplate) {
    case MappingTemplate::KEYSTONES_JSON:
        return &keystonesJsonKeys;
    case MappingTemplate::SECONDARY_RUNES_JSON:
        return &secondaryRunesJsonKeys;
    case MappingTemplate::SUMMMONERS_JSON:
        return &summonersJsonKeys;
    case MappingTemplate::ITEMS_JSON:
        return &itemsJsonKeys;
    case MappingTemplate::CHAMPIONS_JSON:
        return &championsJsonKeys;
    default:
        std::cerr << "Error: Invalid JSON template requested." << std::endl;
        return nullptr;
    }
}

}  // namespace keyMapping
