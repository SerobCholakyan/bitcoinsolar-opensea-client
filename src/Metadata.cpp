#include "Metadata.h"

std::string Metadata::getName(const json& asset) {
    auto it = asset.object().find("name");
    if (it == asset.object().end() || !it->second.is_string()) return "";
    return it->second.string();
}

std::string Metadata::getDescription(const json& asset) {
    auto it = asset.object().find("description");
    if (it == asset.object().end() || !it->second.is_string()) return "";
    return it->second.string();
}

std::string Metadata::getImageUrl(const json& asset) {
    auto it = asset.object().find("image_url");
    if (it == asset.object().end() || !it->second.is_string()) return "";
    return it->second.string();
}
