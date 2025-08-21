#pragma once

#include <string>

class cliHelper {
public:
    static int parseIntArg(const char* arg, const std::string& flagName);

    static void printBanner(std::string& target, std::string& endpoint, int& numThreads, int& rateLimit);

    static void parseArguments(int argc, char* argv[], int& numThreads, int& payloadCount, int& rateLimit,
                                int& ramp, int& spike, std::string& target, std::string& endpoint,
                                bool& verbose, std::string& payload,
                                std::string& parameter, std::string& bearer, std::string& authorization);

    static void displayHelp();

    static bool isValidInt(double num);
    
};
