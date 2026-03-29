#pragma once
#include "json.hpp"
#include <string>

using json = Json;

class Marketplace {
public:
    static void printListingInfo(const json& asset);
};
