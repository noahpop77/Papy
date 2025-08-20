#pragma once

#include <atomic>
#include <mutex>
#include <string>

class apiClient;
class millisecondClock;

class threadWorks {
public:
    static std::atomic<bool> isProgramActive;            // Shared across threads and signal handler
    static std::mutex consoleMutex;                      // Used in the signal handler
    static std::atomic<int> totalPayloadsSent;           // Shared across threads
    static std::atomic<int> totalPayloadsSuccessful;     // Shared across threads
    static std::atomic<int> packetsPerSecond;            // Shared across threads

    // Signal handler to stop the program
    static void signalHandler(int signal);

    // Function to send requests and optionally log verbose output
    static void sendRequest(apiClient& client, bool verbose, std::string payload, millisecondClock& clock, std::string bearer, std::string authorization);

    // Worker thread function that sends requests based on the provided parameters
    static void runWorkerThread(const std::string& targetURL, const std::string& endpoint, bool verbose, int payloadCount, int rateLimit, int ramp, int spike, std::string payload, std::string parameter, std::string bearer, std::string authorization);

    static std::string gzip_compress(const std::string &data);
};
