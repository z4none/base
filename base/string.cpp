#include "string.h"

#include <algorithm>
#include <cctype>

namespace base
{

//
std::string ToLower(const std::string & s)
{
    std::string str = s;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

//
std::wstring ToLower(const std::wstring & s)
{
    std::wstring str = s;
    std::transform(str.begin(), str.end(), str.begin(), ::towlower);
    return str;
}

//
std::string ToUpper(const std::string & s)
{
    std::string str = s;
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

//
std::wstring ToUpper(const std::wstring & s)
{
    std::wstring str = s;
    std::transform(str.begin(), str.end(), str.begin(), ::towupper);
    return str;
}

//
std::string ReplaceAll(std::string & str, const std::string & toSearch, const std::string & replaceStr)
{
    size_t pos = str.find(toSearch);
    while (pos != std::string::npos)
    {
        str.replace(pos, toSearch.size(), replaceStr);
        pos = str.find(toSearch, pos + replaceStr.size());
    }
    return str;
}

//
std::wstring ReplaceAll(std::wstring & wstr, const std::wstring & toSearch, const std::wstring & replaceStr)
{
    size_t pos = wstr.find(toSearch);
    while (pos != std::wstring::npos)
    {
        wstr.replace(pos, toSearch.size(), replaceStr);
        pos = wstr.find(toSearch, pos + replaceStr.size());
    }
    return wstr;
}

//
std::string RTrim(std::string s, const char * t)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

//
std::string LTrim(std::string s, const char * t)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

//
std::string Trim(std::string s, const char * t)
{
    return LTrim(RTrim(s, t), t);
}

//
std::wstring RTrim(std::wstring s, const wchar_t * t)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

//
std::wstring LTrim(std::wstring s, const wchar_t * t)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

//
std::wstring Trim(std::wstring s, const wchar_t * t)
{
    return LTrim(RTrim(s, t), t);
}

//
bool StrEqual(const std::string & a, size_t pos, size_t len, const std::string & b, bool ic)
{
    if (len != b.size()) return false;

    if (ic) 
    {
        return  std::equal(a.begin() + pos, a.begin() + pos + len, b.begin(), [](const char& c, const char& d)
            {
                return (std::tolower(c) == std::tolower(d));
            }
        );
    }
    return  std::equal(a.begin() + pos, a.begin() + pos + len, b.begin());
}

//
bool StrEqual(const std::wstring & a, size_t pos, size_t len, const std::wstring & b, bool ic)
{
    if (len != b.size()) return false;

    if (ic)
    {
        return  std::equal(a.begin() + pos, a.begin() + pos + len, b.begin(), [](const wchar_t& c, const wchar_t& d)
        {
            return (std::tolower(c) == std::tolower(d));
        }
        );
    }
    return  std::equal(a.begin() + pos, a.begin() + pos + len, b.begin());
}

//
bool StartsWith(const std::string & s, const std::string & pre, bool ic)
{
    if (s.size() < pre.size())
    {
        return false;
    }

    return StrEqual(s, 0, pre.size(), pre);
}

//
bool EndsWith(const std::string & s, const std::string & suf, bool ic)
{
    if (s.size() < suf.size())
    {
        return false;
    }

    return StrEqual(s, s.size() - suf.size(), suf.size(), suf);
}

//
bool StartsWith(const std::wstring & s, const std::wstring & pre, bool ic)
{
    if (s.size() < pre.size())
    {
        return false;
    }

    return StrEqual(s, 0, pre.size(), pre);
}

//
bool EndsWith(const std::wstring & s, const std::wstring & suf, bool ic)
{
    if (s.size() < suf.size())
    {
        return false;
    }

    return StrEqual(s, s.size() - suf.size(), suf.size(), suf);
}

//
size_t StrFind(const std::string & strHaystack, const std::string & strNeedle, bool ci)
{
    if (ci)
    {
        auto it = std::search(
            strHaystack.begin(), strHaystack.end(),
            strNeedle.begin(), strNeedle.end(),
            [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
        );
        return it == strHaystack.end() ? -1 : it - strHaystack.begin();
    }
    return strHaystack.find(strNeedle);
}

//
size_t StrFind(const std::wstring & strHaystack, const std::wstring & strNeedle, bool ci)
{
    if (ci)
    {
        auto it = std::search(
            strHaystack.begin(), strHaystack.end(),
            strNeedle.begin(), strNeedle.end(),
            [](wchar_t ch1, wchar_t ch2) { return std::toupper(ch1) == std::toupper(ch2); }
        );
        return it == strHaystack.end() ? -1 : it - strHaystack.begin();
    }
    return strHaystack.find(strNeedle);
}

//
std::wstring StrJoin(std::vector<std::wstring> items, const std::wstring & sep)
{
    std::wstring str;
    for (size_t i = 0; i < items.size(); i++)
    {
        str += items[i];
        if (i + 1 < items.size())
        {
            str += sep;
        }
    }
    return str;
}

//
std::string StrJoin(std::vector<std::string> items, const std::string & sep)
{
    std::string str;
    for (size_t i = 0; i < items.size(); i++)
    {
        str += items[i];
        if (i + 1 < items.size())
        {
            str += sep;
        }
    }
    return str;
}

//
size_t StrSplit(const std::string & s, std::vector<std::string>& tokens, const std::string & sep)
{
    std::string::size_type lastPos = s.find_first_not_of(sep, 0);
    std::string::size_type pos = s.find_first_of(sep, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(sep, pos);
        pos = s.find_first_of(sep, lastPos);
    }
    return tokens.size();
}

//
size_t StrSplit(const std::wstring & s, std::vector<std::wstring>& tokens, const std::wstring & sep)
{
    std::wstring::size_type lastPos = s.find_first_not_of(sep, 0);
    std::wstring::size_type pos = s.find_first_of(sep, lastPos);
    while (std::wstring::npos != pos || std::wstring::npos != lastPos)
    {
        tokens.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(sep, pos);
        pos = s.find_first_of(sep, lastPos);
    }
    return tokens.size();
}


std::string Rot13(const std::string & str) 
{
    std::string result = "";
    for (char c : str) {
        if (c >= 'a' && c <= 'z') {
            c = 'a' + ((c - 'a' + 13) % 26);
        }
        else if (c >= 'A' && c <= 'Z') {
            c = 'A' + ((c - 'A' + 13) % 26);
        }
        result += c;
    }
    return result;
}

} // namespace base
