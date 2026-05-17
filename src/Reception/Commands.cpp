//
// Created by pavel on 13/05/2026.
//

#include "Commands.hpp"

#include <iostream>

void Commands::status(Reception& reception, [[maybe_unused]] const std::string& order = "") {
    std::cout << "STATUS" << std::endl;
}

void Commands::order(Reception& reception, std::string order) {
    std::cout << "ORDER: " << order << std::endl;
}