#pragma once
#include <string>
#include "json.hpp"

using json = Json;

class Metadata {
public:
    static std::string getName(const json& asset);
    static std::string getDescription(const json& asset);
    static std::string getImageUrl(const json& asset);
};
