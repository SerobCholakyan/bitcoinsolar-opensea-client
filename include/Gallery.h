#pragma once
#include <vector>
#include "json.hpp"

using json = Json;

class Gallery {
public:
    static void printSummary(const std::vector<json>& assets);
};
