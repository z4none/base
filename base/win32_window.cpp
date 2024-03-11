#include <Windows.h>
#include <CommCtrl.h>
#include "win32_window.h"

namespace base
{

const wchar_t* TAG = L"SUBCLASSPTR";

int SubClasser::id = 0;

LRESULT CALLBACK SubClasser::SubclassProc(HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR uIdSubclass,
    DWORD_PTR dwRefData)
{
    SubClasser* self = (SubClasser*)dwRefData;
    if (self)
    {
        return self->CallProc(hWnd, uMsg, wParam, lParam);
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

//
bool SubClasser::Install(HWND hwnd, PROC_FUNC proc, LPVOID data)
{
    m_id = id++;
    m_hwnd = hwnd;
    m_proc = proc;
    m_data = data;
    SetWindowSubclass(m_hwnd, SubClasser::SubclassProc, m_id, (DWORD_PTR)this);
    return true;
}

//
void SubClasser::Remove()
{
    RemoveWindowSubclass(m_hwnd, SubClasser::SubclassProc, m_id);
}

//
LRESULT SubClasser::CallProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (m_proc)
    {
        BOOL handled = FALSE;
        LRESULT ret = m_proc(hwnd, uMsg, wParam, lParam, m_data, handled);
        if (handled) return ret;
        return DefSubclassProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


//
template<class T, class U, HWND(U::* m_hWnd)> T*
    InstanceFromWndProc(HWND hWnd, UINT uMsg, LPARAM lParam)
{
    T* pInstance;
    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pInstance = (T*)(pCreateStruct->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)(pInstance));
        pInstance->*m_hWnd = hWnd;
    }
    else
    {
        pInstance = (T*)(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    }

    return pInstance;
}

//
LRESULT CALLBACK CWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CWindow* window = InstanceFromWndProc<CWindow, CWindow, &CWindow::m_hWnd>(hWnd, uMsg, lParam);
    if (window)
    {
        bool bHandled = false;
        LRESULT ret = window->HandleWndProc(hWnd, uMsg, wParam, lParam, bHandled);
        if (bHandled)
        {
            return ret;
        }
    }
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CWindow::HandleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    return 0;
}

};// namespace base