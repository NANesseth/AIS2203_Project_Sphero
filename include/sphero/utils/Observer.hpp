#ifndef AIS2203_PROJECT_SPHERO_OBSERVER_HPP
#define AIS2203_PROJECT_SPHERO_OBSERVER_HPP

#include <thread>
#include <atomic>
#include <vector>
#include <mutex>

class Observer {
public:
    virtual void onChange() = 0;
    virtual ~Observer() = default;
};

class MyObserver : public Observer {
public:
    void onChange() override {
        // do something when change is observed
    }
};

class MyThreadedClass {
public:
    MyThreadedClass() : started_(false) {}

    void start() {
        std::lock_guard<std::mutex> lock(start_stop_mutex_);
        if (!started_) {
            started_ = true;
            stop_ = false;
            t_ = std::thread(&MyThreadedClass::run, this);
        }
    }

    void stop() {
        std::lock_guard<std::mutex> lock(start_stop_mutex_);
        if (started_) {
            stop_ = true;
            if (t_.joinable()) {
                t_.join();
            }
            started_ = false;
        }
    }

    void addObserver(Observer *observer) {
        std::lock_guard<std::mutex> lock(observers_mutex_);
        observers.emplace_back(observer);
    }

    void removeObserver(Observer *observer) {
        std::lock_guard<std::mutex> lock(observers_mutex_);
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

private:
    std::thread t_;
    std::atomic_bool stop_{false};
    bool started_;
    std::mutex start_stop_mutex_;
    std::vector<Observer *> observers;
    std::mutex observers_mutex_;

    void run() {
        while (!stop_) {
            // Check for controller input or perform regular actions
            // ....

            // When some change happens:
            notifyObservers();
        }
    }

    void notifyObservers() {
        std::vector<Observer *> observers_copy;
        {
            std::lock_guard<std::mutex> lock(observers_mutex_);
            observers_copy = observers; // Make a copy to avoid deadlock
        }

        for (auto *observer : observers_copy) {
            observer->onChange();
        }
    }
};

#endif//AIS2203_PROJECT_SPHERO_OBSERVER_HPP
