/**
 * @file Pizzas.hpp
 * @brief Header file defining pizza types, sizes, ingredients, and related structures.
 */

#pragma once
#include <cstdint>
#include <vector>
#include <map>

/**
 * @enum MessageType
 * @brief Types of messages that can be sent between Reception and Kitchen.
 */
enum class MessageType : uint8_t {
    Order,  ///< A pizza order.
    Done,   ///< Notification that a pizza is done.
    Status  ///< Request for kitchen status.
};

/**
 * @enum PizzaType
 * @brief Different types of pizzas.
 */
enum PizzaType {
    Regina = 1,     ///< Regina pizza.
    Margarita = 2,  ///< Margarita pizza.
    Americana = 4,  ///< Americana pizza.
    Fantasia = 8,   ///< Fantasia pizza.
};

/**
 * @enum PizzaSize
 * @brief Different sizes of pizzas.
 */
enum PizzaSize {
    S = 1,    ///< Small size.
    M = 2,    ///< Medium size.
    L = 4,    ///< Large size.
    XL = 8,   ///< Extra large size.
    XXL = 16, ///< Extra extra large size.
};

/**
 * @struct Message
 * @brief Structure for messages sent over IPC.
 */
struct Message {
    MessageType type;   ///< The type of the message.
    PizzaType pizzaType; ///< The type of the pizza.
    PizzaSize pizzaSize; ///< The size of the pizza.
    uint8_t pizzaNumber; ///< The number of pizzas (usually 1).
    uint32_t orderId = 0; ///< The ID of the order this message belongs to.
};

/**
 * @enum IngredientType
 * @brief Different types of ingredients.
 */
enum class IngredientType {
    Dough,      ///< Dough.
    Tomato,     ///< Tomato.
    Gruyere,    ///< Gruyere cheese.
    Ham,        ///< Ham.
    Mushrooms,  ///< Mushrooms.
    Steak,      ///< Steak.
    Eggplant,   ///< Eggplant.
    GoatCheese, ///< Goat cheese.
    ChiefLove   ///< Chief Love (special ingredient).
};

/**
 * @struct Ingredient
 * @brief Structure representing an ingredient and its quantity.
 */
struct Ingredient {
    IngredientType type; ///< The type of the ingredient.
    int quantity;        ///< The quantity of the ingredient.
};

/**
 * @struct PizzaOrder
 * @brief Structure representing a pizza order.
 */
struct PizzaOrder {
    PizzaSize size; ///< The size of the pizza.
    PizzaType type; ///< The type of the pizza.
};

/**
 * @typedef Stock
 * @brief Type definition for ingredient stock.
 */
using Stock = std::map<IngredientType, int>;

/**
 * @struct PizzaRecipe
 * @brief Structure representing a pizza recipe.
 */
struct PizzaRecipe {
    PizzaType type;                     ///< The type of the pizza.
    std::vector<Ingredient> ingredients; ///< The list of ingredients required.
    uint32_t cookingTime;               ///< The base cooking time in seconds.
    uint32_t orderId = 0;               ///< The ID of the order.

    /**
     * @brief Check if there are enough ingredients in the stock for this recipe.
     * 
     * @param stock The current ingredient stock.
     * @return true if there are enough ingredients.
     * @return false otherwise.
     */
    bool hasEnoughIngredients(const Stock &stock) const {
        for (const auto &ingredient : ingredients) {
            if (stock.find(ingredient.type) == stock.end() || stock.at(ingredient.type) < ingredient.quantity)
                return false;
        }
        return true;
    }
};

/**
 * @brief Number of different ingredient types.
 */
constexpr int INGREDIENT_COUNT = 9;

/**
 * @struct KitchenStatus
 * @brief Structure representing the status of a kitchen.
 */
struct KitchenStatus {
    int busyCooks;                 ///< Number of busy cooks.
    int totalCooks;                ///< Total number of cooks in the kitchen.
    int stock[INGREDIENT_COUNT];   ///< Current stock of each ingredient.
};

/**
 * @brief List of all pizza recipes.
 */
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