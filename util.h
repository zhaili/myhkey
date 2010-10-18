#pragma once

namespace Util {

typedef void (WINAPI *PROCSWITCHTOTHISWINDOW) (HWND, BOOL);
extern PROCSWITCHTOTHISWINDOW SwitchToThisWindow;

void LoadApi();

void RunProcess(LPCTSTR cmdline);

CString GetAppDir();

}
