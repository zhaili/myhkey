// Implementation of the CNotifyIconData class and the CTrayIconImpl template.
#pragma once

// Wrapper class for the Win32 NOTIFYICONDATA structure
class CNotifyIconData : public NOTIFYICONDATA
{
public:
    CNotifyIconData()
    {
        memset(this, 0, sizeof(NOTIFYICONDATA));
        cbSize = sizeof(NOTIFYICONDATA);
    }
};

// Template used to support adding an icon to the taskbar.
// This class will maintain a taskbar icon and associated context menu.
template <class T>
class CTrayIconImpl
{
private:
    UINT WM_TRAYICON;
    CNotifyIconData m_nid;
    bool m_bInstalled;
    UINT m_nDefault;
public:
    CTrayIconImpl() : m_bInstalled(false), m_nDefault(0)
    {
        WM_TRAYICON = ::RegisterWindowMessage(_T("WM_TRAYICON"));
    }

    ~CTrayIconImpl()
    {
        // Remove the icon
        RemoveIcon();
    }

    // Install a taskbar icon
    //  lpszToolTip     - The tooltip to display
    //  hIcon       - The icon to display
    //  nID     - The resource ID of the context menu
    /// returns true on success
    bool InstallIcon(LPCTSTR lpszToolTip, HICON hIcon, UINT nID)
    {
        T* pT = static_cast<T*>(this);
        // Fill in the data
        m_nid.hWnd = pT->m_hWnd;
        m_nid.uID = nID;
        m_nid.hIcon = hIcon;
        m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        m_nid.uCallbackMessage = WM_TRAYICON;
        _tcscpy_s(m_nid.szTip, lpszToolTip);
        // Install
        m_bInstalled = Shell_NotifyIcon(NIM_ADD, &m_nid) ? true : false;
        // Insert sysmenu
        HMENU hm = GetSystemMenu(pT->m_hWnd, FALSE);
        CMenuHandle menu(hm);
        int menuPos = Util::FindMenuItem(menu, _T("Reload Script"));
        if (menuPos == -1)
            menu.InsertMenu(0, MF_BYPOSITION | MF_STRING, IDC_BTN_RELOAD, _T("Reload Script"));
        // Done
        return m_bInstalled;
    }

    // Remove taskbar icon
    // returns true on success
    bool RemoveIcon()
    {
        if (!m_bInstalled)
            return false;
        // Remove
        m_nid.uFlags = 0;
        return Shell_NotifyIcon(NIM_DELETE, &m_nid) ? true : false;
    }

    // Set the icon tooltip text
    // returns true on success
    bool SetTooltipText(LPCTSTR pszTooltipText)
    {
        if (pszTooltipText == NULL)
            return FALSE;
        // Fill the structure
        m_nid.uFlags = NIF_TIP;
        _tcscpy(m_nid.szTip, pszTooltipText);
        // Set
        return Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? true : false;
    }

    // Set the default menu item ID
    inline void SetDefaultItem(UINT nID) { m_nDefault = nID; }

    BEGIN_MSG_MAP(CTrayIcon)
        MESSAGE_HANDLER(WM_TRAYICON, OnTrayIcon)
    END_MSG_MAP()

    LRESULT OnTrayIcon(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
    {
        // Is this the ID we want?
        if (wParam != m_nid.uID)
            return 0;
        T* pT = static_cast<T*>(this);
        if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
        {
            // Make app the foreground
            ShowWindow(pT->m_hWnd, SW_SHOW);
            ShowWindow(pT->m_hWnd, SW_RESTORE);
            SetForegroundWindow(pT->m_hWnd);
        }
        else if (LOWORD(lParam) == WM_RBUTTONUP)
        {
            HMENU hm = GetSystemMenu(pT->m_hWnd, FALSE);
            CMenuHandle menu(hm);
            CPoint pos;
            GetCursorPos(&pos);

            menu.EnableMenuItem(SC_SIZE, MF_GRAYED);
            menu.EnableMenuItem(SC_MOVE, MF_GRAYED);
            menu.EnableMenuItem(SC_MINIMIZE, MF_GRAYED);
            menu.EnableMenuItem(SC_MAXIMIZE, MF_GRAYED);
            menu.EnableMenuItem(SC_RESTORE, MF_ENABLED);

            SetForegroundWindow(pT->m_hWnd);
            DWORD wparam = menu.TrackPopupMenu(
                TPM_RIGHTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
                pos.x, pos.y, pT->m_hWnd);
            if (wparam) {
                PostMessage(pT->m_hWnd, WM_SYSCOMMAND, wparam, 0);
            }
        }
        return 0;
    }
};
