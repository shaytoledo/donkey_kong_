#pragma once
#include <chrono>
#include <iostream>
#include "Utils.h"
#include "Screen.h"

class Time {

    // Time point when the timer was started
    std::chrono::time_point<std::chrono::steady_clock> start;

    // Variable to track the time when the timer is paused
    std::chrono::time_point<std::chrono::steady_clock> pauseTime;

    // Flag to check if the timer is paused
    bool isPaused;
    float speedMultiplier = 1;
	Screen& screen;

public:
    // Constructor to initialize the timer and set it to not paused
    Time(float _speedMultiplier, Screen& _screen) : start(std::chrono::steady_clock::now()), isPaused(false), speedMultiplier(_speedMultiplier), screen(_screen) {}

    // Get speedMultiplier
	float getSpeedMultiplier() const {
		return speedMultiplier;
	}

    // Function to return the elapsed time (in seconds) since the timer started
    double elapsed() const;

    // Function to display the elapsed time 
    void showElapsedTime() const;

    // Function to pause the timer
    void pause();

    // Function to resume the timer
    void resume();

    // Function to reset the timer - sets a new start time and clears the paused state
    void reset() {
        start = std::chrono::steady_clock::now();
        isPaused = false; // Reset the paused state
    }

    // Function to return the elapsed time in seconds as an integer
    int getTimeBySeconds() const {
        return static_cast<int>(elapsed());
    }
};
