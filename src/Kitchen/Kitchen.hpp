/**
 * @file Kitchen.hpp
 * @brief Header file for the Kitchen and related classes.
 */

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

/**
 * @class KitchenHandle
 * @brief A handle for a kitchen process, used by the Reception to communicate with the kitchen.
 */
class KitchenHandle {
    Internal::Process _process; ///< The process of the kitchen.
    int _currentLoad = 0;       ///< The current load (number of pizzas) of the kitchen.

public:
    std::optional<Message> pendingMessages; ///< Messages waiting to be sent to the kitchen.
    Internal::IPC ipc;                      ///< The IPC object for communication with the kitchen.

    /**
     * @brief Construct a new KitchenHandle object.
     * 
     * @param proc The process object.
     * @param socket The socket for IPC.
     */
    KitchenHandle(Internal::Process &&proc, Internal::Socket &&socket);

    /**
     * @brief Check if the kitchen is saturated.
     * 
     * @param nb_cooks The number of cooks in the kitchen.
     * @return true if the kitchen is saturated.
     * @return false otherwise.
     */
    [[nodiscard]] bool isSaturated(int nb_cooks) const;

    /**
     * @brief Get the current load of the kitchen.
     * 
     * @return int The current load.
     */
    [[nodiscard]] int load() const;

    /**
     * @brief Send a pizza order to the kitchen.
     * 
     * @param order The pizza order.
     */
    void sendOrder(const Message &order);

    /**
     * @brief Notify that a pizza is done.
     */
    void notifyDone();

    /**
     * @brief Close the kitchen.
     */
    void close() const;

    /**
     * @brief Get the IPC file descriptor.
     * 
     * @return int The file descriptor.
     */
    [[nodiscard]] int ipcFd() const;

    /**
     * @brief Get the process ID of the kitchen.
     * 
     * @return pid_t The PID.
     */
    [[nodiscard]] pid_t pid() const;
};

/**
 * @class Kitchen
 * @brief Represents a kitchen process that manages cooks and pizza preparation.
 */
class Kitchen {
public:
    Internal::IPC _ipc; ///< IPC object for communicating with the Reception.

    /**
     * @brief Construct a new Kitchen object.
     * 
     * @param socketPath The path to the Unix socket for IPC.
     * @param nbCooks The number of cooks in the kitchen.
     * @param restockTimer The interval for restocking ingredients.
     * @param multiplier The cooking time multiplier.
     */
    Kitchen(const std::string &socketPath, int nbCooks, int restockTimer, float multiplier);

    /**
     * @brief Start the kitchen execution loop.
     */
    void run();

    /**
     * @brief Handle a command received from the Reception.
     * 
     * @param order The pizza order command.
     */
    void handleReceptionCommand(Message &order);

    /**
     * @brief The loop for restocking ingredients.
     */
    void restockLoop();

    /**
     * @brief Check if the kitchen should close due to inactivity.
     * 
     * @return true if the kitchen should close.
     * @return false otherwise.
     */
    bool shouldClose();

    std::vector<std::unique_ptr<Cooker>> _cookers; ///< The list of cooks.
    std::vector<PizzaRecipe> _pizzaQueue;         ///< The queue of pizzas to be prepared.
    Stock _stock;                                  ///< The current stock of ingredients.
    Internal::Mutex _stockMutex;                   ///< Mutex for thread-safe access to stock.
    Internal::Mutex _pizzaQueueMutex;              ///< Mutex for thread-safe access to pizza queue.
    Internal::Semaphore _pizzaQueueSemaphore;      ///< Semaphore for pizza queue.
    Internal::Mutex _ipcMutex;                     ///< Mutex for thread-safe access to IPC.
    Internal::Mutex _busyMutex;                    ///< Mutex for thread-safe access to busy cooks count.
    Internal::Mutex _activityMutex;                ///< Mutex for thread-safe access to last activity timestamp.
    int _busyCooks = 0;                            ///< Number of cooks currently busy.
    int _nbCooks;                                  ///< Total number of cooks.
    int _restockTimer;                             ///< Restock timer interval.
    float _multiplier;                             ///< Cooking time multiplier.
    std::chrono::steady_clock::time_point _lastActivity; ///< Timestamp of the last activity.
};

/**
 * @struct KitchenKey
 * @brief A key used to identify a kitchen in a map.
 */
struct KitchenKey {
    int    fd;  ///< File descriptor of the kitchen's IPC.
    pid_t  pid; ///< Process ID of the kitchen.

    /**
     * @brief Comparison operator for using KitchenKey in maps.
     * 
     * @param other The other KitchenKey to compare with.
     * @return true if this key is less than the other.
     * @return false otherwise.
     */
    bool operator<(const KitchenKey& other) const {
        if (fd != other.fd)
            return fd < other.fd;
        return pid < other.pid;
    }
};
