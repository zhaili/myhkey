#include "stdafx.h"

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#include <string.h>
#include <vector>

#include "script.h"

#include "util.h"
#include "hotkeyevent.h"

namespace Script {

#define LUA_REG_FUNC(func)                      \
    lua_pushcfunction(g_L, lua_##func);         \
    lua_setglobal(g_L, #func);


lua_State *g_L;

void CallLuaFunc(int r)
{
    lua_rawgeti(g_L, LUA_REGISTRYINDEX, r);
    lua_pcall(g_L, 0, 1, 0);
//	lua_tointeger(L, -1);
}

void GetVKeyFromStr(const char* key, UINT &modifiers, UINT &vkey)
{
    CString str(key);
    CString resToken;
    int curPos= 0;

    modifiers = 0;
    vkey      = 0;

    resToken = str.Tokenize("+", curPos);
    while (resToken != "")
    {
		if (resToken == "WIN") {
            modifiers |= MOD_WIN;
        }
        else if (resToken == "ALT") {
            modifiers |= MOD_ALT;
        }
        else if (resToken == "CTRL") {
            modifiers |= MOD_CONTROL;
        }
        else if (resToken == "SHIFT") {
            modifiers |= MOD_SHIFT;
        }
		else if (resToken.GetLength()==1) {
            vkey = resToken[0];
        }
        resToken = str.Tokenize("+", curPos);
    }
}

int lua_RegHotkey(lua_State* L)
{
    const char* key = lua_tostring(L, 1);
    ATLTRACE("key: %s\n", key);

    UINT modifiers = 0;
    UINT vkey      = 0;
    GetVKeyFromStr(key, modifiers, vkey);
    
	int func_ref = luaL_ref(g_L, LUA_REGISTRYINDEX);

    HotkeyEvent::AddHotkeyEvent(modifiers, vkey, func_ref);

    return 0;                   // no return values
}

int lua_RunProcess(lua_State *L)
{
    const char* procname = lua_tostring(L, 1);
    Util::RunProcess(procname);

    return 0;
}

int lua_ActivateWindow(lua_State *L)
{
    HWND hwnd = (HWND)lua_tointeger(L, 1);
    SwitchToThisWindow(hwnd, TRUE);

    return 0;
}

int lua_ToggleDesktop(lua_State *L)
{
	Util::ToggleDesktop();

    return 0;
}

int lua_MinimizeAll(lua_State *L)
{
	Util::MinimizeAll();

    return 0;
}

int lua_FindWindow(lua_State *L)
{
    const char* title = lua_tostring(L, 1);
    const char* winclass = lua_tostring(L, 2);
    HWND hwnd = FindWindow(title, winclass);
    if (hwnd) {
        lua_pushinteger(L, (lua_Integer)hwnd);
    } else {
        lua_pushnil(L);
    }
    return 1;                   // return 1 value
}

int lua_PostMessage(lua_State *L)
{
    HWND hwnd = (HWND)lua_tointeger(L, 1);
    UINT msg  = (UINT)lua_tointeger(L, 2);
    WPARAM wparam = (WPARAM)lua_tointeger(L, 3);
    LPARAM lparam = (LPARAM)lua_tointeger(L, 4);

    BOOL ret = PostMessage(hwnd, msg, wparam, lparam);
    lua_pushboolean(L, ret);
    
    return 1;                   // return 1 value
}

int lua_RunFileDlg(lua_State *L)
{
	Util::RunFileDlg();
    return 0;                   // no return value
}

void LoadLuaEngine(const char* filename)
{
    g_L = luaL_newstate();

    luaL_openlibs(g_L);

    LUA_REG_FUNC(RegHotkey)
    LUA_REG_FUNC(RunProcess)
    LUA_REG_FUNC(ActivateWindow)
    LUA_REG_FUNC(ToggleDesktop)
	LUA_REG_FUNC(MinimizeAll)
    LUA_REG_FUNC(FindWindow)
	LUA_REG_FUNC(RunFileDlg)
	LUA_REG_FUNC(PostMessage)

    luaL_loadfile(g_L, filename);
    lua_pcall(g_L, 0, 0, 0);

    // lua_getglobal(g_L, "a");
    // ATLTRACE("%d\n", lua_tointeger(g_L, -1));
}

void UnloadLuaEngine()
{
    lua_close(g_L);
}

}
