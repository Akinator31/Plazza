//
// Created by pavel on 14/05/2026.
//

#include "Utils.hpp"

std::string Utils::trim(const std::string& to_clean) {
    const auto start = to_clean.find_first_not_of(" \n");

    if (start == std::string::npos)
        return "";
    const auto end = to_clean.find_last_not_of(" \n");

    std::string trimmedString = to_clean.substr(start, end - start + 1);

    if (const size_t comments = trimmedString.find_last_of('#'); comments != std::string::npos)
        return trimmedString.substr(0, comments);
    return trimmedString;
}

Message Utils::parsePizzaOrder(std::string& order) {
    
}