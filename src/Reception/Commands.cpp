//
// Created by pavel on 13/05/2026.
//

#include "Commands.hpp"

#include <iostream>
#include <sstream>

#include "Utils/Utils.hpp"

void Commands::status(Reception& reception, [[maybe_unused]] const std::string& order = "") {
    std::cout << "STATUS" << std::endl;

    reception.spawnKitchen();
}

void Commands::order(Reception& reception, const std::string& order) {
    const Message pizzaOrder = Utils::parsePizzaOrder(order);

    for (int i = 0; i < pizzaOrder.pizzaNumber; i++) {
        KitchenHandle* kitchen = reception.leastLoadedKitchen();

        if (!kitchen)
            reception.enqueueOrder(pizzaOrder);
        else
            kitchen->sendOrder(pizzaOrder);
    }
}