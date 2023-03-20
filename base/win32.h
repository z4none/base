#pragma once

// windows API 相关
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

//
class CWindow
{
protected:

    HWND m_hWnd = NULL;

    
public:
    CWindow() {}
    template<typename T>
    static std::shared_ptr<T> Create(HINSTANCE hInstance, int nShowCmd)
    {
        std::shared_ptr<T> window = std::make_shared<T>();

        WNDCLASSW wc = { 0 };
        wc.lpfnWndProc = WndProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hIcon = LoadIconW(hInstance, IDI_APPLICATION);
        wc.lpszClassName = window->WndClass();

        if (RegisterClassW(&wc) == 0)
        {
            return nullptr;
        }

        HWND hWnd = CreateWindowExW(
            0,
            window->WndClass(),
            window->Title(),
            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            nullptr,
            hInstance,
            window.get());
        if (hWnd == nullptr)
        {
            return nullptr;
        }

        ShowWindow(hWnd, nShowCmd);
        UpdateWindow(hWnd);

        return window;
    }

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual LRESULT CALLBACK HandleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bHandled)
    {
        return 0;
    }

    virtual const wchar_t * WndClass() const
    {
        return L"BASE_WINDOW";
    }

    virtual const wchar_t * Title() const
    {
        return L"Hello";
    }

    virtual DWORD Run()
    {
        MSG msg;

        // 主消息循环:
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return (int)msg.wParam;
    }
};


} // namespace base



