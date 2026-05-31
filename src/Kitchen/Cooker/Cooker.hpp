/**
 * @file Cooker.hpp
 * @brief Header file for the Cooker class.
 */

#pragma once
#include <memory>
#include "Internal/Thread/Thread.hpp"

class Kitchen;

/**
 * @class Cooker
 * @brief Represents a cook in a kitchen, running in its own thread.
 */
class Cooker {
    Kitchen *_kitchen; ///< Pointer to the kitchen this cook belongs to.
    std::unique_ptr<Internal::Thread> _thread; ///< The thread the cook runs in.

public:
    /**
     * @brief Construct a new Cooker object.
     * 
     * @param kitchen Pointer to the kitchen.
     */
    Cooker(Kitchen *kitchen);

    /**
     * @brief Start the cook's thread.
     */
    void start();

    /**
     * @brief The cook's main execution loop.
     */
    void run();
};
