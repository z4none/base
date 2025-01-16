#include "datetime.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace base 
{

//
uint64_t base::GetTimeUs()
{
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

//
uint64_t base::GetTimeMs()
{
    return base::GetTimeUs() / 1000;
}


//
time_t Now()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

//
std::string StrFormatTime(const std::string& format, time_t t, bool tz)
{
    if (t == 0)
    {
        t = time(0);
    }

    struct tm tm_;
    bool ok;
#ifdef _WIN32
    ok = ((tz ? localtime_s : gmtime_s)(&tm_, &t) == 0);
#else
    ok = (tz ? localtime_r : gmtime_r)(&t, &tm_);
#endif
    if (!ok)
    {
        throw std::runtime_error("get time failed");
    }

    // https://en.cppreference.com/w/cpp/io/manip/put_time
    std::stringstream ss;
    ss << std::put_time(&tm_, format.c_str());
    return ss.str();
}
} // namespace base
