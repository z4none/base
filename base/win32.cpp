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

} // namespace base