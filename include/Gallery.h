#pragma once
#include <string>
#include <vector>
#include "json.hpp"

using json = Json;

class Gallery {
public:
    static void printAssets(const json& assets);
};
