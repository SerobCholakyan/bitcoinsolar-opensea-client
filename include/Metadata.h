#pragma once
#include "OpenSeaClient.h"

inline json getNFT(OpenSeaClient& client,
                   const std::string& chain,
                   const std::string& contract,
                   const std::string& tokenId) {
    std::string url = "https://api.opensea.io/api/v2/chain/" + chain +
                      "/contract/" + contract + "/nfts/" + tokenId;
    return client.get(url);
}
