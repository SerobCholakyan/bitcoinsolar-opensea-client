#pragma once
#include <cstdlib>
#include <stdexcept>
#include <string>

class SecretManager {
public:
    static std::string get(const char* name) {
        const char* value = std::getenv(name);
        if (!value) {
            throw std::runtime_error(std::string("Missing environment variable: ") + name);
        }
        return value;
    }
};
