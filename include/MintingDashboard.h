#pragma once
#include "Metadata.h"

inline bool nftExists(OpenSeaClient& client,
                      const std::string& chain,
                      const std::string& contract,
                      const std::string& tokenId) {
    try {
        auto nft = getNFT(client, chain, contract, tokenId);
        return nft.contains("nft");
    } catch (...) {
        return false;
    }
}
