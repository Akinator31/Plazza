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
    } catch ([[maybe_unused]] std::exception &e) {
        throw PlazzaException(IncorrectArgs);
    }

    this->_serverSocket.bind({});
    this->_serverSocket.listen();
    this->poller.add(STDIN_FILENO, POLLIN);
    this->poller.add(this->_serverSocket.fd(), POLLIN);

    this->_handlers[{STDIN_FILENO, 0}] = [this](const int fd, pid_t) {
        if (this->poller.isReadable(fd)) {
            this->readIncomingCommand();

            while (auto cmd = this->nextCommand()) {
                this->handlePlazzaCommand(*cmd);
            }
            std::cout << "> " << std::flush;
        }
    };

    this->_handlers[{this->_serverSocket.fd(), 0}] = [this](const int fd, pid_t) {
        if (this->poller.isReadable(fd)) this->acceptNewKitchen();
    };
}

Reception::~Reception() {
    for (auto& kitchen : this->kitchens | std::views::values) {
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
    char buffer[4096] = {};
    const ssize_t bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

    this->commandBuffer.append(buffer, bytesRead);
}

void Reception::acceptNewKitchen() {
    Internal::Socket accepted = this->_serverSocket.accept();

    pid_t p = _pendingKitchens.front();
    this->_pendingKitchens.pop();

    Internal::Process proc = this->_pendingPrecesses.at(p);
    this->_pendingPrecesses.erase(p);

    this->kitchens.emplace(p, KitchenHandle(std::move(proc), std::move(accepted)));
    this->poller.add(this->kitchens.at(p).ipcFd(), POLLIN);

    const KitchenHandle& kitchen = this->kitchens.at(p);
    this->_handlers[{kitchen.ipc.fd(), kitchen.pid()}] = [this](const int fd, const pid_t pid) {
        if (this->poller.hasHangup(fd)) {
            this->removeClosedKitchen(fd, pid);
            return;
        }
        if (this->poller.isReadable(fd)) this->readKitchenMessages(pid);
    };

    while (!this->_pendingOrders.empty()) {
        KitchenHandle* handle = this->leastLoadedKitchen();
        if (!handle) break;
        handle->sendOrder(this->_pendingOrders.front());
        this->_pendingOrders.pop();
    }
}

void Reception::spawnKitchen() {
    Internal::Process proc;

    if (proc.isChild()) {
        Kitchen kitchen(
            this->_serverSocket.getPath(),
            this->cook_per_chicken,
            this->time_to_replace_ingredients,
            this->cooking_time_multiplier
        );
        kitchen.run();
        exit(0);
    }

    pid_t pid = proc.pid();
    this->_pendingKitchens.push(pid);
    this->_pendingPrecesses.emplace(pid, proc);
    std::cout << "Kitchen spawned !" << std::endl;
}

static const char *ingredientName(const int index) {
    static const char *names[INGREDIENT_COUNT] = {
        "Dough", "Tomato", "Gruyere", "Ham", "Mushrooms",
        "Steak", "Eggplant", "GoatCheese", "ChiefLove"
    };
    return names[index];
}

void Reception::readKitchenMessages(const pid_t pid) {
    Message extracted{};

    this->kitchens.at(pid).ipc >> extracted;
    if (extracted.type == MessageType::Done) {
        std::cout << "Pizza terminé" << std::endl;
        this->kitchens.at(pid).notifyDone();
    } else if (extracted.type == MessageType::Status) {
        KitchenStatus status{};
        this->kitchens.at(pid).ipc >> status;
        std::cout << "Kitchen [PID " << pid << "]:\n";
        std::cout << "  Cooks : " << status.busyCooks << "/" << status.totalCooks << " busy\n";
        std::cout << "  Stock :\n";
        for (int i = 0; i < INGREDIENT_COUNT; i++)
            std::cout << "    " << ingredientName(i) << ": " << status.stock[i] << "\n";
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

KitchenHandle *Reception::leastLoadedKitchen() {
    KitchenHandle *leastKitchen = nullptr;

    for (auto &kitchen : this->kitchens | std::views::values) {
        if (kitchen.isSaturated(this->cook_per_chicken))
            continue;
        if (!leastKitchen || kitchen.load() < leastKitchen->load()) {
            leastKitchen = &kitchen;
        }
    }

    if (leastKitchen)
        return leastKitchen;

    if (!this->_pendingKitchens.empty() || !this->_pendingPrecesses.empty())
        return nullptr;

    this->spawnKitchen();
    return nullptr;
}

void Reception::removeClosedKitchen(const int fd, const pid_t pid) {
    this->poller.remove(fd);
    this->_handlers.erase({fd, pid});
    this->kitchens.erase(pid);
}

void Reception::enqueueOrder(const Message& order) {
    _pendingOrders.push(order);
}

void Reception::broadcastStatus() {
    if (this->kitchens.empty()) {
        std::cout << "No kitchen running." << std::endl;
        return;
    }
    const Message statusMsg{MessageType::Status, {}, {}, 0};
    for (auto &kitchen : this->kitchens | std::views::values)
        kitchen.ipc << statusMsg;
}

void Reception::startCli() {
    std::cout << "> " << std::flush;

    while (!signalReceived) {
        std::vector<KitchenKey> fds;

        if (this->poller.wait() == -1) {
            if (signalReceived > 0) {
                break;
            }
            throw PlazzaException(PollError);
        }

        for (const auto& fd : this->_handlers | std::views::keys)
            fds.push_back(fd);

        for (const auto [fd, pid] : fds) {
            if (this->_handlers.contains({fd, pid}))
                this->_handlers.at({fd, pid})(fd, pid);
        }
    }
}
