#include "cliHelper.hpp"

#include <climits>
#include <cmath>
#include <iostream>
#include <string>

// Helper function to validate and parse integer arguments
int cliHelper::parseIntArg(const char* arg, const std::string& flagName) {
    try {
        return std::stoi(arg);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Error: Invalid number for " + flagName + ". Must be a positive integer.");
        std::abort();
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Error: Number out of range for " + flagName + ".");
        std::abort();
    }
}

// Runtime banner for program execution
void cliHelper::printBanner (std::string& target, std::string& endpoint, int& numThreads, int& rateLimit) {
    std::cout << "==========================================" << std::endl;
    std::cout << " ____   _    ______   ______  _   _ ____  " << std::endl;
    std::cout << "|  _ \\ / \\  |  _ \\ \\ / /  _ \\| | | / ___| " << std::endl;
    std::cout << "| |_) / _ \\ | |_) \\ V /| |_) | | | \\___ \\ " << std::endl;
    std::cout << "|  __/ ___ \\|  __/ | | |  _ <| |_| |___) |" << std::endl;
    std::cout << "|_| /_/   \\_\\_|    |_| |_| \\_\\\\___/|____/ " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Threads: " << numThreads << std::endl;
    std::cout << "Target: " << target << std::endl;
    std::cout << "Endpoint: " << endpoint << std::endl;
    std::cout << "Rate Limit: " << rateLimit << "ms" << std::endl;
    std::cout <<  "==========================================" << std::endl;
}

// Helper function for parsing command-line arguments
void cliHelper::parseArguments(int argc, char* argv[], int& numThreads, int& payloadCount, int& rateLimit, int& ramp, int& spike, std::string& target, std::string& endpoint, bool& verbose, std::string& payload, std::string& parameter) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        try {
            if (arg == "--help" || arg == "-h") {
                cliHelper::displayHelp();
                exit(0);
            } else if (arg == "--threads" || arg == "-th") {
                numThreads = cliHelper::parseIntArg(argv[++i], "threads");
            } else if (arg == "--target" || arg == "-ta") {
                target = argv[++i];
                if (target.empty()) throw std::runtime_error("Error: Target value cannot be empty.");
            } else if (arg == "--count" || arg == "-c") {
                payloadCount = cliHelper::parseIntArg(argv[++i], "count");
            } else if (arg == "--rate" || arg == "-r") {
                rateLimit = cliHelper::parseIntArg(argv[++i], "rate");
            } else if (arg == "--ramp" || arg == "-ra") {
                ramp = cliHelper::parseIntArg(argv[++i], "ramp");
            } else if (arg == "--spike" || arg == "-s") {
                spike = cliHelper::parseIntArg(argv[++i], "spike");
            } else if (arg == "--endpoint" || arg == "-e") {
                endpoint = argv[++i];
            } else if (arg == "--parameter" || arg == "-pa") {
                parameter = argv[++i];
            } else if (arg == "--payload" || arg == "-p") {
                payload = argv[++i];
                if (payload.empty()) throw std::runtime_error("Error: Payload value cannot be empty.");
            } else if (arg == "--verbose" || arg == "-v") {
                verbose = true;
            } else {
                throw std::runtime_error("Error: Unknown option '" + arg + "'. Use --help for usage.");
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            std::abort();
            std::exit(EXIT_FAILURE);
        }
    }

    if (target.empty()) throw std::runtime_error("Error: Target URL is required.");
}

// Function to display help message
void cliHelper::displayHelp() {
    std::cout << "Usage: ./program [options] {value}\n"
            << "Options:\n"
            << "  -h, --help                Show this help message\n"
            << "  -th, --threads [num]      Number of threads to run (default: 1)\n"
            << "  -ta, --target [url]       Target URL ex. \"http://mtrack.com\"\n"
            << "  -e, --endpoint [path]     API endpoint ex. \"/addMatch\"\n"
            << "  -pa, --parameter [string]  String of parameters that will be appeneded to address.\n"
            << "  -c, --count [num]         Number of payloads to send\n"
            << "  -r, --rate [ms]           Rate limit in milliseconds between sent payloads\n"
            << "  -v, --verbose             Enable verbose output\n"
            << "  -s, --spike [ms]          Each thread will send data in random spikes \n"
            << "  -ra, --ramp [ms]           Each thread will send data gradually faster\n"
            << "  -p, --payload [filepath]  Option to specify file path to .JSON payload \n"
            << "\nExample: \n"
            << "./papy --threads 4 --target \"http://127.0.0.1\" --endpoint \"/printJson\" --payload ocean --verbose --rate 2000\n";
}

// Helper function to check if a number is a valid integer or not and returns
// 1 if it is
bool cliHelper::isValidInt(double num) {
    return std::floor(num) == num && num >= INT_MIN && num <= INT_MAX;
}
