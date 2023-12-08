#ifndef AIS2203_PROJECT_SPHERO_TASKMANAGER_HPP
#define AIS2203_PROJECT_SPHERO_TASKMANAGER_HPP

#include <functional>
#include <iostream>
#include <mutex>
#include <queue>

class TaskManager {

public:
    using Task = std::function<void()>;

    void step(double stepSize);

    void invokeLater(Task task);

    double getTime() const {
        return t_;
    }

private:
    double t_{0};
    std::mutex m_;
    std::queue<Task> queue_;
};


#endif//AIS2203_PROJECT_SPHERO_TASKMANAGER_HPP