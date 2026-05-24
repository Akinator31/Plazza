//
// Created by pavel on 12/05/2026.
//

#include "Kitchen.hpp"

#include <iostream>
#include <bits/this_thread_sleep.h>

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

Kitchen::Kitchen(const std::string &socketPath, int nbCooks, int restock_timer)
    : _ipc(socketPath) {
}

void Kitchen::handleReceptionCommand(Message &message) {
    PizzaOrder order = {message.pizzaSize, message.pizzaType};

    for (auto nb_pizza = 0; nb_pizza < message.pizzaNumber; nb_pizza++)
        this->_orders.push_back(order);
}

void Kitchen::run() {
    while (true) {
        IPCStatus ret = this->_ipc.wait(5000);

        if (ret == TIMEOUT) {
            std::cout << "Timeout reached! Closing the kitchen." << std::endl;
            break;
        }

        Message receivedCommand{};
        this->_ipc >> receivedCommand;

        this->handleReceptionCommand(receivedCommand);
    }
}