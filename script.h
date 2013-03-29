#pragma once

namespace Script {

int LoadLuaEngine(const char* filename);
void UnloadLuaEngine();

void CallLuaFunc(int r);

}
