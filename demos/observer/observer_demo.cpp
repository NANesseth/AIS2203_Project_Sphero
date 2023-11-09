#include "sphero/sphero.hpp"

#include <iostream>
#include <chrono>

// Extending MyObserver to do something more specific
class MyConcreteObserver : public Observer {
public:
    void onChange() override {
        std::cout << "Observer notified of change, one second has passed!" << std::endl;
    }
};

int main() {
    MyThreadedClass inputListener;

    // Create observer instance
    MyConcreteObserver observer_1;

    // Add observer to our threaded class
    inputListener.addObserver(&observer_1);

    // Start the thread
    inputListener.start();

    // For demonstration purposes, we'll simulate some time passing
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Optionally, ensure the thread has stopped.
    if (inputListener.isRunning()) {
        inputListener.stop();
    }

    std::cout << "Loop finished" << std::endl;

    // Stop the thread
    // inputListener.stop();

    // Clean-up: Remove observer if necessary
    // (Not strictly required in this example since we are stopping the thread and ending the program)
    inputListener.removeObserver(&observer_1);

    return 0;
}
