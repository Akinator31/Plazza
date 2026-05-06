//
// Created by pavel on 06/05/2026.
//

#pragma once

class Reception {
    float cooking_time_multiplier = 0.0;
    int cook_per_chicken = 0;
    int time_to_replace_ingredients = 0;

public:
    Reception(int ac, char **av);
};