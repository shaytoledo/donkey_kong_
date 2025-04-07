#include "Time.h"

// Function to return the elapsed time (in seconds) since the timer started
double Time::elapsed() const {
    if (isPaused) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(pauseTime - start).count() * getSpeedMultiplier() / 1000.0;
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() * getSpeedMultiplier() / 1000.0;
}

// Function to display the elapsed time 
void Time::showElapsedTime() const {
    double totalSeconds = elapsed();
    int minutes = static_cast<int>(totalSeconds) / 60;
    int seconds = static_cast<int>(totalSeconds) % 60;
    gotoxy({ 72, 1 });
    std::string toPrint = std::to_string(minutes) + " : " + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    screen.coloredStrPrint(toPrint);
}

// Function to pause the timer
void Time::pause() {
    if (!isPaused) {
        pauseTime = std::chrono::steady_clock::now();
        isPaused = true;
    }
}

// Function to resume the timer
void Time::resume() {
    if (isPaused) {
        auto pausedDuration =
            std::chrono::steady_clock::now() - pauseTime;
        start += pausedDuration;
        isPaused = false;
    }
}