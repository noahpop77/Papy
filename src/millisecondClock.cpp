#include <chrono>

#include "millisecondClock.hpp"

// Start the clock by recording the current time
void millisecondClock::start() {
    initialStart = std::chrono::high_resolution_clock::now();
    currentStart = std::chrono::high_resolution_clock::now();
}

// Get the elapsed time in milliseconds since the clock was started
long long millisecondClock::perSecondCheck() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - currentStart);
    return duration.count();
}

// Get the elapsed time in milliseconds since the clock was started
long long millisecondClock::elapsedMilliseconds() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - initialStart);
    return duration.count();
}

// Reset the clock to start counting from the current time
// Used in calculating packets per second
void millisecondClock::resetClock() {
    currentStart = std::chrono::high_resolution_clock::now();
}
