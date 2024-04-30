//// test.cpp : 定义应用程序的入口点。
////
//
//#include <base/win32.h>
//#include <base/task.h>
//
//#include <sstream>
//#include <thread>
//
//#pragma comment(lib, "base.lib")
//
////
//class CWinMain : public base::CWindow
//{
//    base::TaskRunner m_taskRunnner;
//public:
//    virtual LRESULT CALLBACK HandleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bHandled) override
//    {
//        bHandled = true;
//        if (uMsg == WM_CREATE)
//        {
//            std::thread([=]() {
//                while (1) {
//                    Sleep(100);
//                    m_taskRunnner.Queue([]() {
//                        std::stringstream ss;
//                        ss << "task " << GetTickCount() << "\n";
//                        OutputDebugStringA(ss.str().c_str());
//                    });
//                }
//            }).detach();
//            SetWindowText(m_hWnd, L"haha");
//            SetTimer(hWnd, 100, 100, NULL);
//            return TRUE;
//        }
//        else if (uMsg == WM_ERASEBKGND)
//        {
//            HDC hdc = (HDC)wParam;
//            RECT rect;
//            GetClientRect(hWnd, &rect);
//            HBRUSH hBrush = CreateSolidBrush(RGB(100, 100, 100)); // 创建白色画刷
//            FillRect(hdc, &rect, hBrush); // 填充矩形
//            DeleteObject(hBrush); // 删除画刷
//            return TRUE;
//        }
//        else if (uMsg == WM_CLOSE)
//        {
//            PostQuitMessage(0);
//        }
//        else if (uMsg == WM_TIMER)
//        {
//            if (wParam == 100)
//            {
//                m_taskRunnner.Run();
//            }
//            return TRUE;
//        }
//        bHandled = false;
//        return 0;
//    }
//};
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    auto win = CWinMain::Create<CWinMain>(hInstance, nCmdShow);
//    return win->Run();
//}


#include <iostream>
#include <base/win32.h>
#include <base/task.h>
#include <base/datetime.h>
#include <base/stream.h>

#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <future>



uint64_t fab(uint64_t x)
{
    if (x == 1 || x == 2) return 1;
    return fab(x - 1) + fab(x - 2);
}

//
int main()
{

    base::TaskRunner task_runner;
    task_runner.SetTimeout(1000ms, [](){
        base::AtomicWriter() << "call 1" << std::endl;
    });

    task_runner.SetTimeout(2000ms, []() {
        base::AtomicWriter() << "call 2" << std::endl;
    });

    task_runner.SetTimeout(5000ms, []() {
        base::AtomicWriter() << "call 3" << std::endl;
    });

    task_runner.Queue([]() {
        base::AtomicWriter() << "call 0" << std::endl;
    });

    while (1)
    {
        task_runner.Run();
        Sleep(200);
        base::AtomicWriter() << "." << std::endl;
    }

    return 0;
}