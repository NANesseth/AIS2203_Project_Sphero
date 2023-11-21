#include "sphero/utils/Observer.hpp"
#include <chrono>
#include <iostream>

// Class ThreadedTimerClass
// =================================================================================================
// Public methods

void ThreadedTimerClass::start() {
    std::lock_guard<std::mutex> lock(start_stop_mutex_);
    if (!started_) {
        started_ = true;
        stop_ = false;
        t_ = std::thread(&ThreadedTimerClass::runTimer, this);
    }
}

void ThreadedTimerClass::stop() {
    std::lock_guard<std::mutex> lock(start_stop_mutex_);
    if (started_) {
        stop_ = true;
        if (t_.joinable()) {
            t_.join();
        }
        started_ = false;
    }
}

void ThreadedTimerClass::addObserver(Observer *observer) {
    std::lock_guard<std::mutex> lock(observers_mutex_);
    observers.emplace_back(observer);
}

void ThreadedTimerClass::removeObserver(Observer *observer) {
    std::lock_guard<std::mutex> lock(observers_mutex_);
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

bool ThreadedTimerClass::hasObservers(){
    std::lock_guard<std::mutex> lock(observers_mutex_);
    return !observers.empty();
}

// Method to check if the thread is running
bool ThreadedTimerClass::isRunning() const {
    return started_;
}

// =================================================================================================
// Private methods

void ThreadedTimerClass::runTimer() {
    auto startTime = std::chrono::high_resolution_clock::now();
    auto lastNotificationTime = startTime;
    long long secondsPassed = 0;

    while (!stop_) {
        // Check for controller input or perform regular actions

        auto current_time = std::chrono::high_resolution_clock::now();

        // Notify observers every sec ond
        if (current_time - lastNotificationTime >= std::chrono::seconds(1)) {
            lastNotificationTime = current_time;  // Reset the last notification time

            secondsPassed = std::chrono::duration_cast<std::chrono::seconds>(current_time - startTime).count();

            std::cout << "Time passed: " << secondsPassed << " seconds" << std::endl;

            notifyObservers();
        }

        // Sleep for a short duration to prevent tight looping
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void ThreadedTimerClass::notifyObservers() {
    std::vector<Observer *> observers_copy;
    {
        std::lock_guard<std::mutex> lock(observers_mutex_);
        observers_copy = observers; // Make a copy to avoid deadlock
    }

    for (auto *observer : observers_copy) {
        observer->onChange();
    }
}