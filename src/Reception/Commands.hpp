/**
 * @file Commands.hpp
 * @brief Header file for the Commands class containing CLI command handlers.
 */

#pragma once
#include "Reception.hpp"

/**
 * @class Commands
 * @brief Static class containing handlers for CLI commands.
 */
class Commands {

public:
    /**
     * @brief Handle the 'status' command.
     * 
     * @param reception Reference to the Reception object.
     */
    static void status(Reception &reception, const std::string&);

    /**
     * @brief Handle the 'order' command.
     * 
     * @param reception Reference to the Reception object.
     * @param order The order string to be processed.
     */
    static void order(Reception &reception, const std::string& order);
};