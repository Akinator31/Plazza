//
// Created by pavel on 12/05/2026.
//

#include "Kitchen.hpp"
#include "Internal/Thread/Thread.hpp"
#include <thread>
#include <chrono>

KitchenHandle::KitchenHandle(Internal::Process&& proc, Internal::Socket&& socket)
    : _process(proc), pendingMessages(std::nullopt), ipc(std::move(socket)) {}

void KitchenHandle::close() const {
    this->_process.kill();
}

int KitchenHandle::ipcFd() const {
    return this->ipc.fd();
}

bool KitchenHandle::isSaturated(const int nb_cooks) const {
    return _currentLoad >= 2 * nb_cooks;
}

int KitchenHandle::load() const {
    return this->_currentLoad;
}

void KitchenHandle::sendOrder(const Message &order) {
    this->ipc << order;
    this->_currentLoad++;
}

void KitchenHandle::notifyDone() {
    _currentLoad--;
}

pid_t KitchenHandle::pid() const {
    return this->_process.pid();
}

Kitchen::Kitchen(const std::string &socketPath, int nbCooks, int restockTimer, float multiplier)
    : _ipc(socketPath), _nbCooks(nbCooks), _restockTimer(restockTimer), _multiplier(multiplier) {
    _stock = {
        {IngredientType::Dough, 5},
        {IngredientType::Tomato, 5},
        {IngredientType::Gruyere, 5},
        {IngredientType::Ham, 5},
        {IngredientType::Mushrooms, 5},
        {IngredientType::Steak, 5},
        {IngredientType::Eggplant, 5},
        {IngredientType::GoatCheese, 5},
        {IngredientType::ChiefLove, 5}
    };
}

void Kitchen::handleReceptionCommand(Message &message) {
    if (message.type == MessageType::Status) {
        KitchenStatus status{};

        _busyMutex.lock();
        status.busyCooks = _busyCooks;
        _busyMutex.unlock();
        status.totalCooks = _nbCooks;

        _stockMutex.lock();
        for (const auto &[type, qty] : _stock)
            status.stock[static_cast<int>(type)] = qty;
        _stockMutex.unlock();

        Message header{MessageType::Status, {}, {}, 0};
        _ipcMutex.lock();
        _ipc << header;
        _ipc << status;
        _ipcMutex.unlock();
        return;
    }

    if (message.type != MessageType::Order)
        return;

    const PizzaRecipe *recipe = nullptr;
    
    for (const auto &r : pizzaRecipes) {
        if (r.type == message.pizzaType) {
            recipe = &r;
            break;
        }
    }
    if (!recipe)
        return;

    _pizzaQueueMutex.lock();
    _busyMutex.lock();
    const int capacity = 2 * _nbCooks;
    const int current = static_cast<int>(_pizzaQueue.size()) + _busyCooks;
    int accepted = message.pizzaNumber;
    if (current + accepted > capacity)
        accepted = std::max(0, capacity - current);
    for (int i = 0; i < accepted; i++) {
        PizzaRecipe r = *recipe;
        r.orderId = message.orderId;
        _pizzaQueue.push_back(r);
    }
    _busyMutex.unlock();
    _pizzaQueueMutex.unlock();

    for (int i = 0; i < accepted; i++)
        _pizzaQueueSemaphore.post();
    _activityMutex.lock();
    _lastActivity = std::chrono::steady_clock::now();
    _activityMutex.unlock();
}

void Kitchen::restockLoop() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(_restockTimer));
        _stockMutex.lock();
        for (auto &[type, qty] : _stock)
            qty++;
        _stockMutex.unlock();
    }
}

bool Kitchen::shouldClose() {
    _busyMutex.lock();
    const int busy = _busyCooks;
    _busyMutex.unlock();
    if (busy > 0)
        return false;

    _pizzaQueueMutex.lock();
    const bool emptyQueue = _pizzaQueue.empty();
    _pizzaQueueMutex.unlock();
    if (!emptyQueue)
        return false;

    _activityMutex.lock();
    const auto last = _lastActivity;
    _activityMutex.unlock();
    return std::chrono::steady_clock::now() - last > std::chrono::seconds(5);
}

void Kitchen::run() {
    _lastActivity = std::chrono::steady_clock::now();

    for (int i = 0; i < _nbCooks; i++) {
        _cookers.push_back(std::make_unique<Cooker>(this));
        _cookers.back()->start();
    }

    Internal::Thread restockThread([this] { this->restockLoop(); });
    restockThread.start();

    while (true) {
        const IPCStatus ret = _ipc.wait(1000);

        if (ret == OK) {
            Message receivedCommand{};
            try {
                _ipc >> receivedCommand;
            } catch (...) {
                break;
            }
            handleReceptionCommand(receivedCommand);
        }

        if (shouldClose())
            break;
    }
}