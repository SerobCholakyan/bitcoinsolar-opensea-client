#include "OpenSeaClient.h"
#include <stdexcept>
#include <iostream>

OpenSeaClient::OpenSeaClient(const std::string& apiKey)
    : apiKey_(apiKey), curl_(curl_easy_init()) {
    if (!curl_) throw std::runtime_error("Failed to init CURL");
}

OpenSeaClient::~OpenSeaClient() {
    if (curl_) curl_easy_cleanup(curl_);
}

size_t OpenSeaClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), total);
    return total;
}

std::optional<std::string> OpenSeaClient::httpGet(const std::string& url) {
    if (!curl_) return std::nullopt;

    std::string buffer;

    curl_easy_reset(curl_);
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &OpenSeaClient::writeCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &buffer);

    struct curl_slist* headers = nullptr;
    std::string auth = "x-api-key: " + apiKey_;
    headers = curl_slist_append(headers, auth.c_str());
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl_);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
        return std::nullopt;
    }

    long code = 0;
    curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &code);
    if (code < 200 || code >= 300) {
        std::cerr << "HTTP error: " << code << "\n";
        return std::nullopt;
    }

    return buffer;
}

std::optional<json> OpenSeaClient::getAsset(const std::string& contractAddress,
                                            const std::string& tokenId) {
    std::string url =
        "https://api.opensea.io/api/v2/chain/polygon/contract/" +
        contractAddress + "/nfts/" + tokenId;

    auto res = httpGet(url);
    if (!res) return std::nullopt;

    try {
        return json::parse(*res);
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return std::nullopt;
    }
}

std::vector<json> OpenSeaClient::getCollectionAssets(const std::string& slug,
                                                     int pageSize) {
    std::vector<json> out;
    int next = 0;

    while (true) {
        std::string url =
            "https://api.opensea.io/api/v2/collection/" + slug +
            "/nfts?limit=" + std::to_string(pageSize) +
            "&next=" + std::to_string(next);

        auto res = httpGet(url);
        if (!res) break;

        json parsed;
        try {
            parsed = json::parse(*res);
        } catch (...) {
            break;
        }

        auto it = parsed.object().find("nfts");
        if (it == parsed.object().end() || !it->second.is_array()) break;

        auto& arr = it->second.array();
        if (arr.empty()) break;

        for (auto& nft : arr) out.push_back(nft);

        // naive pagination: stop when fewer than pageSize
        if (static_cast<int>(arr.size()) < pageSize) break;
        next += pageSize;
    }

    return out;
}
