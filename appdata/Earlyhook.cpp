#include "pch-ue.h"
#include "Earlyhook.h"



struct FPakFile
{

	char Pad[24];

	wchar_t* PakFilenameDataPtr;

};

void* luacore;
std::string ConvertWCharToNarrow(const wchar_t* wstr) {
    if (!wstr || *wstr == L'\0')
        return "";

	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	if (bufferSize == 0)
		return "ConversionError";

	std::string str(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], bufferSize, nullptr, nullptr);

	if (!str.empty() && str.back() == '\0')
		str.pop_back();

	return str;
}

std::wstring g_dll_dir;
std::unordered_map<std::string, std::wstring> g_script_map;
std::once_flag g_scan_once;

std::wstring GetDllDir() {
    HMODULE hModule = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCWSTR>(&enabled_hook_early), &hModule);
    wchar_t pathBuf[MAX_PATH];
    DWORD len = GetModuleFileNameW(hModule, pathBuf, MAX_PATH);
    if (len == 0) return L"";
    std::filesystem::path p(pathBuf);
    return p.parent_path().wstring();
}

std::string NormalizeKey(const std::wstring& w) {
    std::string s = ConvertWCharToNarrow(w.c_str());
    // for (auto& c : s) c = (char)tolower((unsigned char)c);
    for (auto& c : s) if (c == '\\') c = '/';
    return s;
}

void ScanScriptFolder() {
    g_dll_dir = GetDllDir();
    if (g_dll_dir.empty()) {
        LOG_WARNING("DLL directory not found");
        return;
    }
    std::filesystem::path scriptDir = std::filesystem::path(g_dll_dir) / L"Script";
    if (!std::filesystem::exists(scriptDir)) {
        LOG_INFO("Script directory not found");
        return;
    }
    size_t count = 0;
    for (auto& entry : std::filesystem::recursive_directory_iterator(scriptDir)) {
        if (!entry.is_regular_file()) continue;
        auto rel = std::filesystem::relative(entry.path(), scriptDir);
        auto key = NormalizeKey(rel.wstring());
        g_script_map[key] = entry.path().wstring();
        count++;
    }
    LOG_INFO("Script files scanned: {}", (unsigned long long)count);
}

void init_external_script_map() {
  ScanScriptFolder(); 
  LOG_INFO("g_script_map content after scan:"); // Add this line
  for (const auto& pair : g_script_map) { // Add this loop
      LOG_INFO("  Key: {}, Path: {}", pair.first.c_str(), ConvertWCharToNarrow(pair.second.c_str())); // Add this line
  }
  //auto key = ExtractKeyFromInName("game/test/Script/test.lua");
  //LOG_INFO("Test key extracted: {}", key.c_str()); // Add this line
  //if (!key.empty()) {
  //    std::vector<char> extBuf;
  //    if (TryLoadExternal(key, extBuf)) {
  //        LOG_INFO("Lua overridden: {}", key.c_str());
  //        /*return CALL_ORIGIN(FLuaEnv_LoadBuffer_hook, a1, InL, static_cast<const char*>(extBuf.data()), static_cast<size_t>(extBuf.size()), InName);*/
  //    } else {
  //        LOG_INFO("TryLoadExternal failed for test key: {}", key.c_str()); // Add this line
  //    }
  //} else {
  //    LOG_INFO("Test key is empty."); // Add this line
  //}

}

std::string ExtractKeyFromInName(const char* InName) {
    if (!InName) return "";
    std::string s(InName);

    for (auto& c : s) if (c == '\\') c = '/';


    size_t pos = s.find("/Script/");

    if (pos == std::string::npos) return "";
    std::string key = s.substr(pos + 8);

    return key;
}

bool TryLoadExternal(const std::string& key, std::vector<char>& out) {
    auto it = g_script_map.find(key);
    if (it == g_script_map.end()) {
        LOG_INFO("Key not found in g_script_map: {}", key.c_str());
        return false;
    }
    LOG_INFO("Attempting to open file: {}", ConvertWCharToNarrow(it->second.c_str()));
    std::ifstream f(it->second, std::ios::binary);
    if (!f) {
        LOG_INFO("Failed to open file: {}", ConvertWCharToNarrow(it->second.c_str()));
        return false;
    }
    f.seekg(0, std::ios::end);
    std::streamsize size = f.tellg();
    if (size <= 0) {
        LOG_INFO("File is empty or seek failed for: {}", ConvertWCharToNarrow(it->second.c_str()));
        return false;
    }
    f.seekg(0, std::ios::beg);
    out.resize((size_t)size);
    if (!f.read(out.data(), size)) {
        LOG_INFO("Failed to read file: {}", ConvertWCharToNarrow(it->second.c_str()));
        return false;
    }
    return true;
}


bool pak_check_hook(void* a1) {

	//FPakFile* PakFile = static_cast<FPakFile*>(a1);
	//wchar_t* FilenamePtr = PakFile->PakFilenameDataPtr;
	//if (FilenamePtr)
	//{
	//	std::string narrow_filename = ConvertWCharToNarrow(FilenamePtr);
	//	LOG_DEBUG("Pak Filename: {}", narrow_filename.c_str());
	//	//  wprintf(L"[Hook] FPakFile::Check() called. PakFilename: \"%s\"\n", FilenamePtr);
	//}
	LOG_DEBUG("Pak File check");
	//CALL_ORIGIN(pak_check_hook, a1);
	return true;
}

void Tick_Hook(__int64 a1, void* a2, char* a3) {

	CALL_ORIGIN(Tick_Hook ,a1,a2,a3);
}
bool FLuaEnv_LoadBuffer_hook(__int64 a1, void* InL, const char* Buffer, const size_t Size, const char* InName) {
	if (InName) {
        LOG_INFO("Found Lua path: {}", InName);
		auto key = ExtractKeyFromInName(InName);
		if (!key.empty()) {
			std::vector<char> extBuf;
			if (TryLoadExternal(key, extBuf)) {
				LOG_INFO("Lua overridden: {}", key.c_str());
				return CALL_ORIGIN(FLuaEnv_LoadBuffer_hook, a1, InL, static_cast<const char*>(extBuf.data()), static_cast<size_t>(extBuf.size()), InName);
			}
		}
	}
	return CALL_ORIGIN(FLuaEnv_LoadBuffer_hook, a1, InL, Buffer, Size, InName);
}
void* luaL_dostring_hook(void* a1, const char* a2) {
	luacore = a1;
	LOG_DEBUG(a2);

	return CALL_ORIGIN(luaL_dostring_hook, a1,a2 );
}

void enabled_hook_early() {
	//HookManager::install(app::pak_check, pak_check_hook);
	//LOG_INFO("pak_check hooked");
	HookManager::install(app::FLuaEnv_LoadBuffer, FLuaEnv_LoadBuffer_hook);
	LOG_INFO("FLuaEnv_LoadBuffer hooked");
	//HookManager::install(app::luaL_dostring, luaL_dostring_hook);
	//HookManager::install(app::Tick, Tick_Hook);
	// LOG_INFO("AntiCheat hooked");

}

