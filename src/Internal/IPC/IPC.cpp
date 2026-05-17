//
// Created by pavel on 16/05/2026.
//

#include "IPC.hpp"
#include "Errors/PlazzaErrors.hpp"
#include "Pizzas.hpp"

namespace Internal {
    IPC::IPC(Socket&& accepted) : _socket(std::move(accepted)) {}

    IPC::IPC(const std::string& socketPath) {
        if (_socket.connect(socketPath) == -1)
            throw PlazzaException(ConnectError);
    }

    int IPC::fd() const {
        return this->_socket.fd();
    }

    IPC& IPC::operator<<(const Message& msg) {
        this->_socket.write(reinterpret_cast<const char *>(&msg), sizeof(msg));
        return *this;
    }

    IPC& IPC::operator>>(Message& msg) {
        ssize_t totla = 0;
        constexpr ssize_t expected = sizeof(msg);
        const auto buf = reinterpret_cast<char *>(&msg);

        while (totla < expected) {
            ssize_t n = this->_socket.read(buf + totla, expected - totla);
            if (n <= 0)
                throw PlazzaException(IPCReadError);
            totla += n;
        }
        return *this;
    }
}