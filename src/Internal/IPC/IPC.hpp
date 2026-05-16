//
// Created by pavel on 16/05/2026.
//

#pragma once
#include "Internal/Socket/Socket.hpp"

struct Message;

namespace Internal {
    class IPC {
        Socket _socket;

    public:
        explicit IPC(Socket &&accepted);

        explicit IPC(const std::string &socketPath);

        int fd() const;

        IPC& operator<<(const Message& msg);

        IPC& operator>>(Message& msg);
    };
}