#include <csignal>
#include <iostream>
#include <thread>
#include <vector>

#include "cliHelper.hpp"
#include "threadWorks.hpp"

int main(int argc, char* argv[]) {
    // Runtime configuration variables declared and initialized with default values
    int numThreads = 1, payloadCount = 0, rateLimit = 0, ramp = 0, spike = 0;
    bool verbose = false;
    std::string target, endpoint, payload, parameter;

    // Process CLI Arguments, passed by reference
    cliHelper::parseArguments(argc, argv, numThreads, payloadCount, rateLimit, ramp, spike, target, endpoint, verbose, payload, parameter);

    // Prints the large PAPYRUS banner at the start of the program
    cliHelper::printBanner(target, endpoint, numThreads, rateLimit);

    // Signal handler controlling ctl + c program halting
    signal(SIGINT, threadWorks::signalHandler);
    std::cout << "Program is running. Press Ctrl+C to stop.\n";

    // Launch threads
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&threadWorks::runWorkerThread, target, endpoint, verbose, payloadCount, rateLimit, ramp, spike, payload, parameter);
    }

    // Wait for threads to finish
    for (auto& thread : threads) {
        if (thread.joinable()) thread.join();
    }

    std::cout << "\nProgram has stopped.\n";
    return 0;
}
