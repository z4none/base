#pragma once

#include <string>
#include <vector>

// 字符串相关

namespace base
{

// 大小写
std::string  ToLower(const std::string & s);
std::wstring ToLower(const std::wstring & s);
std::string  ToUpper(const std::string & s);
std::wstring ToUpper(const std::wstring & s);

// 字符串替换
std::string ReplaceAll(std::string & str, const std::string & toSearch, const std::string & replaceStr);
std::wstring ReplaceAll(std::wstring & wstr, const std::wstring & toSearch, const std::wstring & replaceStr);

//
// 截取首尾空白字符(或者其他)
//   Trim(" 123  \r\n")
//   Trim("*123*", "*")
#define SPACE " \t\n\r\f\v"
std::string RTrim(std::string s, const char* t = SPACE);
std::string LTrim(std::string s, const char* t = SPACE);
std::string Trim(std::string s, const char* t = SPACE);

#define WSPACE L" \t\n\r\f\v"
std::wstring RTrim(std::wstring s, const wchar_t* t = WSPACE);
std::wstring LTrim(std::wstring s, const wchar_t* t = WSPACE);
std::wstring Trim(std::wstring s, const wchar_t* t = WSPACE);

// 字符串比较
bool StrEqual(const std::string & a, size_t pos, size_t len, const std::string & b, bool ic = false);
bool StrEqual(const std::wstring & a, size_t pos, size_t len, const std::wstring & b, bool ic = false);

bool StartsWith(const std::string & s, const std::string & pre, bool ic = false);
bool EndsWith(const std::string & s, const std::string & suf, bool ic = false);
bool StartsWith(const std::wstring & s, const std::wstring & pre, bool ic = false);
bool EndsWith(const std::wstring &s, const std::wstring & suf, bool ic = false);

// 查找
size_t StrFind(const std::string & strHaystack, const std::string & strNeedle, bool ci = false);
size_t StrFind(const std::wstring & strHaystack, const std::wstring & strNeedle, bool ci = false);

// 合并
std::string StrJoin(std::vector<std::string> items, const std::string & sep = ",");
std::wstring StrJoin(std::vector<std::wstring> items, const std::wstring & sep = L",");

// 切割
size_t StrSplit(const std::string& s, std::vector<std::string>& tokens, const std::string& sep = ",");
size_t StrSplit(const std::wstring& s, std::vector<std::wstring>& tokens, const std::wstring& sep= L",");

// 脱敏
std::string Desensitize(const std::string & str, size_t left = 2, size_t right = 2, char ch = '*');

// 格式化
#ifdef _MSC_VER
template<typename ... Args>
std::wstring StrFormat(const std::wstring & format, Args ... args)
{
    size_t size = _snwprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size <= 0) { throw std::runtime_error("Error during formatting."); }
    std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
    _snwprintf(buf.get(), size, format.c_str(), args ...);
    return std::wstring(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
#endif

// 字符串格式化
template<typename ... Args>
std::string StrFormat(const std::string & format, Args ... args)
{
#ifdef _MSC_VER
#define __snprintf _snprintf
#else
#define __snprintf snprintf
#endif
    size_t size = __snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size <= 0) { throw std::runtime_error("Error during formatting."); }
    std::unique_ptr<char[]> buf(new char[size]);
    __snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

// 字符串到任意类型
template<typename T>
T StrToX(const std::string& str) {
    T result;
    std::istringstream stream(str);
    stream >> result;
    return result;
}

}; // namespace base