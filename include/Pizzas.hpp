//
// Created by pavel on 17/05/2026.
//

#pragma once
#include <cstdint>

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