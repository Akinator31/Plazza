//
// Created by pavel on 12/05/2026.
//

#pragma once

#include "Internal/Process/Process.hpp"
#include "Internal/IPC/IPC.hpp"

class KitchenHandle {
    Internal::Process _process;
    Internal::IPC _ipc;
    int currentLoad = 0;

public:
    KitchenHandle(Internal::Process &&proc, Internal::Socket &&socket);

    void close();

    [[nodiscard]] int ipcFd() const;
};

class Kitchen {
    Internal::IPC _ipc;

public:
    Kitchen(const std::string &socketPath, int nbCooks, int restock_timer);
    void run();
};