#include "sphero/sphero.hpp"

#include <iostream>
#include <chrono>

// This demo code no longer shows a working part of the project, and is to be deleted

// Extending SpecificObserver to do something more specific
// One implementation of SpecificObserver as shown in Observer.hpp
class TimerObserver: public Observer {
public:
    void onChange() override {
        std::cout << "Observer notified of change, one second has passed!" << std::endl;
    }
};

int main() {
    ThreadedTimerClass inputListener;

    // Create observer instance
    TimerObserver observer_1;

    // Add observer to our threaded class
    inputListener.addObserver(&observer_1);

    // Start the thread
    inputListener.start();

    // Simulating work
    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "Loop finished" << std::endl;

    // Clean-up: Remove observer if necessary
    // (Not strictly required in this example since we are stopping the thread and ending the program)
    inputListener.removeObserver(&observer_1);

    // Optionally, ensure the thread has stopped.
    if (inputListener.isRunning()) {
        inputListener.stop();
    }

    return 0;
}
