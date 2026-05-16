//
// Created by pavel on 16/05/2026.
//

#include "IPC.hpp"

#include "Errors/PlazzaErrors.hpp"

enum class MessageType : uint8_t {
    Order,
    Done,
    Status
};

enum PizzaType {
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8,
};

enum PizzaSize {
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16,
};

struct Message {
    MessageType type;
    PizzaType pizzaType;
    PizzaSize pizzaSize;
};

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