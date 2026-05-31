/**
 * @file Utils.hpp
 * @brief Header file for the Utils class containing static utility methods.
 */

#pragma once
#include <string>
#include "Pizzas.hpp"

/**
 * @class Utils
 * @brief Static utility class for common operations.
 */
class Utils {
public:
    /**
     * @brief Trim whitespace from both ends of a string.
     * 
     * @param to_clean The string to trim.
     * @return std::string The trimmed string.
     */
    static std::string trim(const std::string& to_clean);

    /**
     * @brief Parse a pizza order from a string.
     * 
     * @param order The string containing the pizza order.
     * @return Message The parsed pizza order as a Message struct.
     */
    static Message parsePizzaOrder(const std::string &order);
};