#pragma once
#include "OpenSeaClient.h"

inline json getCollectionNFTs(OpenSeaClient& client,
                              const std::string& collectionSlug,
                              int limit = 50,
                              const std::string& next = "") {
    std::string url = "https://api.opensea.io/api/v2/collection/" + collectionSlug +
                      "/nfts?limit=" + std::to_string(limit);
    if (!next.empty()) {
        url += "&next=" + next;
    }
    return client.get(url);
}
