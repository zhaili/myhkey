#include "stdafx.h"
#include "resource.h"

#include <string.h>

#include "util.h"

namespace util {

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

void BringEmacsToFront()
{
    HWND hwnd = FindWindow("Emacs", NULL);
    //BringWindowToTop(hwnd);
    if (hwnd != NULL) {
        ShowWindow(hwnd, SW_RESTORE);
        SetForegroundWindow(hwnd);
    }
    else {
        RunProcess("D:\\Emacs\\bin\\runemacs.exe");
    }
}

}
