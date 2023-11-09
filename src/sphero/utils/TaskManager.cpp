#include "sphero/utils/TaskManager.hpp"


void TaskManager::invokeLater(Task task) {

    std::lock_guard<std::mutex> lck(m_);
    queue_.emplace(std::move(task));
}

void TaskManager::step(double stepSize) {

    std::unique_lock<std::mutex> lck(m_);
    while (!queue_.empty()) {
        Task &task = queue_.front();
        lck.unlock();

        std::cout << "Processing task @ t=" << t_ << std::endl;
        task();

        lck.lock();
        queue_.pop();
        // not unlocking here because we are looping and the next invocation expects a locked mutex
    }
    lck.unlock();

    // simulate something
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    t_ += stepSize;

    std::cout << "TaskManager stepped @ t=" << t_ << std::endl;
}