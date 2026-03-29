#pragma once
#include <string>
#include <vector>
#include <optional>
#include <curl/curl.h>
#include "json.hpp"

using json = Json;

class OpenSeaClient {
public:
    explicit OpenSeaClient(const std::string& apiKey);
    ~OpenSeaClient();

    std::optional<json> getAsset(const std::string& contractAddress, const std::string& tokenId);
    std::optional<json> getCollection(const std::string& slug);
    std::optional<json> getAssetsByOwner(const std::string& owner, int limit = 20);

private:
    std::string apiKey_;
    CURL* curl_;

    std::optional<std::string> httpGet(const std::string& url);
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};
