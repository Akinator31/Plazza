//
// Created by pavel on 06/05/2026.
//

#include "Reception.hpp"
#include "Errors/PlazzaErrors.hpp"

Reception::Reception(const int ac, char** av) {
    if (ac != 4)
        throw PlazzaException(IncorrectArgs);

    try {
        this->cooking_time_multiplier = std::stof(av[1]);
        this->cook_per_chicken = std::stoi(av[2]);
        this->time_to_replace_ingredients = std::stoi(av[3]);
    } catch ([[maybe_unused]] std::exception &e) {
        throw PlazzaException(IncorrectArgs);
    }
}