#include "Marketplace.h"
#include <iostream>

void Marketplace::printListingInfo(const json& asset) {
    auto it = asset.object().find("permalink");
    if (it != asset.object().end() && it->second.is_string()) {
        std::cout << "OpenSea URL: " << it->second.string() << "\n";
    } else {
        std::cout << "No marketplace info available.\n";
    }
}
