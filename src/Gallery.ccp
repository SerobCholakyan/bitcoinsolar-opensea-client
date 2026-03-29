#include "Gallery.h"
#include "Metadata.h"
#include <iostream>

void Gallery::printSummary(const std::vector<json>& assets) {
    std::cout << "=== GALLERY (" << assets.size() << " NFTs) ===\n";
    int idx = 1;
    for (const auto& asset : assets) {
        std::cout << idx++ << ". "
                  << Metadata::getName(asset)
                  << " — " << Metadata::getImageUrl(asset) << "\n";
    }
}
