#pragma once

// windows API ฯเนุ
#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include "path.h"


namespace base
{

fs::path GetAppPath();
fs::path GetAppDataDir();

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
        std::wstring str = m_stream.str() + L"\n";
        OutputDebugStringW(str.c_str());
    }
};

#ifdef _DEBUG
#define DBG(x) base::Dbg() << _T(__FILE__) << L"(" << __LINE__ << L") : " << x;
#else
#define DBG() (void)0;
#endif

// lang MAKELANGID(LANG_CHINESE, SUBLANG_DEFAULT)
std::wstring GetErrorMsg(DWORD errCode, DWORD lang = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));


} // namespace base



