#include "sphero/sphero.hpp"

#include <iostream>

int main(){
    std::cout << "Hello Task Queue!" << std::endl;

    Simulation sim;

    std::thread t([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        sim.invokeLater([&] {
            std::cout << "First task" << std::endl;

            sim.invokeLater([&] {
                std::cout << "Second task" << std::endl;
            });
        });
    });

    double dt = 0.01;// 100hz
    while (sim.getTime() < 0.2) {
        sim.step(dt);
    }

    if (t.joinable()) t.join();

    return 0;
}