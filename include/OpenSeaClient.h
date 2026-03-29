#pragma once
#include <string>
#include <stdexcept>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

class OpenSeaClient {
public:
    explicit OpenSeaClient(std::string apiKey, long timeoutMs = 10000)
        : apiKey_(std::move(apiKey)), timeoutMs_(timeoutMs) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~OpenSeaClient() {
        curl_global_cleanup();
    }

    json get(const std::string& url) const {
        CURL* curl = curl_easy_init();
        if (!curl) throw std::runtime_error("curl_easy_init failed");

        std::string responseBody;
        struct curl_slist* headers = nullptr;
        std::string headerKey = "X-API-KEY: " + apiKey_;

        headers = curl_slist_append(headers, headerKey.c_str());
        headers = curl_slist_append(headers, "Accept: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &OpenSeaClient::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs_);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);
        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error(std::string("curl error: ") + curl_easy_strerror(res));
        }
        if (httpCode < 200 || httpCode >= 300) {
            throw std::runtime_error("HTTP error: " + std::to_string(httpCode) +
                                     " body: " + responseBody);
        }

        return json::parse(responseBody);
    }

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        auto* s = static_cast<std::string*>(userp);
        s->append(static_cast<char*>(contents), size * nmemb);
        return size * nmemb;
    }

    std::string apiKey_;
    long timeoutMs_;
};
