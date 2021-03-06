#include "stdafx.h"
#include "resource.h"

#include <string.h>

#include <Userenv.h>

#include "util.h"

namespace Util {

void RunProcess(LPCTSTR cmdline)
{
    TCHAR command[MAX_PATH] = {0};
    _tcscpy_s(command, cmdline);

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
		fullPath.GetBufferSetLength(MAX_PATH),
        MAX_PATH);
	// if pathLen == MAX_PATH then error occurred
	if (pathLen == MAX_PATH)
		fullPath.ReleaseBuffer(pathLen-1);
	else
        fullPath.ReleaseBuffer(pathLen);

    return GetFolderOnly(fullPath);
}

CString GetUserHomeDir()
{
    TCHAR fullPath[MAX_PATH];

    HANDLE hToken = 0;
    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
   
	DWORD pathLen = MAX_PATH;
    if (!GetUserProfileDirectory(hToken, fullPath, &pathLen)) {
        fullPath[0] = 0;        // failed
		pathLen = 1;
    }

    CloseHandle(hToken);
    return CString(fullPath, pathLen-1);
}

void ToggleDesktop()
{
    // Create an instance of the shell class
    IShellDispatch4 *pShellDisp = NULL;
    HRESULT sc = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_SERVER, IID_IDispatch, (LPVOID *) &pShellDisp);
    if (SUCCEEDED(sc)) {
        pShellDisp->ToggleDesktop();
        pShellDisp->Release();
    }
}

void MinimizeAll()
{
    // Create an instance of the shell class
    IShellDispatch4 *pShellDisp = NULL;
    HRESULT sc = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_SERVER, IID_IDispatch, (LPVOID *) &pShellDisp);
    if (SUCCEEDED(sc)) {
        pShellDisp->MinimizeAll();
        pShellDisp->Release();
    }
}

typedef int (__stdcall *pfnRunFileDlg) (HWND hwndParent,
                                        HICON hIcon,
                                        LPWSTR lpszDestDirectory ,
                                        LPWSTR lpszCaption,
                                        LPWSTR lpszText,
                                        DWORD dwFlags);

HINSTANCE hShell32;               // Handle to DLL
pfnRunFileDlg runFileDlg;

#define 	DESKTOPBARBAR_HEIGHT   (GetSystemMetrics(SM_CYSIZE) + 5 * GetSystemMetrics(SM_CYEDGE))

int RunFileDlg()
{
    if (hShell32 == NULL) {
        hShell32 = LoadLibrary("Shell32.dll");
        if (hShell32 == NULL) return -1;
    }
    
    runFileDlg = (pfnRunFileDlg)GetProcAddress(hShell32, (LPCSTR)61);
    if (!runFileDlg)
    {
        FreeLibrary(hShell32);
        return -1;
    }
    else
    {
        RECT rect = {0};
		rect.top = GetSystemMetrics(SM_CYSCREEN) - DESKTOPBARBAR_HEIGHT;
        rect.right = GetSystemMetrics(SM_CXSCREEN);
        rect.bottom = rect.top + DESKTOPBARBAR_HEIGHT;

        HWND dlgOwner = CreateWindow("STATIC", NULL, NULL, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, NULL, NULL);
		USES_CONVERSION;
		CString homedir = GetUserHomeDir();
		int ret = runFileDlg(dlgOwner, LoadIcon(NULL, IDI_INFORMATION), A2W(homedir), NULL, NULL, NULL);
        DestroyWindow(dlgOwner);

        return ret;
    }
}

// FindMenuItem() will find a menu item string from the specified
// popup menu and returns its position (0-based) in the specified 
// popup menu. It returns -1 if no such menu item string is found.
int FindMenuItem(CMenuHandle menu, LPCTSTR menuString)
{
   ATLASSERT(menu.IsMenu());

   int count = menu.GetMenuItemCount();
   for (int i = 0; i < count; i++)
   {
      CString str;
	  if (menu.GetMenuString(i, str, MF_BYPOSITION) &&
		  str.Compare(menuString) == 0)
         return i;
   }

   return -1;
}

}
