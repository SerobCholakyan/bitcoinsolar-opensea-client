#include "Gallery.h"
#include "Metadata.h"
#include <iostream>

void Gallery::printSummary(const std::vector<json>& assets) {
    std::cout << "=== GALLERY (" << assets.size() << " NFTs) ===\n";
    int index = 1;

    for (const auto& asset : assets) {
        std::cout << index++ << ". "
                  << Metadata::getName(asset)
                  << " — " << Metadata::getImageUrl(asset)
                  << "\n";
    }
}
