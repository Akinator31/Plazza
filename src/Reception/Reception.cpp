//
// Created by pavel on 06/05/2026.
//

#include "Reception.hpp"

#include <iostream>
#include <ranges>

#include "Commands.hpp"
#include "Errors/PlazzaErrors.hpp"
#include "Utils/Utils.hpp"

Reception::Reception(const int ac, char** av) {
    if (ac != 4)
        throw PlazzaException(IncorrectArgs);

    try {
        this->cooking_time_multiplier = std::stof(av[1]);
        this->cook_per_chicken = std::stoi(av[2]);
        this->time_to_replace_ingredients = std::stoi(av[3]);

        this->poller.add(STDIN_FILENO, POLLIN);

    } catch ([[maybe_unused]] std::exception &e) {
        throw PlazzaException(IncorrectArgs);
    }
}

Reception::~Reception() {
    for (const auto& kitchen : this->kitchens | std::views::values) {
        kitchen.close();
    }
}

void Reception::handlePlazzaCommand(const std::string& command) {
    if (command.empty())
        return;
    if (command == "status") {
        Commands::status(*this, command);
    } else {
        Commands::order(*this, command);
    }
}

void Reception::readIncomingCommand() {
    if (this->poller.isReadable(STDIN_FILENO)) {
        char buffer[4096] = {};
        const ssize_t bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

        this->commandBuffer.append(buffer, bytesRead);
    }
}

std::optional<std::string> Reception::nextCommand() {
    const size_t pos = this->commandBuffer.find(';');
    std::string command = {};

    if (this->commandBuffer.empty())
        return std::nullopt;

    if (pos == std::string::npos) {
        this->commandBuffer.pop_back();
        command = Utils::trim(this->commandBuffer);
        this->commandBuffer = {};
        return command;
    }

    command = this->commandBuffer.substr(0, pos);
    this->commandBuffer.erase(0, pos + 1);
    return Utils::trim(command);
}

void Reception::startCli() {
    std::cout << "> " << std::flush;

    while (!signalReceived) {
        if (this->poller.wait() == -1) {
            if (signalReceived > 0) {
                break;
            }
            throw PlazzaException(PollError);
        }

        readIncomingCommand();

        while (auto cmd = this->nextCommand()) {
            this->handlePlazzaCommand(*cmd);
        }

        std::cout << "> " << std::flush;
    }
}