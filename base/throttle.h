#pragma once

#include <chrono>
#include <functional>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

namespace base
{

class Throttle
{
    int64_t m_lastTm = 0;
    int64_t m_interval = 0;
    std::function<void()> m_func = nullptr;

    int64_t now()
    {
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
public:
    Throttle(int64_t interval, std::function<void()> func)
    {
        m_interval = interval;
        m_func = func;
    }

    void operator () ()
    {
        if (m_lastTm + m_interval <= now())
        {
            m_lastTm = now();
            m_func();
        }
    }
};

} // namespace base
