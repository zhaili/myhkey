#pragma once

namespace Script {

void LoadLuaEngine(const char* filename);
void UnloadLuaEngine();

void CallLuaFunc(const char* func);

}
