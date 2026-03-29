#include "Metadata.h"

std::string Metadata::getName(const json& asset) {
    auto it = asset.object().find("name");
    return (it != asset.object().end() && it->second.is_string())
           ? it->second.string()
           : "";
}

std::string Metadata::getDescription(const json& asset) {
    auto it = asset.object().find("description");
    return (it != asset.object().end() && it->second.is_string())
           ? it->second.string()
           : "";
}

std::string Metadata::getImageUrl(const json& asset) {
    auto it = asset.object().find("image_url");
    return (it != asset.object().end() && it->second.is_string())
           ? it->second.string()
           : "";
}
