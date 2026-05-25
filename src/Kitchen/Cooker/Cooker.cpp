#include "Cooker.hpp"
#include "Pizzas.hpp"
#include "../Kitchen.hpp"
#include <chrono>
#include <thread>

Cooker::Cooker(Kitchen *kitchen) : _kitchen(kitchen) {
    _thread = std::make_unique<Thread>([this] { this->run(); });
}

void Cooker::start() {
    _thread->start();
}

static bool handleIngredients(Kitchen &k, const PizzaRecipe &pizza) {
    if (!pizza.hasEnoughIngredients(k._stock)) {
        k._stockMutex.unlock();
        k._pizzaQueueMutex.lock();
        k._pizzaQueue.insert(k._pizzaQueue.begin(), pizza);
        k._pizzaQueueMutex.unlock();
        k._pizzaQueueSemaphore.post();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return false;
    }
    for (const auto &ing : pizza.ingredients)
        k._stock[ing.type] -= ing.quantity;
    return true;
}

void Cooker::run() {
    Kitchen &k = *_kitchen;

    while (true) {
        k._pizzaQueueSemaphore.wait();
        k._pizzaQueueMutex.lock();
        if (k._pizzaQueue.empty()) {
            k._pizzaQueueMutex.unlock();
            std::this_thread::yield();
            continue;
        }
        PizzaRecipe pizza = k._pizzaQueue.back();

        k._pizzaQueue.pop_back();
        k._pizzaQueueMutex.unlock();

        k._stockMutex.lock();
        if (!handleIngredients(k, pizza)) {
            k._stockMutex.unlock();
            continue;
        }
        k._stockMutex.unlock();

        k._busyMutex.lock();
        k._busyCooks++;
        k._busyMutex.unlock();

        auto cookMs = static_cast<long>(
            static_cast<float>(pizza.cookingTime) * 1000.0f * k._multiplier);
        std::this_thread::sleep_for(std::chrono::milliseconds(cookMs));

        Message done{MessageType::Done, pizza.type, PizzaSize::S, 1};

        k._ipcMutex.lock();
        k._ipc << done;
        k._ipcMutex.unlock();

        k._busyMutex.lock();
        k._busyCooks--;
        k._busyMutex.unlock();

        k._activityMutex.lock();
        k._lastActivity = std::chrono::steady_clock::now();
        k._activityMutex.unlock();
    }
}
