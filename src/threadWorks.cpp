#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

// project dependencies
#include "dependencies/json.hpp"
// https://github.com/madler/zlib
#include "dependencies/gzip/zlib.h"

#include "apiClient.hpp"
#include "matchBuilder.hpp"
#include "millisecondClock.hpp"
#include "myRandom.hpp"
#include "oceanBuilder.hpp"
#include "threadWorks.hpp"

using json = nlohmann::json;


// Static member initialization
std::atomic<bool> threadWorks::isProgramActive(true);
std::mutex threadWorks::consoleMutex;
std::atomic<int> threadWorks::totalPayloadsSent(0);
std::atomic<int> threadWorks::totalPayloadsSuccessful(0);
std::atomic<int> threadWorks::packetsPerSecond(0);
std::mutex errorMutex;

// Static signal handler function
void threadWorks::signalHandler(int signal) {
    std::lock_guard<std::mutex> guard(consoleMutex);
    std::cout << "\nStopping program..." << std::endl;
    isProgramActive = false;
}
// Function to compress data using zlib
std::string threadWorks::gzip_compress(const std::string &data) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw std::runtime_error("Failed to initialize zlib for compression");
    }

    zs.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(data.data()));
    zs.avail_in = data.size();

    char buffer[4096];
    std::string compressed;

    do {
        zs.next_out = reinterpret_cast<Bytef *>(buffer);
        zs.avail_out = sizeof(buffer);

        if (deflate(&zs, Z_FINISH) == Z_STREAM_ERROR) {
            deflateEnd(&zs);
            throw std::runtime_error("Compression failed");
        }

        compressed.append(buffer, sizeof(buffer) - zs.avail_out);
    } while (zs.avail_out == 0);

    deflateEnd(&zs);
    return compressed;
}

void threadWorks::sendRequest(apiClient& client, bool verbose, std::string payload, millisecondClock& clock) {
    std::transform(payload.begin(), payload.end(), payload.begin(), ::tolower);
    std::string response;


    // This is where you add additional options for handling different preset payloads
    // You add another option to the conditional and however you want to handle it
    if (payload.empty()) {
        response = client.sendGETRequest();
    } else if (payload == "lol") {
        nlohmann::json lolPayload = matchBuilder::randomMatch();
        std::string compressedLolPayload = gzip_compress(lolPayload.dump());
        client.setPayload(compressedLolPayload);
        response = client.sendPOSTRequest();
    } else if (payload == "ocean") {
        client.setPayload(oceanBuilder::randomOcean().dump());
        response = client.sendPOSTRequest();
    } else {

        std::ifstream f(payload);
        json jsonPayload;

        // Catches payload not found errors essentially. If it can not parse it
        // then it was mostly not found or malformed.
        try {
            json jsonPayload = json::parse(f);
        } catch (const nlohmann::json_abi_v3_11_3::detail::parse_error& e) {
            std::lock_guard<std::mutex> lock(errorMutex);
            std::cerr << "Parsing Error: Payload file not found or incorrect JSON" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        client.setPayload(jsonPayload.dump());
        response = client.sendPOSTRequest();

    }

    if (verbose) {
        std::cout << "Response: " << response << std::endl;
    } else {
        

        // Mutex guarded packets per second calculations
        std::lock_guard<std::mutex> guard(consoleMutex);

        // You can run it for two days before you get an integer overflow LOL
        int elapsedSec = clock.elapsedMilliseconds() / 1000;
        if (elapsedSec > 0) {
            packetsPerSecond = totalPayloadsSent / elapsedSec;
        } else {
            packetsPerSecond = 0; // Default value if elapsed time is too short
        }

        if (response == "200") {
            totalPayloadsSuccessful++;
        }

        // Having totalPayloadsSent++ at the top of the namespace caused the 
        // failed stat to blip
        totalPayloadsSent++;

        // Limit how frequently we display the update message
        static auto last_update_time = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_time);

        // 20 Hz refresh rate = less than screen, almost max human eye
        constexpr unsigned int DELAY_MS = 25;
        if (elapsed.count() < DELAY_MS) {
            // It's been less than the delay time, so skip displaying an update.
            return;
        }

        last_update_time = now;

        // Print updated info on the same line
        std::cout << "\rTotal Sent: " << totalPayloadsSent
                  << " | Successful: " << totalPayloadsSuccessful
                  << " | Failed: " << (totalPayloadsSent - totalPayloadsSuccessful)
                  << " | Packets/s: " << packetsPerSecond
                  << " | Elapsed Time: " << clock.elapsedMilliseconds()
		  << "     "  // clear a few characters past the end
                  << std::flush;
    }
}

// Orchestrates the sending of requests and main loop for program
void threadWorks::runWorkerThread(const std::string& targetURL, const std::string& endpoint, bool verbose, int payloadCount, int rateLimit, int ramp, int spike, std::string payload, std::string parameter) {
    millisecondClock clock;
    apiClient client(targetURL);
    client.setEndpoint(endpoint);
    client.setParameter(parameter);

    clock.start();
    while (isProgramActive) {
        sendRequest(client, verbose, payload, clock);

        if (payloadCount > 0 && --payloadCount == 0) break;

        if (rateLimit) {
            std::this_thread::sleep_for(std::chrono::milliseconds(rateLimit));
        }
        if (ramp > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(ramp));
            ramp = std::max(ramp / 2, 1);
        } else if (spike > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(myRandom::generateRandomInt(0, spike)));
        }
    }
}
