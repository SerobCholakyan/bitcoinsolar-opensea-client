#include <iostream>
#include <cstdlib>
#include "OpenSeaClient.h"
#include "Gallery.h"
#include "Metadata.h"
#include "Rarity.h"
#include "Marketplace.h"
#include "MintingDashboard.h"

int main() {
    const char* token = std::getenv("OPENSEA_MCP_TOKEN");
    if (!token) {
        std::cerr << "OPENSEA_MCP_TOKEN not set.\n";
        return 0;
    }

    OpenSeaClient client(token);

    auto asset = client.getAsset("0xd12aa3c5369d17ff1ef71cdfb8ee97b4e2d10651", "1");
    if (!asset) {
        std::cerr << "Failed to fetch asset.\n";
        return 0;
    }

    std::cout << "=== Metadata ===\n";
    std::cout << "Name: " << Metadata::getName(*asset) << "\n";
    std::cout << "Description: " << Metadata::getDescription(*asset) << "\n";
    std::cout << "Image: " << Metadata::getImageUrl(*asset) << "\n\n";

    std::cout << "=== Traits ===\n";
    Rarity::printTraits(*asset);

    std::cout << "\n=== Listing Info ===\n";
    Marketplace::printListingInfo(*asset);

    std::cout << "\n=== Mint Status ===\n";
    MintingDashboard::printMintStatus(*asset);

    return 0;
}
