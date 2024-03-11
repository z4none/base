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


} // namespace base