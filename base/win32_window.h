#pragma once

// windows API 相关
#include <Windows.h>
#include <tchar.h>
#include <Tlhelp32.h>

#include <sstream>
#include "path.h"


namespace base
{

//
class SubClasser
{
public:
    ~SubClasser()
    {
        Remove();
    }
    static int id;
    typedef std::function<HRESULT(HWND, UINT, WPARAM, LPARAM, LPVOID, BOOL&)> PROC_FUNC;
    static LRESULT CALLBACK SubclassProc(
        HWND hWnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam,
        UINT_PTR uIdSubclass,
        DWORD_PTR dwRefData);

    bool Install(HWND hwnd, PROC_FUNC proc, LPVOID data);
    void Remove();

    LRESULT CallProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd = NULL;
    WNDPROC m_oldproc = NULL;
    PROC_FUNC m_proc;
    LPVOID m_data;
    int m_id;
};

//
class CWindow
{
public:

    HWND m_hWnd = NULL;

public:
    CWindow() {}
    template<typename T>
    static std::shared_ptr<T> Create(HINSTANCE hInstance, HWND parent = NULL,DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, DWORD dwExStyle=WS_EX_APPWINDOW, int nLeft= CW_USEDEFAULT, int nTop= CW_USEDEFAULT, int nWidth= CW_USEDEFAULT, int nHeight= CW_USEDEFAULT, int nShowCmd=SW_SHOW)
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
            dwExStyle,
            window->WndClass(),
            window->Title(),
            dwStyle,
            nLeft,
            nTop,
            nWidth,
            nHeight, 
            parent,
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

    virtual LRESULT CALLBACK HandleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bHandled);

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



