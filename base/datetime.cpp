#include "datetime.h"

namespace base 
{

//
time_t Now()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

//
std::string StrFormatTime(time_t t, const std::string & format, bool tz)
{
    struct tm timeinfo;
    char buffer[256] = { 0 };
    if ((tz ? localtime_s : gmtime_s)(&timeinfo, &t) == 0)
    {
        if (strftime(buffer, sizeof(buffer), format.c_str(), &timeinfo))
        {
            return buffer;
        }
    }

    return "";
}

} // namespace base
