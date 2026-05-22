//
// Created by pavel on 06/05/2026.
//

#pragma once
#include <csignal>
#include <functional>
#include <map>
#include <optional>
#include <queue>
#include <string>

#include "Internal/Poller/Poller.hpp"
#include "Kitchen/Kitchen.hpp"

inline volatile sig_atomic_t signalReceived = 0;

class Reception {
    float cooking_time_multiplier = 0.0;
    int cook_per_chicken = 0;
    int time_to_replace_ingredients = 0;

    std::queue<pid_t>_pendingKitchens;
    std::map<pid_t, Internal::Process> _pendingPrecesses;
    std::map<pid_t, KitchenHandle> kitchens;
    Internal::Socket _serverSocket;
    Internal::Poller poller;
    std::string commandBuffer;
    std::queue<Message> _pendingOrders;

    std::map<std::string, std::function<void (Reception&, std::string)>> _map;

    std::map<KitchenKey, std::function<void (int, pid_t)>> _handlers;

public:
    Reception(int ac, char **av);
    ~Reception();

    void startCli();
    void readIncomingCommand();
    void acceptNewKitchen();
    void readKitchenMessages(int pid);
    KitchenHandle* leastLoadedKitchen();
    void spawnKitchen();
    std::optional<std::string> nextCommand();
    void handlePlazzaCommand(const std::string& command);
    void removeClosedKitchen(int fd, pid_t pid);
    void enqueueOrder(const Message& order);
};