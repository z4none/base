#pragma once

// windows API ���
#include <Windows.h>
#include <tchar.h>
#include <Tlhelp32.h>

#include <sstream>
#include "path.h"


namespace base
{

// path
fs::path GetAppPath();
fs::path GetAppDataDir();

// process
DWORD GetProcessIdByName(LPCTSTR name, PROCESSENTRY32 * proc = nullptr);

// thread
void SetCurrentThreadName(const std::wstring name);


// shell
void ShowInExplorer(const fs::path & path);

fs::path OpenFileDialog(HWND hwndOwner, const std::wstring& title=L"", const std::wstring& initialDir=L"", 
                        const std::wstring& filter=L"", DWORD flags=0);
fs::path SaveFileDialog(HWND hwndOwner, const std::wstring& title=L"", const std::wstring& initialDir=L"", 
                        const std::wstring& filter=L"", DWORD flags=0);

//
class Dbg
{
    std::wstringstream m_stream;

public:
    template <typename T>
    Dbg& operator<<(const T& obj)
    {
        m_stream << obj;
        return *this;
    }

    ~Dbg()
    {
        // 获取当前时间
        SYSTEMTIME st;
        GetLocalTime(&st);

        // 格式化时间字符串 [hh:mm:ss.ms]
        wchar_t timeStr[20];
        swprintf_s(timeStr, L"[%02d:%02d:%02d.%03d] ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

        // 拼接时间和原始消息
        std::wstring str = timeStr + m_stream.str() + L"\n";
        OutputDebugStringW(str.c_str());
    }
};

#ifdef _DEBUG
// #define DBG(x) base::Dbg() << _T(__FILE__) << L"(" << __LINE__ << L") : " << x;
#define DBG(x) base::Dbg() << x;
#else
#define DBG(x) (void)0;
#endif

// lang MAKELANGID(LANG_CHINESE, SUBLANG_DEFAULT)
std::wstring GetErrorMsg(DWORD errCode, DWORD lang = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));


} // namespace base



