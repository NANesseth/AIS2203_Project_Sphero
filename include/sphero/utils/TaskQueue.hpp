#ifndef AIS2203_PROJECT_SPHERO_TASKQUEUE_HPP
#define AIS2203_PROJECT_SPHERO_TASKQUEUE_HPP

#include <functional>
#include <iostream>
#include <mutex>
#include <queue>

class Simulation {

public:
    using Task = std::function<void()>;

    void step(double stepSize) {

        {
            std::unique_lock<std::mutex> lck(m_);
            while (!queue_.empty()) {
                Task &task = queue_.front();
                lck.unlock();

                std::cout << "Invoking task @ t=" << t_ << std::endl;
                task();

                lck.lock();
                queue_.pop();
                // not unlocking here because we are looping and the next invocation expects a locked mutex
            }
        }

        // simulate something
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        t_ += stepSize;

        std::cout << "Simulation stepped @ t=" << t_ << std::endl;
    }

    void invokeLater(Task task) {

        std::lock_guard<std::mutex> lck(m_);
        queue_.emplace(std::move(task));
    }

    double getTime() const {
        return t_;
    }

private:
    double t_{0};
    std::mutex m_;
    std::queue<Task> queue_;
};


#endif//AIS2203_PROJECT_SPHERO_TASKQUEUE_HPP