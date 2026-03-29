#pragma once
#include "json.hpp"
#include <string>

using json = Json;

class Rarity {
public:
    static void printTraits(const json& asset);
};
