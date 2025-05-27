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

fs::path OpenFileDialog() 
{
    // Initialize the OPENFILENAME structure
    OPENFILENAMEW ofn;       // common dialog box structure
    wchar_t szFile[260];     // buffer for file name

    // Initialize the buffer to empty string
    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(szFile, sizeof(szFile));

    // Set up members of the OPENFILENAME structure
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;             // Handle to owner window
    ofn.lpstrFile = szFile;           // buffer for file name
    ofn.nMaxFile = sizeof(szFile);    // Maximum length for file name
    ofn.lpstrFilter =
        L"All\0*.*\0Text\0*.TXT\0"    // File types (first: name, second: extension)
        L"Images\0*.BMP;*.JPG;*.GIF;*.PNG\0";
    ofn.nFilterIndex = 1;             // Default filter index
    ofn.lpstrFileTitle = NULL;        // Title of the selected file
    ofn.nMaxFileTitle = 0;            // Maximum length of file title
    ofn.lpstrInitialDir = NULL;       // Initial directory
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Flags

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

fs::path SaveFileDialog()
{
    // 初始化 OPENFILENAME 结构体
    OPENFILENAMEW ofn;       // 通用对话框结构
    wchar_t szFile[260];     // 文件名的缓冲区

    // 初始化缓冲区为空字符串
    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(szFile, sizeof(szFile));

    // 设置 OPENFILENAME 结构体的成员
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;             // 持有窗口的句柄
    ofn.lpstrFile = szFile;           // 文件名的缓冲区
    ofn.nMaxFile = sizeof(szFile);    // 文件名的最大长度
    ofn.lpstrFilter =
        L"All\0*.*\0Text\0*.TXT\0"    // 文件类型（名称，扩展名）
        L"Images\0*.BMP;*.JPG;*.GIF;*.PNG\0";
    ofn.nFilterIndex = 1;             // 默认过滤器索引
    ofn.lpstrFileTitle = NULL;        // 选择文件的标题
    ofn.nMaxFileTitle = 0;            // 文件标题的最大长度
    ofn.lpstrInitialDir = NULL;       // 初始目录
    ofn.Flags = OFN_OVERWRITEPROMPT;  // 如果文件已存在，提示覆盖

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