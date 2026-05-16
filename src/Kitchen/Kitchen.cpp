//
// Created by pavel on 12/05/2026.
//

#include "Kitchen.hpp"

#include <iostream>
#include <bits/this_thread_sleep.h>

KitchenHandle::KitchenHandle(Internal::Process&& proc, Internal::Socket&& socket)
    : _process(proc), _ipc(std::move(socket)) {}

void KitchenHandle::close() {
    this->_process.kill();
    this->_process.wait();
}

int KitchenHandle::ipcFd() const {
    return this->_ipc.fd();
}

Kitchen::Kitchen(const std::string &socketPath, int nbCooks, int restock_timer)
    : _ipc(socketPath) {
}

void Kitchen::run() {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "I gonna die now" << std::endl;
}