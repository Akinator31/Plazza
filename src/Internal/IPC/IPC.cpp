//
// Created by pavel on 16/05/2026.
//

#include "IPC.hpp"
#include "Errors/PlazzaErrors.hpp"
#include "Pizzas.hpp"

namespace Internal {
    IPC::IPC(Socket&& accepted) : _socket(std::move(accepted)) {}

    IPC::IPC(const std::string& socketPath) {
        if (this->_socket.connect(socketPath) == -1)
            throw PlazzaException(ConnectError);

        this->_poller.add(this->_socket.fd(), POLLIN);
    }

    int IPC::fd() const {
        return this->_socket.fd();
    }

    IPCStatus IPC::wait(const int timeout) {
        if (const int ret = this->_poller.wait(timeout); ret == 0)
            return TIMEOUT;
        return OK;
    }
}