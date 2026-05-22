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

void Kitchen::run() {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "I ended the pizza" << std::endl;

    Message msg1{};
    Message msg2{};

    this->_ipc >> msg1;
    this->_ipc >> msg2;

    this->_ipc << Message {
        .type = MessageType::Done
    };

    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "I gonna die now " << static_cast<int>(msg1.type) << " " << static_cast<int>(msg2.type) << std::endl;
}