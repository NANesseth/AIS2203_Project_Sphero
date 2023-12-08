#ifndef AIS2203_PROJECT_SPHERO_OBSERVER_HPP
#define AIS2203_PROJECT_SPHERO_OBSERVER_HPP

#include <thread>
#include <atomic>
#include <vector>
#include <mutex>

//class Observer {
//public:
//    virtual void onChange() = 0;
//    virtual ~Observer() = default;
//};
//
//class SpecificObserver: public Observer {
//public:
//    void onChange() override {
//        // do something when change is observed
//    }
//};
//
//class ThreadedTimerClass {
//public:
//    ThreadedTimerClass() : started_(false) {}
//
//    void start();
//    void stop();
//
//    void addObserver(Observer *observer);
//    void removeObserver(Observer *observer);
//
//private:
//    std::thread t_;
//    std::atomic_bool stop_{false};
//    bool started_;
//    std::mutex start_stop_mutex_;
//    std::vector<Observer *> observers;
//    std::mutex observers_mutex_;
//
//    void runTimer();
//
//    void notifyObservers();
//
//public:
//    bool hasObservers();
//    bool isRunning() const;
//};

#endif//AIS2203_PROJECT_SPHERO_OBSERVER_HPP
