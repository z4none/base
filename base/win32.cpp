#include "win32.h"

#include <shlobj_core.h>


namespace base
{

//
fs::path GetAppPath()
{
    wchar_t buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    return buffer;
}

//
fs::path GetAppDataDir()
{
    wchar_t path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL,
        CSIDL_APPDATA,
        NULL,
        0,
        path)))
    {
        return path;
    }

    return {};
}

// process
DWORD GetProcessIdByName(LPCTSTR name, PROCESSENTRY32 * proc)
{
    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);

    DWORD id = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL ProcFind = FALSE;

    for (ProcFind = Process32First(snapshot, &pe); ProcFind; ProcFind = Process32Next(snapshot, &pe))
    {
        if (wcscmp(pe.szExeFile, name) == 0)
        {
            id = pe.th32ProcessID;
            if (proc)
            {
                *proc = pe;
            }
            break;
        }
    }

    CloseHandle(snapshot);
    return id;
}

// thread
void SetCurrentThreadName(const std::wstring name)
{
    SetThreadDescription(GetCurrentThread(), name.c_str());
}

//
std::wstring GetErrorMsg(DWORD errCode, DWORD lang)
{
    std::wstring msg;
    LPWSTR buffer = nullptr;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errCode,
        lang,
        (LPWSTR)&buffer,
        0, NULL);
    if (buffer)
    {
        msg = buffer;
        LocalFree(buffer);
    }

    return msg;
}

void ShowInExplorer(const fs::path& path)
{
    CoInitialize(NULL);
    ITEMIDLIST* pidl = ILCreateFromPath(path.wstring().c_str());
    if (pidl)
    {
        SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);
        ILFree(pidl);
    }
    CoUninitialize();
}


fs::path OpenFileDialog(HWND hwndOwner, const std::wstring& title, const std::wstring& initialDir, 
                        const std::wstring& filter, DWORD flags) 
{
    // Initialize the OPENFILENAME structure
    OPENFILENAMEW ofn;       // common dialog box structure
    wchar_t szFile[260];     // buffer for file name

    // Initialize the buffer to empty string
    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(szFile, sizeof(szFile));

    // Set up members of the OPENFILENAME structure
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwndOwner;        // Handle to owner window
    ofn.lpstrFile = szFile;           // buffer for file name
    ofn.nMaxFile = sizeof(szFile);    // Maximum length for file name
    
    // 处理文件过滤器
    std::wstring filterStr;
    if (filter.empty()) {
        filterStr = L"All Files\0*.*\0";
    } else {
        filterStr = filter;
        // 确保过滤器以双NULL结束
        if (filterStr.back() != L'\0') {
            filterStr.push_back(L'\0');
        }
        if (filterStr.size() > 0 && filterStr[filterStr.size() - 2] != L'\0') {
            filterStr.push_back(L'\0');
        }
    }
    
    // 创建一个临时缓冲区来存储过滤器字符串
    std::vector<wchar_t> filterBuffer(filterStr.begin(), filterStr.end());
    // 确保以双NULL结束
    if (filterBuffer.size() > 0 && filterBuffer.back() != L'\0') {
        filterBuffer.push_back(L'\0');
    }
    if (filterBuffer.size() > 1 && filterBuffer[filterBuffer.size() - 2] != L'\0') {
        filterBuffer.push_back(L'\0');
    }
    
    ofn.lpstrFilter = filterBuffer.data();
    ofn.nFilterIndex = 1;             // Default filter index
    
    // 设置对话框标题
    std::wstring titleStr = title;
    ofn.lpstrTitle = title.empty() ? NULL : titleStr.c_str();
    
    ofn.lpstrFileTitle = NULL;        // Title of the selected file
    ofn.nMaxFileTitle = 0;            // Maximum length of file title
    
    // 设置初始目录
    std::wstring initialDirStr = initialDir;
    ofn.lpstrInitialDir = initialDir.empty() ? NULL : initialDirStr.c_str();
    
    // 设置标志
    ofn.Flags = flags == 0 ? (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST) : flags;

    // Display the Open dialog box and get the result
    if (GetOpenFileNameW(&ofn)) {
        // Return the selected file name as std::wstring
        return std::wstring(ofn.lpstrFile);
    }
    else {
        // If there was an error, return an empty string (you can also handle the error here)
        return std::wstring();
    }
}


fs::path SaveFileDialog(HWND hwndOwner, const std::wstring& title, const std::wstring& initialDir, 
                        const std::wstring& filter, DWORD flags)
{
    // 初始化 OPENFILENAME 结构体
    OPENFILENAMEW ofn;       // 通用对话框结构
    wchar_t szFile[260];     // 文件名的缓冲区

    // 初始化缓冲区为空字符串
    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(szFile, sizeof(szFile));

    // 设置 OPENFILENAME 结构体的成员
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwndOwner;        // 持有窗口的句柄
    ofn.lpstrFile = szFile;           // 文件名的缓冲区
    ofn.nMaxFile = sizeof(szFile);    // 文件名的最大长度
    
    // 处理文件过滤器
    std::wstring filterStr;
    if (filter.empty()) {
        filterStr = L"All Files\0*.*\0";
    } else {
        filterStr = filter;
        // 确保过滤器以双NULL结束
        if (filterStr.back() != L'\0') {
            filterStr.push_back(L'\0');
        }
        if (filterStr.size() > 0 && filterStr[filterStr.size() - 2] != L'\0') {
            filterStr.push_back(L'\0');
        }
    }
    
    // 创建一个临时缓冲区来存储过滤器字符串
    std::vector<wchar_t> filterBuffer(filterStr.begin(), filterStr.end());
    // 确保以双NULL结束
    if (filterBuffer.size() > 0 && filterBuffer.back() != L'\0') {
        filterBuffer.push_back(L'\0');
    }
    if (filterBuffer.size() > 1 && filterBuffer[filterBuffer.size() - 2] != L'\0') {
        filterBuffer.push_back(L'\0');
    }
    
    ofn.lpstrFilter = filterBuffer.data();
    ofn.nFilterIndex = 1;             // 默认过滤器索引
    
    // 设置对话框标题
    std::wstring titleStr = title;
    ofn.lpstrTitle = title.empty() ? NULL : titleStr.c_str();
    
    ofn.lpstrFileTitle = NULL;        // 选择文件的标题
    ofn.nMaxFileTitle = 0;            // 文件标题的最大长度
    
    // 设置初始目录
    std::wstring initialDirStr = initialDir;
    ofn.lpstrInitialDir = initialDir.empty() ? NULL : initialDirStr.c_str();
    
    // 设置标志
    ofn.Flags = flags == 0 ? OFN_OVERWRITEPROMPT : flags;  // 如果文件已存在，提示覆盖

    // 显示保存对话框并获取结果
    if (GetSaveFileNameW(&ofn)) {
        // 返回所选文件名作为 std::wstring
        return std::wstring(ofn.lpstrFile);
    }
    else {
        // 如果发生错误，返回空字符串（也可以在这里处理错误）
        return std::wstring();
    }
}


} // namespace base