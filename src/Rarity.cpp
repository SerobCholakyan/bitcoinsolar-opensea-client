#include "Rarity.h"
#include <iostream>

void Rarity::printTraits(const json& asset) {
    auto it = asset.object().find("attributes");
    if (it == asset.object().end() || !it->second.is_array()) {
        std::cout << "No traits\n";
        return;
    }

    for (const auto& attr : it->second.array()) {
        auto& a = attr.object();
        std::cout << a.at("trait_type").string()
                  << ": "
                  << a.at("value").string()
                  << "\n";
    }
}

void Rarity::printRarity(const json& asset, const RarityEngine& engine) {
    RarityScore rs = engine.getRarity(asset);

    std::cout << "\n=== RARITY ===\n";
    std::cout << "Score: " << rs.score << "\n";
    std::cout << "Rank: " << rs.rank << "\n";
}
