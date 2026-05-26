//
// Created by pavel on 13/05/2026.
//

#include "Commands.hpp"

#include <iostream>
#include <sstream>

#include "Utils/Utils.hpp"

void Commands::status(Reception &reception,
                      [[maybe_unused]] const std::string &order = "") {
  std::cout << "STATUS" << std::endl;

  reception.spawnKitchen();
}

void Commands::order(Reception &reception, const std::string &order) {
    Message pizzaOrder = Utils::parsePizzaOrder(order);

    std::cout << static_cast<int>(pizzaOrder.pizzaNumber) << " "
            << pizzaOrder.pizzaType << std::endl;

    for (int i = 0; i < pizzaOrder.pizzaNumber; i++) {
        KitchenHandle *kitchen = reception.leastLoadedKitchen();

        Message orderSend = {pizzaOrder.type, pizzaOrder.pizzaType,
                             pizzaOrder.pizzaSize, 1};

        if (!kitchen) {
          std::cout << "pas de kitchen" << std::endl;
          reception.enqueueOrder(orderSend);
        } else {
          std::cout << "kitchen dispo" << std::endl;
          kitchen->sendOrder(orderSend);
        }
    }
}
