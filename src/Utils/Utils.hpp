//
// Created by pavel on 14/05/2026.
//

#pragma once
#include <string>
#include "Pizzas.hpp"

class Utils {
public:
    static std::string trim(const std::string& to_clean);
    static Message parsePizzaOrder(const std::string &order);
};