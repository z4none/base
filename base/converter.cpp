#include <algorithm>
#include "converter.h"

namespace base
{

std::string WsToUtf8(const std::wstring & ws)
{
    std::string utf8;
    Converter("wchar_t", "UTF-8", true).convert(ws, utf8);
    return utf8;
}

std::wstring Utf8ToWs(const std::string & utf8)
{
    std::wstring ws;
    Converter("UTF-8", "wchar_t", true).convert(utf8, ws);
    return ws;
}

std::string Utf32ToUtf8(const std::u32string & u32)
{
    std::string utf8;
    Converter("UTF-32LE", "UTF-8", true).convert(u32, utf8);
    return utf8;
}

std::u32string Utf8ToUtf32(const std::string & utf8)
{
    std::u32string u32;
    Converter("UTF-8", "UTF-32LE", true).convert(utf8, u32);
    return u32;
}

std::string GbkToUtf8(const std::string & gbk)
{
    std::string utf8;
    Converter("GBK", "UTF-8", true).convert(gbk, utf8);
    return utf8;
}

std::string Utf8ToGbk(const std::string & utf8)
{
    std::string gbk;
    Converter("UTF-8", "GBK", true).convert(utf8, gbk);
    return gbk;
}

} // namespace base

