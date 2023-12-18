#include "sphero/sphero.hpp"

#include <iostream>

// This is a demo of the TaskManager class
// However, we have moved away from this implementation in the main project, and it is only here for our own reference

void task_1(){
    // Simulate work
    std::cout << "Task 1: start" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "Task 1: end" << std::endl;
}

void task_2(){
    // Simulate work
    std::cout << "Task 2: start" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Task 2: end" << std::endl;
}


int main(){
    TaskManager tm;
    std::cout << "Hello TaskManager Demo!" << std::endl;

    std::thread t_1([&] {
        tm.invokeLater([&] {
            task_1();
        });
    });

    std::thread t_2([&] {
        tm.invokeLater([&] {
            task_1();
        });
    });

    double dt = 0.01;// 100hz
    while (tm.getTime() < 0.2) {
        tm.step(dt);
    }

    if (t_1.joinable()) t_1.join();
    if (t_2.joinable()) t_2.join();


    return 0;
}