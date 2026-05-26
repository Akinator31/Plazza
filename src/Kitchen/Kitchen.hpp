//
// Created by pavel on 12/05/2026.
//

#pragma once

#include "Internal/Process/Process.hpp"
#include "Internal/IPC/IPC.hpp"
#include "Kitchen/Cooker/Cooker.hpp"
#include "Pizzas.hpp"
#include "Internal/Mutex/Mutex.hpp"
#include "Internal/Semaphore/Semaphore.hpp"
#include <chrono>
#include <memory>
#include <vector>

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
public:
    Internal::IPC _ipc;

    Kitchen(const std::string &socketPath, int nbCooks, int restockTimer, float multiplier);

    void run();
    void handleReceptionCommand(Message &order);
    void restockLoop();
    bool shouldClose();

    std::vector<std::unique_ptr<Cooker>> _cookers;
    std::vector<PizzaRecipe> _pizzaQueue;
    Stock _stock;
    Internal::Mutex _stockMutex;
    Internal::Mutex _pizzaQueueMutex;
    Internal::Semaphore _pizzaQueueSemaphore;
    Internal::Mutex _ipcMutex;
    Internal::Mutex _busyMutex;
    Internal::Mutex _activityMutex;
    int _busyCooks = 0;
    int _nbCooks;
    int _restockTimer;
    float _multiplier;
    std::chrono::steady_clock::time_point _lastActivity;
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
