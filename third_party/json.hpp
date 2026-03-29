
---

### `third_party/json.hpp` (minimal parser)

```cpp
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <stdexcept>
#include <cctype>

class Json {
public:
    using object_t = std::unordered_map<std::string, Json>;
    using array_t  = std::vector<Json>;
    using value_t  = std::variant<std::nullptr_t, bool, double, std::string, object_t, array_t>;

    value_t value;

    Json() : value(nullptr) {}
    Json(std::nullptr_t) : value(nullptr) {}
    Json(bool b) : value(b) {}
    Json(double d) : value(d) {}
    Json(const std::string& s) : value(s) {}
    Json(const char* s) : value(std::string(s)) {}
    Json(const object_t& o) : value(o) {}
    Json(const array_t& a) : value(a) {}

    bool is_object() const { return std::holds_alternative<object_t>(value); }
    bool is_array()  const { return std::holds_alternative<array_t>(value); }
    bool is_string() const { return std::holds_alternative<std::string>(value); }
    bool is_number() const { return std::holds_alternative<double>(value); }
    bool is_bool()   const { return std::holds_alternative<bool>(value); }
    bool is_null()   const { return std::holds_alternative<std::nullptr_t>(value); }

    object_t& object() { return std::get<object_t>(value); }
    const object_t& object() const { return std::get<object_t>(value); }

    array_t& array() { return std::get<array_t>(value); }
    const array_t& array() const { return std::get<array_t>(value); }

    std::string& string() { return std::get<std::string>(value); }
    const std::string& string() const { return std::get<std::string>(value); }

    double number() const { return std::get<double>(value); }
    bool boolean() const { return std::get<bool>(value); }

    Json& operator[](const std::string& key) {
        return std::get<object_t>(value)[key];
    }

    const Json& operator[](const std::string& key) const {
        return std::get<object_t>(value).at(key);
    }

    Json& operator[](size_t idx) {
        return std::get<array_t>(value)[idx];
    }

    const Json& operator[](size_t idx) const {
        return std::get<array_t>(value)[idx];
    }

    static Json parse(const std::string& s) {
        size_t i = 0;
        return parse_value(s, i);
    }

private:
    static void skip_ws(const std::string& s, size_t& i) {
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) i++;
    }

    static Json parse_value(const std::string& s, size_t& i) {
        skip_ws(s, i);
        if (i >= s.size()) throw std::runtime_error("Unexpected end of JSON");

        char c = s[i];

        if (c == '{') return parse_object(s, i);
        if (c == '[') return parse_array(s, i);
        if (c == '"') return parse_string(s, i);
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '-') return parse_number(s, i);
        if (s.compare(i, 4, "true") == 0)  { i += 4; return Json(true); }
        if (s.compare(i, 5, "false") == 0) { i += 5; return Json(false); }
        if (s.compare(i, 4, "null") == 0)  { i += 4; return Json(nullptr); }

        throw std::runtime_error("Invalid JSON value");
    }

    static Json parse_object(const std::string& s, size_t& i) {
        object_t obj;
        i++; 
        skip_ws(s, i);

        if (s[i] == '}') { i++; return obj; }

        while (true) {
            skip_ws(s, i);
            std::string key = parse_string(s, i).string();

            skip_ws(s, i);
            if (s[i] != ':') throw std::runtime_error("Expected ':'");
            i++;

            Json val = parse_value(s, i);
            obj[key] = val;

            skip_ws(s, i);
            if (s[i] == '}') { i++; break; }
            if (s[i] != ',') throw std::runtime_error("Expected ','");
            i++;
        }

        return obj;
    }

    static Json parse_array(const std::string& s, size_t& i) {
        array_t arr;
        i++; 
        skip_ws(s, i);

        if (s[i] == ']') { i++; return arr; }

        while (true) {
            Json val = parse_value(s, i);
            arr.push_back(val);

            skip_ws(s, i);
            if (s[i] == ']') { i++; break; }
            if (s[i] != ',') throw std::runtime_error("Expected ','");
            i++;
        }

        return arr;
    }

    static Json parse_string(const std::string& s, size_t& i) {
        i++; 
        std::string out;

        while (i < s.size()) {
            char c = s[i++];
            if (c == '"') break;
            if (c == '\\') {
                char esc = s[i++];
                if (esc == '"' || esc == '\\' || esc == '/') out.push_back(esc);
                else if (esc == 'n') out.push_back('\n');
                else if (esc == 't') out.push_back('\t');
                else throw std::runtime_error("Unsupported escape");
            } else {
                out.push_back(c);
            }
        }

        return Json(out);
    }

    static Json parse_number(const std::string& s, size_t& i) {
        size_t start = i;
        while (i < s.size() && (std::isdigit(static_cast<unsigned char>(s[i])) || s[i] == '.' || s[i] == '-' || s[i] == '+'))
            i++;
        return Json(std::stod(s.substr(start, i - start)));
    }
};
