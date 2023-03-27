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

// 
time_t Now();

// 
std::string StrFormatTime(time_t t, const std::string & format = "%Y-%m-%d %H:%M:%S", bool tz = false);

}; // namespace base

