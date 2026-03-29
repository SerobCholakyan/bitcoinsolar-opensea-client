#include <iostream>
#include <cstdlib>
#include "OpenSeaClient.h"
#include "RarityEngine.h"
#include "Rarity.h"
#include "Gallery.h"
#include "Metadata.h"
#include "Marketplace.h"
#include "MintingDashboard.h"

int main() {
    // Load MCP token
    const char* token = std::getenv("OPENSEA_MCP_TOKEN");
    if (!token) {
        std::cerr << "ERROR: OPENSEA_MCP_TOKEN not set.\n";
        return 1;
    }

    // Initialize OpenSea client
    OpenSeaClient client(token);

    // Your collection slug on OpenSea
    std::string slug = "bitcoin-solar-cosmic-apes";

    std::cout << "Fetching full collection metadata...\n";

    // Fetch all 333 NFTs from your collection
    std::vector<json> collection = client.getCollectionAssets(slug);

    if (collection.empty()) {
        std::cerr << "ERROR: Could not fetch collection metadata.\n";
        return 1;
    }

    std::cout << "Loaded " << collection.size() << " NFTs.\n";

    // Build rarity engine
    RarityEngine engine(collection);

    // Example: fetch a specific asset
    std::string contract = "0xd12aa3c5369d17ff1ef71cdfb8ee97b4e2d10651";
    std::string tokenId = "1";

    auto asset = client.getAsset(contract, tokenId);

    if (!asset) {
        std::cerr << "ERROR: Could not fetch asset " << tokenId << ".\n";
        return 1;
    }

    std::cout << "\n=== METADATA ===\n";
    std::cout << "Name: " << Metadata::getName(*asset) << "\n";
    std::cout << "Description: " << Metadata::getDescription(*asset) << "\n";
    std::cout << "Image: " << Metadata::getImageUrl(*asset) << "\n";

    std::cout << "\n=== TRAITS ===\n";
    Rarity::printTraits(*asset);

    std::cout << "\n=== RARITY ===\n";
    RarityScore rs = engine.getRarity(*asset);
    std::cout << "Rarity score: " << rs.score << "\n";
    std::cout << "Rarity rank: " << rs.rank << " / " << collection.size() << "\n";

    std::cout << "\n=== MARKETPLACE ===\n";
    Marketplace::printListingInfo(*asset);

    std::cout << "\n=== MINTING STATUS ===\n";
    MintingDashboard::printMintStatus(*asset);

    return 0;
}
