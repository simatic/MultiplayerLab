#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class ThreadSafeQueue
{
private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
    bool stopWaiting = false;

public:
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {return !data_queue.empty() || stopWaiting; });
        if (stopWaiting) {
            return;
        }
        value = data_queue.front();
        data_queue.pop();
    }

    void StopWaiting()
    {
        stopWaiting = true;
        data_cond.notify_one();
    }
};