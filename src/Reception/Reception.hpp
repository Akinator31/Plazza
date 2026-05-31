/**
 * @file Reception.hpp
 * @brief Header file for the Reception class.
 */

#pragma once
#include <csignal>
#include <functional>
#include <map>
#include <optional>
#include <queue>
#include <string>

#include "Internal/Poller/Poller.hpp"
#include "Kitchen/Kitchen.hpp"

/**
 * @brief Global variable to track if a signal has been received.
 */
inline volatile sig_atomic_t signalReceived = 0;

/**
 * @class Reception
 * @brief The main class for the Plazza reception, managing kitchens and processing commands.
 */
class Reception {
    float cooking_time_multiplier = 0.0; ///< Multiplier for the cooking time of pizzas.
    int cook_per_chicken = 0;           ///< Number of cooks per kitchen.
    int time_to_replace_ingredients = 0; ///< Time in ms to replace ingredients.

    /**
     * @struct OrderRecord
     * @brief Structure to track an order's status.
     */
    struct OrderRecord {
        std::string description; ///< Description of the order.
        int remaining;           ///< Number of pizzas remaining to be cooked.
    };

    std::queue<pid_t>_pendingKitchens; ///< Kitchens that are pending (just spawned).
    std::map<pid_t, Internal::Process> _pendingPrecesses; ///< Process objects for pending kitchens.
    std::map<pid_t, KitchenHandle> kitchens; ///< Active kitchens mapped by their PID.
    std::map<uint32_t, OrderRecord> _orderTracking; ///< Tracking of orders by ID.
    uint32_t _nextOrderId = 1; ///< Next ID to assign to an order.
    Internal::Socket _serverSocket; ///< Server socket for IPC with kitchens.
    Internal::Poller poller; ///< Poller for handling multiple IPC channels.
    std::string commandBuffer; ///< Buffer for reading CLI commands.
    std::queue<Message> _pendingOrders; ///< Orders waiting to be assigned to a kitchen.

    std::map<std::string, std::function<void (Reception&, std::string)>> _map; ///< Map of CLI commands.

    std::map<KitchenKey, std::function<void (int, pid_t)>> _handlers; ///< Handlers for kitchen events.

public:
    /**
     * @brief Construct a new Reception object.
     * 
     * @param ac Argument count from main.
     * @param av Argument vector from main.
     * @throw PlazzaException if arguments are incorrect.
     */
    Reception(int ac, char **av);

    /**
     * @brief Destroy the Reception object.
     */
    ~Reception();

    /**
     * @brief Start the Command Line Interface loop.
     */
    void startCli();

    /**
     * @brief Read an incoming command from the CLI.
     */
    void readIncomingCommand();

    /**
     * @brief Accept a connection from a newly spawned kitchen.
     */
    void acceptNewKitchen();

    /**
     * @brief Read messages from a specific kitchen.
     * 
     * @param pid The PID of the kitchen.
     */
    void readKitchenMessages(int pid);

    /**
     * @brief Find the kitchen with the least load.
     * 
     * @return KitchenHandle* Pointer to the least loaded kitchen handle, or nullptr if all are full.
     */
    KitchenHandle* leastLoadedKitchen();

    /**
     * @brief Spawn a new kitchen process.
     */
    void spawnKitchen();

    /**
     * @brief Get the next command from the command buffer.
     * 
     * @return std::optional<std::string> The command if found, std::nullopt otherwise.
     */
    std::optional<std::string> nextCommand();

    /**
     * @brief Handle a command received from the CLI.
     * 
     * @param command The command string.
     */
    void handlePlazzaCommand(const std::string& command);

    /**
     * @brief Remove a kitchen that has closed.
     * 
     * @param fd The file descriptor of the kitchen's IPC.
     * @param pid The PID of the kitchen.
     */
    void removeClosedKitchen(int fd, pid_t pid);

    /**
     * @brief Enqueue a pizza order to be sent to a kitchen.
     * 
     * @param order The pizza order.
     */
    void enqueueOrder(const Message& order);

    /**
     * @brief Broadcast a status request to all kitchens.
     */
    void broadcastStatus();

    /**
     * @brief Create a new order and track it.
     * 
     * @param description The description of the order.
     * @param count The number of pizzas in the order.
     * @return uint32_t The assigned order ID.
     */
    uint32_t createOrder(const std::string& description, int count);
};