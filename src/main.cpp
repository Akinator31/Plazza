#include <exception>
#include <iostream>

#include "Reception/Reception.hpp"

int main(const int ac, char **av) {
    try {
        Reception reception(ac, av);

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}