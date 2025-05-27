#pragma once

#include <string>
#include <chrono>

/*
operator""h
operator""min
operator""s
operator""ms
operator""us
operator""ns
*/
using namespace std::chrono_literals;
using namespace std::chrono;

namespace base 
{

uint64_t GetTimeUs();
uint64_t GetTimeMs();

// 
//time_t Now();

// 
std::string StrFormatTime(time_t t, const std::string & format = "%Y-%m-%d %H:%M:%S", bool tz = false);

//
class TimeIt
{
    time_t m_begin;
    time_t m_end;

public:
    TimeIt()
    {
        Begin();
    }

    void Begin()
    {
        m_begin = GetTimeMs();
    }

    time_t End()
    {
        m_end = GetTimeMs();
        return Duration();
    }

    time_t Duration()
    {
        return m_end - m_begin;
    }
};

}; // namespace base

