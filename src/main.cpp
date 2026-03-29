#include <iostream>
#include <cstdlib>
#include "OpenSeaClient.h"
#include "Gallery.h"
#include "Metadata.h"
#include "Rarity.h"
#include "Marketplace.h"
#include "MintingDashboard.h"

int main() {
    const char* key = std::getenv("OPENSEA_MCP_TOKEN");
    if (!key) {
        std::cerr << "Set OPENSEA_MCP_TOKEN env var.\n";
        return 1;
    }

    OpenSeaClient client(key);
    std::string collection = "bitcoin-solar-cosmic-apes";
    std::string chain = "polygon";
    std::string contract = "0xd12aa3c5369d17ff1ef71cdfb8ee97b4e2d10651";

    // 1) Gallery
    auto page = getCollectionNFTs(client, collection, 50);
    std::cout << "=== GALLERY ===\n";
    for (const auto& nft : page["nfts"]) {
        std::cout << nft["identifier"] << " - " << nft.value("name", "") << "\n";
    }

    // 2) Rarity snapshot
    std::cout << "\n=== RARITY SNAPSHOT ===\n";
    auto stats = computeTraitStats(page);
    for (const auto& [traitType, values] : stats) {
        std::cout << traitType << ":\n";
        for (const auto& [val, s] : values) {
            std::cout << "  " << val << " -> " << s.count << "\n";
        }
    }

    // 3) Metadata for #1
    std::cout << "\n=== METADATA FOR #1 ===\n";
    auto nft = getNFT(client, chain, contract, "1");
    std::cout << nft.dump(2) << "\n";

    // 4) Listings
    std::cout << "\n=== LISTINGS ===\n";
    auto listings = getCollectionListings(client, collection, 20);
    for (const auto& l : listings["listings"]) {
        const auto& pd = l["protocol_data"]["parameters"];
        std::string tokenId = pd["offer"][0]["identifierOrCriteria"];
        std::string price = l["price"]["current"]["value"];
        std::string currency = l["price"]["current"]["currency"];
        std::cout << "Token #" << tokenId << " listed for " << price << " " << currency << "\n";
    }

    // 5) Minting dashboard
    std::cout << "\n=== MINTING DASHBOARD (1–20) ===\n";
    for (int i = 1; i <= 20; ++i) {
        std::string id = std::to_string(i);
        bool exists = nftExists(client, chain, contract, id);
        std::cout << "Token #" << id << (exists ? " ✅" : " ❌") << "\n";
    }

    return 0;
}
