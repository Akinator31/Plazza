//
// Created by pavel on 14/05/2026.
//

#include "Utils.hpp"

#include <algorithm>
#include <sstream>
#include <unordered_map>

#include "Errors/PlazzaErrors.hpp"

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

Message Utils::parsePizzaOrder(const std::string& order) {
    static const std::unordered_map<std::string, PizzaType> typeMap = {
        {"regina",    Regina},
        {"margarita", Margarita},
        {"americana", Americana},
        {"fantasia",  Fantasia},
    };
    static const std::unordered_map<std::string, PizzaSize> sizeMap = {
        {"s",   S},
        {"m",   M},
        {"l",   L},
        {"xl",  XL},
        {"xxl", XXL},
    };

    std::stringstream lineStream(order);
    std::string extractedWord;
    std::string pizzaType, pizzaSize, numberOfPizza;

    while (lineStream >> extractedWord) {
        if (pizzaType.empty())     { pizzaType     = extractedWord; continue; }
        if (pizzaSize.empty())     { pizzaSize      = extractedWord; continue; }
        if (numberOfPizza.empty()) { numberOfPizza  = extractedWord; break;    }
    }

    if (pizzaType.empty() || pizzaSize.empty() || numberOfPizza.empty())
        throw PlazzaException(PizzaOrderError);

    std::ranges::transform(pizzaType, pizzaType.begin(), tolower);
    std::ranges::transform(pizzaSize, pizzaSize.begin(), tolower);

    if (numberOfPizza[0] != 'x')
        throw PlazzaException(PizzaOrderError);
    const std::string numberStr = numberOfPizza.substr(1);
    if (numberStr.empty() || !std::ranges::all_of(numberStr, isdigit))
        throw PlazzaException(PizzaOrderError);

    const auto typeIt = typeMap.find(pizzaType);
    const auto sizeIt = sizeMap.find(pizzaSize);

    if (typeIt == typeMap.end() || sizeIt == sizeMap.end())
        throw PlazzaException(PizzaOrderError);

    return Message{
        MessageType::Order,
        typeIt->second,
        sizeIt->second,
        static_cast<uint8_t>(std::stoul(numberStr))
    };
}