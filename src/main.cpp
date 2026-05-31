/**
 * @file main.cpp
 * @brief Entry point for the Plazza project.
 */

#include <exception>
#include <iostream>
#include <csignal>
#include <sys/wait.h>

#include "Reception/Reception.hpp"

/**
 * @brief Global signal handler for catching signals like SIGINT.
 * 
 * @param signum The signal number received.
 */
void signalHandler(const int signum) {
    signalReceived = signum;
}

/**
 * @brief SIGCHLD handler to reap dead kitchen processes and avoid zombies.
 * 
 * @param int Unused signal number.
 */
void waitDeadKitchens(int) {
    while (waitpid(-1, nullptr, WNOHANG) > 0);
}

/**
 * @brief Main function of the Plazza project.
 * 
 * @param ac Argument count.
 * @param av Argument vector.
 * @return int 0 on success, 84 on failure.
 */
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