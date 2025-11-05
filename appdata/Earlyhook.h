#pragma once

#include <string>

extern void* luacore;

bool pak_check_hook(void* a1);
void Tick_Hook (__int64 a1, void* a2, char* a3);
bool FLuaEnv_LoadBuffer_hook (__int64 a1, void* InL, const char* Buffer, const size_t Size, const char* InName);
std::string ConvertWCharToNarrow(const wchar_t* wstr);
void enabled_hook_early();
void init_external_script_map();
void* luaL_dostring_hook (void* a1, const char* a2);

