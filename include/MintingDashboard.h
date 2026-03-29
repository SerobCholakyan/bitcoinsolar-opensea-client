#pragma once
#include "json.hpp"
#include <string>

using json = Json;

class MintingDashboard {
public:
    static void printMintStatus(const json& asset);
};
