#include "stdafx.h"
#include "resource.h"

#include <string.h>

#include "util.h"

namespace Util {

PROCSWITCHTOTHISWINDOW SwitchToThisWindow;

void RunProcess(LPCTSTR cmdline)
{
    TCHAR command[MAX_PATH] = {0};
    strcpy(command, cmdline);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi))
    {
        // Failed
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


void LoadApi()
{
    static bool loaded = FALSE;
    if (!loaded) {
        HMODULE hUser32 = GetModuleHandle("user32");
        SwitchToThisWindow = (PROCSWITCHTOTHISWINDOW)
            GetProcAddress(hUser32,"SwitchToThisWindow");
		loaded = TRUE;
    }
}

//
// Returns the folder portion from a path
//
CString GetFolderOnly(LPCTSTR Path)
{
    // Strip off the file name so we can direct the file scanning dialog to
    // go back to the same directory as before.
    CString temp = (LPCTSTR) Path; // Force CString to make a copy
    ::PathRemoveFileSpec(temp.GetBuffer(0));
    temp.ReleaseBuffer(-1);
    return temp;
}

CString GetAppDir()
{
    CString fullPath;
	DWORD pathLen = ::GetModuleFileName(
        _Module.GetModuleInstance(),
        fullPath.GetBufferSetLength(MAX_PATH+1),
        MAX_PATH);
     // Note that ReleaseBuffer doesn't need a +1 for the null byte.
    fullPath.ReleaseBuffer(pathLen);
    return GetFolderOnly(fullPath);
}

}
