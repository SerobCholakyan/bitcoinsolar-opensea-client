#include "RarityEngine.h"

RarityEngine::RarityEngine(const std::vector<json>& collection)
    : totalSupply_(static_cast<int>(collection.size())) {
    buildTraitFrequencies(collection);
    computeAllAssetScores(collection);
}

int RarityEngine::getTraitCount(const json& asset) {
    if (!asset.is_object()) return 0;
    auto& obj = asset.object();
    auto it = obj.find("attributes");
    if (it == obj.end() || !it->second.is_array()) return 0;
    return static_cast<int>(it->second.array().size());
}

std::vector<RarityEngine::TraitKey> RarityEngine::extractTraits(const json& asset) {
    std::vector<TraitKey> traits;
    if (!asset.is_object()) return traits;

    auto it = asset.object().find("attributes");
    if (it == asset.object().end() || !it->second.is_array()) return traits;

    for (const auto& attr : it->second.array()) {
        if (!attr.is_object()) continue;
        const auto& aobj = attr.object();

        auto tt = aobj.find("trait_type");
        auto tv = aobj.find("value");
        if (tt == aobj.end() || tv == aobj.end()) continue;
        if (!tt->second.is_string() || !tv->second.is_string()) continue;

        traits.emplace_back(tt->second.string(), tv->second.string());
    }

    return traits;
}

void RarityEngine::buildTraitFrequencies(const std::vector<json>& collection) {
    for (const auto& asset : collection) {
        auto traits = extractTraits(asset);
        int tcount = static_cast<int>(traits.size());
        traitCountFrequency_[tcount]++;

        for (const auto& key : traits) {
            traitCounts_[key]++;
        }
    }
}

double RarityEngine::computeTraitScore(const TraitKey& key) const {
    auto it = traitCounts_.find(key);
    if (it == traitCounts_.end() || totalSupply_ == 0) return 0.0;

    double freq = static_cast<double>(it->second) / static_cast<double>(totalSupply_);
    if (freq <= 0.0) return 0.0;

    // Statistical rarity: -log(frequency)
    return -std::log(freq);
}

double RarityEngine::computeTraitCountScore(int traitCount) const {
    auto it = traitCountFrequency_.find(traitCount);
    if (it == traitCountFrequency_.end() || totalSupply_ == 0) return 0.0;

    double freq = static_cast<double>(it->second) / static_cast<double>(totalSupply_);
    if (freq <= 0.0) return 0.0;

    return -std::log(freq);
}

void RarityEngine::computeAllAssetScores(const std::vector<json>& collection) {
    // First pass: compute scores
    std::vector<double> scores;
    scores.reserve(collection.size());

    for (const auto& asset : collection) {
        auto traits = extractTraits(asset);
        int tcount = static_cast<int>(traits.size());

        double totalScore = 0.0;
        for (const auto& key : traits) {
            totalScore += computeTraitScore(key);
        }
        totalScore += computeTraitCountScore(tcount);

        assetScores_[&asset] = totalScore;
        scores.push_back(totalScore);
    }

    // Second pass: assign ranks (higher score = better rank)
    std::vector<double> sorted = scores;
    std::sort(sorted.begin(), sorted.end(), std::greater<>());

    int rank = 1;
    for (double s : sorted) {
        if (scoreToRank_.find(s) == scoreToRank_.end()) {
            scoreToRank_[s] = rank;
        }
        rank++;
    }
}

RarityScore RarityEngine::getRarity(const json& asset) const {
    RarityScore rs{};

    auto it = assetScores_.find(&asset);
    if (it == assetScores_.end()) {
        rs.score = 0.0;
        rs.rank = 0;
        return rs;
    }

    double score = it->second;
    rs.score = score;

    auto rit = scoreToRank_.find(score);
    rs.rank = (rit != scoreToRank_.end()) ? rit->second : 0;

    return rs;
}
