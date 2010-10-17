// maindlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "maindlg.h"

#include "util.h"
#include "script.h"
#include "hotkeyevent.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

    InstallIcon(_T("myhkey"), hIconSmall, NULL);

    //RegHotKey();
    Script::LoadLuaEngine("keyset.lua");
	HotkeyEvent::AssocHotkeyToWindow(m_hWnd);

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    RemoveIcon();
    //UnRegHotKey();

	bHandled = FALSE;
    
    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CMainDlg::OnSysCommand(UINT wParam, CPoint point)
{
	if (wParam == SC_MINIMIZE) {
		ShowWindow(SW_MINIMIZE);
		ShowWindow(SW_HIDE);
	}
	else {
		SetMsgHandled(FALSE);
	}
}

void CMainDlg::RegHotKey()
{
    m_hotkTC.reg(m_hWnd, ID_HOTK_START_TC, MOD_WIN, '1');
    m_hotkEmacs.reg(m_hWnd, ID_HOTK_START_EMACS, MOD_WIN, '2');

    return ;
}

void CMainDlg::UnRegHotKey()
{
    m_hotkTC.unreg();
    m_hotkEmacs.unreg();
}

void CMainDlg::OnHotKey(WPARAM id, WORD Vcode, WORD wModifiers)
{
    // if (ID_HOTK_START_TC == id) {
    //     Util::RunProcess("d:\\totalcmd\\TOTALCMD.EXE /o");
    // }
    // else if (ID_HOTK_START_EMACS == id) {
    //     //util::RunProcess("d:\\Emacs\bin\launch-emacs.exe");
    //     Util::BringEmacsToFront();
    //     //ATLTRACE("Emacs");
    // }
    HotkeyEvent::OnHotkeyEvent(id);
}
