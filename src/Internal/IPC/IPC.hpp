//
// Created by pavel on 16/05/2026.
//

#pragma once
#include "Internal/Socket/Socket.hpp"
#include "Pizzas.hpp"
#include "Internal/Poller/Poller.hpp"

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

        IPC& operator<<(const Message& msg);

        IPC& operator>>(Message& msg);

        IPCStatus wait(int timeout);
    };
}