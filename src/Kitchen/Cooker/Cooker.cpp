#include "Cooker.hpp"
#include "Pizzas.hpp"
#include "../Kitchen.hpp"
#include <chrono>
#include <thread>

static void removeIngredients(Stock &stock, const std::vector<Ingredient> &ingredients) {
    for (const auto &ingredient : ingredients) {
        if (stock.find(ingredient.type) != stock.end()) {
            stock[ingredient.type] -= ingredient.quantity;
        }
    }
}

void *Cooker::run(void *arg) {
    Cooker *cooker = static_cast<Cooker *>(arg);
    Semaphore &timeToWork = cooker->_kitchen->_pizzaQueueSemaphore;
    Mutex &pizzaQueueMutex = cooker->_kitchen->_pizzaQueueMutex;
    Mutex &stockMutex = cooker->_kitchen->_stockMutex;
    PizzaRecipe pizza;

    auto hasEnoughIngredients = [](const Stock &stock, const std::vector<Ingredient> &ingredients) {
        for (const auto &ingredient : ingredients) {
            const auto it = stock.find(ingredient.type);
            if (it == stock.end() || it->second < ingredient.quantity)
                return false;
        }
        return true;
    };

    while (true) {
        timeToWork.wait();
        pizzaQueueMutex.lock();
        if (cooker->_kitchen->_pizzaQueue.empty()) {
            pizzaQueueMutex.unlock();
            continue;
        }
        pizza = cooker->_kitchen->_pizzaQueue.back();
        cooker->_kitchen->_pizzaQueue.pop_back();
        stockMutex.lock();
        if (!hasEnoughIngredients(cooker->_kitchen->_stock, pizza.ingredients)) {
            stockMutex.unlock();
            cooker->_kitchen->_pizzaQueue.push_back(pizza);
            pizzaQueueMutex.unlock();
            continue;
        }
        removeIngredients(cooker->_kitchen->_stock, pizza.ingredients);
        stockMutex.unlock();
        pizzaQueueMutex.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(
            pizza.cookingTime * cooker->_kitchen->_multiplier));
    }
    return nullptr;
}
