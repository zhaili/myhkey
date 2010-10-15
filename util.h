#pragma once

namespace util {

void RunProcess(LPCTSTR cmdline);
void BringEmacsToFront();

class RegHotKey {
public:
    void reg(HWND hwnd, int id, UINT fsModifiers, UINT vk) {
        if (::RegisterHotKey(hwnd, id, fsModifiers, vk)) {
            m_nID = id;
            m_hWnd = hwnd;
            ATLTRACE(_T("Register Hotkey %d succeed!\n"), m_nID);
        }
        else {
            // ::MessageBox(hwnd, _T("Sorry, Register hotkey failed!"), _T("Error"), MB_ICONWARNING);
            ATLTRACE(_T("Sorry, Register hotkey failed!\n"));
        }
        return;
    }

    void unreg() {
        ::UnregisterHotKey(m_hWnd, m_nID);
        ATLTRACE(_T("Unregistered Hotkey %d!\n"), m_nID);
    }

private:
    int m_nID;
    HWND m_hWnd;
};

}
