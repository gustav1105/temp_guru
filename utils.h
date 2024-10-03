#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <algorithm>
#include <cctype>

// Inline helper function to trim leading and trailing whitespaces
inline std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Inline helper function to sanitize string
inline std::string sanitize_string(const std::string& str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result),
                 [](unsigned char c) { return std::isprint(c); });
    return result;
}

#endif // UTILS_H

