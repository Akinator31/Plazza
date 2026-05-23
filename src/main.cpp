#include <exception>
#include <iostream>
#include <csignal>
#include <sys/wait.h>

#include "Reception/Reception.hpp"

void signalHandler(const int signum) {
    signalReceived = signum;
}

void waitDeadKitchens(int) {
    while (waitpid(-1, nullptr, WNOHANG) > 0);
}

int main(const int ac, char **av) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGCHLD, waitDeadKitchens);

    try {
        Reception reception(ac, av);

        reception.startCli();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}