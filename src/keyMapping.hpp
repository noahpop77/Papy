#pragma once

#include <string>
#include <vector>

namespace keyMapping {

enum class MappingTemplate {
    KEYSTONES_JSON,
    SECONDARY_RUNES_JSON,
    SUMMMONERS_JSON,
    ITEMS_JSON,
    CHAMPIONS_JSON
};

void preProcessJsonToKeys();

std::vector<std::string>* getKeysForJsonTemplate(MappingTemplate mappingTemplate);

}  // namespace keyMapping
