//
// Created by pavel on 06/05/2026.
//

#pragma once
#include <csignal>
#include <functional>
#include <map>
#include <optional>
#include <string>

#include "Internal/Poller/Poller.hpp"
#include "Kitchen/Kitchen.hpp"

inline volatile sig_atomic_t signalReceived = 0;

class Reception {
    float cooking_time_multiplier = 0.0;
    int cook_per_chicken = 0;
    int time_to_replace_ingredients = 0;

    std::map<PID, Kitchen> kitchens;
    Internal::Poller poller;
    std::string commandBuffer;

    std::map<std::string, std::function<void(Reception&, std::string)>> _map;

public:
    Reception(int ac, char **av);
    ~Reception();

    void startCli();
    void readIncomingCommand();
    std::optional<std::string> nextCommand();
    void handlePlazzaCommand(const std::string& command);
};