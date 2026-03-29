#pragma once
#include "OpenSeaClient.h"
#include <map>

struct TraitStat {
    int count = 0;
};

using TraitValueMap = std::map<std::string, TraitStat>;
using TraitStatsMap = std::map<std::string, TraitValueMap>;

inline TraitStatsMap computeTraitStats(const json& nftsPage) {
    TraitStatsMap stats;
    if (!nftsPage.contains("nfts")) return stats;

    for (const auto& nft : nftsPage["nfts"]) {
        if (!nft.contains("traits")) continue;
        for (const auto& t : nft["traits"]) {
            std::string type = t.value("trait_type", "");
            std::string value = t.value("value", "");
            if (type.empty() || value.empty()) continue;
            stats[type][value].count++;
        }
    }
    return stats;
}
