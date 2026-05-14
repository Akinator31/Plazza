#include <exception>
#include <iostream>
#include <csignal>

#include "Reception/Reception.hpp"

void signalHandler(const int signum) {
    signalReceived = signum;
}

int main(const int ac, char **av) {
    std::signal(SIGINT, signalHandler);

    try {
        Reception reception(ac, av);

        reception.startCli();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}