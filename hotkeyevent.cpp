/// class HotkeyEventList

#include "stdafx.h"

#include "HotkeyEvent.h"
#include "script.h"

namespace HotkeyEvent {

typedef std::vector<HOTKEY_EVENT> HotkeyEventList;

HotkeyEventList g_keylist;

int GetHotkeyGid()
{
    static int gid = 1000;
    return (++gid);
}

void AddHotkeyEvent(UINT modifiers, UINT vk, const char* func)
{
    HOTKEY_EVENT e;
    e.modifiers = modifiers;
    e.vk        = vk;
    e.id        = GetHotkeyGid();
    e.func      = func;

    g_keylist.push_back(e);
}

void RegHotkeyEvent(const HOTKEY_EVENT &e, HWND hwnd)
{
    if (::RegisterHotKey(hwnd, e.id, e.modifiers, e.vk)) {
        ATLTRACE(_T("Register Hotkey %d succeed!\n"), e.id);
    }
}

void UnregHotkeyEvent(const HOTKEY_EVENT &e, HWND hwnd)
{
    ::UnregisterHotKey(hwnd, e.id);
    ATLTRACE(_T("Unregistered Hotkey %d!\n"), e.id);
}

BOOL GetHotkeyEventByID(int id, HOTKEY_EVENT& e)
{
    std::vector<HOTKEY_EVENT>::const_iterator it = g_keylist.begin();

    BOOL status = FALSE;
    for (; it != g_keylist.end(); ++it) {
        if ((*it).id == id) {
            e = *it;
            status = TRUE;
            break;
        }
    }
    return (status);
}

void CallHotkeyEvent(const HOTKEY_EVENT& e)
{
    Script::CallLuaFunc(e.func);
}

void OnHotkeyEvent(int id)
{
    HOTKEY_EVENT e;
    GetHotkeyEventByID(id, e);
    CallHotkeyEvent(e);
}

void AssocHotkeyToWindow(HWND hwnd)
{
    std::vector<HOTKEY_EVENT>::const_iterator it = g_keylist.begin();

    for (; it != g_keylist.end(); ++it) {
        RegHotkeyEvent(*it, hwnd);
    }
}

}
