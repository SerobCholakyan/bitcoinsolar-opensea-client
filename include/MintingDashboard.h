#include "MintingDashboard.h"
#include <iostream>

void MintingDashboard::printMintStatus(const json& asset) {
    auto it = asset.object().find("minted_at");
    if (it != asset.object().end() && it->second.is_string()) {
        std::cout << "Minted at: " << it->second.string() << "\n";
    } else {
        std::cout << "Minting timestamp not available.\n";
    }
}
