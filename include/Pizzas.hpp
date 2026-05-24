//
// Created by pavel on 17/05/2026.
//

#pragma once
#include <cstdint>
#include <vector>
#include <map>

enum class MessageType : uint8_t {
    Order,
    Done,
    Status
};

enum PizzaType {
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8,
};

enum PizzaSize {
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16,
};

struct Message {
    MessageType type;
    PizzaType pizzaType;
    PizzaSize pizzaSize;
    uint8_t pizzaNumber;
};

enum class IngredientType {
    Dough,
    Tomato,
    Gruyere,
    Ham,
    Mushrooms,
    Steak,
    Eggplant,
    GoatCheese,
    ChiefLove
};

struct Ingredient {
    IngredientType type;
    int quantity;
};

struct PizzaOrder {
    PizzaSize size;
    PizzaType type;
};

struct PizzaRecipe {
    PizzaType type;
    std::vector<Ingredient> ingredients;
    uint32_t cookingTime;
};

using Stock = std::map<IngredientType, int>;

const std::vector<PizzaRecipe> pizzaRecipes = {
    {
        PizzaType::Margarita,
        {
            {IngredientType::Dough, 1},
            {IngredientType::Tomato, 1},
            {IngredientType::Gruyere, 1}
        },
        1
    },
    {
        PizzaType::Regina,
        {
            {IngredientType::Dough, 1},
            {IngredientType::Tomato, 1},
            {IngredientType::Gruyere, 1},
            {IngredientType::Ham, 1},
            {IngredientType::Mushrooms, 1}
        },
        2
    },
    {
        PizzaType::Americana,
        {
            {IngredientType::Dough, 1},
            {IngredientType::Tomato, 1},
            {IngredientType::Gruyere, 1},
            {IngredientType::Steak, 1}
        },
        2
    },
    {
        PizzaType::Fantasia,
        {
            {IngredientType::Dough, 1},
            {IngredientType::Tomato, 1},
            {IngredientType::Eggplant, 1},
            {IngredientType::GoatCheese, 1},
            {IngredientType::ChiefLove, 1}
        },
        4
    }
};
