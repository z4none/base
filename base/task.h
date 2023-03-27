#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>


namespace base
{

using namespace std::chrono;

//
class TaskRunner
{
    typedef milliseconds Ms;
    typedef steady_clock Clock;
    typedef std::function<void(void)> Task;

    std::multimap<Clock::time_point, Task> m_tasks;
    std::mutex m_mtx;
    

public:
    TaskRunner() {}

    void SetTimeout(Ms ms, Task task) 
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_tasks.emplace(Clock::now() + ms, task);
    }

    void Queue(const Task task)
    {
        SetTimeout(1ms, task);
    }

    void Run()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        if (m_tasks.empty()) return;

        const Clock::time_point now = Clock::now();
        for (auto task = m_tasks.begin(); task != m_tasks.end(); )
        {
            if (task->first <= now)
            {
                task->second();
                auto taskSaved = task;
                task++;
                task = m_tasks.erase(taskSaved);
            }
            else break;
        }
    }

    void Clear()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_tasks.clear();
    }

    bool Empty()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        return m_tasks.empty();
    }
};

} // namespace base
