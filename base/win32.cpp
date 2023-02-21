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

std::wstring GetErrorMsg(DWORD errCode, DWORD lang)
{
    LPWSTR messageText = nullptr;

    __try
    {
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errCode,
            lang,
            (LPWSTR)&messageText,
            0, NULL);
        return messageText;
    }
    __finally
    {
        if (messageText)
        {
            LocalFree(messageText);
        }
    }
}

} // namespace base