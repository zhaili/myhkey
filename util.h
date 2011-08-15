#pragma once

namespace Util {

typedef void (WINAPI *PROCSWITCHTOTHISWINDOW) (HWND, BOOL);

void RunProcess(LPCTSTR cmdline);

CString GetAppDir();

int RunFileDlg();

}
