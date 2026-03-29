#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <cmath>
#include "json.hpp"

using json = Json;

struct RarityScore {
    double score{};
    int rank{};
};

class RarityEngine {
public:
    // Build rarity table from full collection metadata
    // collection: vector of NFT JSON objects (as returned by OpenSea)
    explicit RarityEngine(const std::vector<json>& collection);

    // Get rarity score + rank for a single asset JSON
    RarityScore getRarity(const json& asset) const;

private:
    using TraitKey = std::pair<std::string, std::string>; // (trait_type, value)

    int totalSupply_;
    std::unordered_map<TraitKey, int, TraitKeyHash> traitCounts_;
    std::unordered_map<int, int> traitCountFrequency_; // number_of_traits -> count
    std::unordered_map<const json*, double> assetScores_; // pointer identity
    std::map<double, int, std::greater<>> scoreToRank_;   // score -> rank

    struct TraitKeyHash {
        std::size_t operator()(const TraitKey& k) const noexcept {
            return std::hash<std::string>()(k.first) ^ (std::hash<std::string>()(k.second) << 1);
        }
    };

    static int getTraitCount(const json& asset);
    static std::vector<TraitKey> extractTraits(const json& asset);

    double computeTraitScore(const TraitKey& key) const;
    double computeTraitCountScore(int traitCount) const;

    void buildTraitFrequencies(const std::vector<json>& collection);
    void computeAllAssetScores(const std::vector<json>& collection);
};
