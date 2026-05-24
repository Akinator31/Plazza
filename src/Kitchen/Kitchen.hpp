//
// Created by pavel on 12/05/2026.
//

#pragma once

#include "Internal/Process/Process.hpp"
#include "Internal/IPC/IPC.hpp"
#include "Kitchen/Cooker/Cooker.hpp"
#include "Pizzas.hpp"
#include "Mutex/Mutex.hpp"
#include <array>

class Cooker;

class KitchenHandle {
    Internal::Process _process;
    int _currentLoad = 0;

public:
    std::optional<Message> pendingMessages;
    Internal::IPC ipc;

    KitchenHandle(Internal::Process &&proc, Internal::Socket &&socket);

    [[nodiscard]] bool isSaturated(int nb_cooks) const;

    [[nodiscard]] int load() const;

    void sendOrder(const Message &order);

    void notifyDone();

    void close() const;

    [[nodiscard]] int ipcFd() const;

    [[nodiscard]] pid_t pid() const;
};

class Kitchen {
    Internal::IPC _ipc;

public:
    Kitchen(const std::string &socketPath, int nbCooks, int restock_timer);
    void run();
    void handleReceptionCommand(Message &order);
    std::array<Cooker *, 3> _cookers;
    std::vector<PizzaRecipe> _pizzaQueue;
    Stock _stock;
    Mutex _stockMutex;
    Mutex _pizzaQueueMutex;
    Semaphore _pizzaQueueSemaphore;
    uint _multiplier = 1;
};

struct KitchenKey {
    int    fd;
    pid_t  pid;

    bool operator<(const KitchenKey& other) const {
        if (fd != other.fd)
            return fd < other.fd;
        return pid < other.pid;
    }
};