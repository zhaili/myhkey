#pragma once

namespace HotkeyEvent {

struct HOTKEY_EVENT {
    UINT modifiers;
    UINT vk;
    int  id;
    CAtlString func;
};

void AddHotkeyEvent(UINT modifiers, UINT vk, const char* func);

void AssocHotkeyToWindow(HWND hwnd);

void OnHotkeyEvent(int id);

};
