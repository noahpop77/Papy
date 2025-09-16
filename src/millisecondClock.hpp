#pragma once

#include <chrono>

class millisecondClock {
public:
    // Start the clock by recording the current time
    void start();

    // Get the elapsed time in milliseconds since the clock was started
    long long perSecondCheck() const;

    // Get the elapsed time in milliseconds since the clock was started
    long long elapsedMilliseconds() const;

    // Reset the clock to the current time
    void resetClock();

private:
    std::chrono::high_resolution_clock::time_point initialStart; // Store the start time
    std::chrono::high_resolution_clock::time_point currentStart;
};
