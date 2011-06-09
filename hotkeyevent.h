#pragma once

namespace HotkeyEvent {

struct HOTKEY_EVENT {
    UINT modifiers;
    UINT vk;
    int  id;
    int func_ref;
};

void AddHotkeyEvent(UINT modifiers, UINT vk, int func_ref);

void AssocHotkeyToWindow(HWND hwnd);
void DisassocHotkeyToWindow(HWND hwnd);

void OnHotkeyEvent(int id);

};
