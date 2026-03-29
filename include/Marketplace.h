#pragma once
#include "OpenSeaClient.h"

inline json getCollectionListings(OpenSeaClient& client,
                                  const std::string& collectionSlug,
                                  int limit = 50,
                                  const std::string& next = "") {
    std::string url = "https://api.opensea.io/api/v2/listings/collection/" +
                      collectionSlug + "?limit=" + std::to_string(limit);
    if (!next.empty()) {
        url += "&next=" + next;
    }
    return client.get(url);
}
