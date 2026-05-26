//
// Created by pavel on 16/05/2026.
//

#pragma once
#include "Internal/Socket/Socket.hpp"
#include "Pizzas.hpp"
#include "Internal/Poller/Poller.hpp"
#include "Errors/PlazzaErrors.hpp"

enum IPCStatus {
    TIMEOUT,
    OK
};

namespace Internal {
    class IPC {
        Socket _socket;
        Poller _poller;

    public:
        explicit IPC(Socket &&accepted);

        explicit IPC(const std::string &socketPath);

        [[nodiscard]] int fd() const;

        template<typename T>
        IPC& operator<<(const T& data) {
            this->_socket.write(reinterpret_cast<const char*>(&data), sizeof(data));
            return *this;
        }

        template<typename T>
        IPC& operator>>(T& data) {
            ssize_t total = 0;
            const ssize_t expected = static_cast<ssize_t>(sizeof(data));
            auto buf = reinterpret_cast<char*>(&data);
            while (total < expected) {
                const ssize_t n = this->_socket.read(buf + total, expected - total);
                if (n <= 0)
                    throw PlazzaException(IPCReadError);
                total += n;
            }
            return *this;
        }

        IPCStatus wait(int timeout);
    };
}