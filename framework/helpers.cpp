// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Helper functions

#include "pch-ue.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <codecvt>
#include <locale>
#include <filesystem>
#include "helpers.h"

using namespace std;

// Define current server region global variable (declared in helpers.h)
GameServerRegion curr_server = GameServerRegion::UNKNOWN;

// Log file location
extern const LPCWSTR LOG_FILE;

// Helper function to get the current exe directory
string get_em_directory()
{
    //从 D:\Duet Night Abyss\DNA Game\EM\Binaries\Win64 获取到 D:\Duet Night Abyss\DNA Game\EM
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    
    string exePath(buffer);
    
    // 找到exe文件名，去掉文件名得到Binaries\Win64目录
    size_t pos = exePath.find_last_of("\\/");
    if (pos == string::npos) {
        return "";
    }
    string binariesWin64Dir = exePath.substr(0, pos);
    
    // 从Binaries\Win64目录向上一级到Binaries目录
    pos = binariesWin64Dir.find_last_of("\\/");
    if (pos == string::npos) {
        return "";
    }
    string binariesDir = binariesWin64Dir.substr(0, pos);
    
    // 从Binaries目录向上一级到EM目录
    pos = binariesDir.find_last_of("\\/");
    if (pos == string::npos) {
        return "";
    }
    string emDir = binariesDir.substr(0, pos);
    
    return emDir;
}

// Helper function to get the module base address
uintptr_t ue_get_base_address()
{

    auto ptr = (uintptr_t)GetModuleHandleW(L"EM-Win64-Shipping.exe");
    LOG_INFO("BaseModuleHandle: {:#x}", ptr);
    return ptr;

    // return (uintptr_t) GetModuleHandleW(L"game-shipping.exe");
}

// Helper function to append text to a file
void ue_log_write(string text)
{
    HANDLE hfile = CreateFileW(LOG_FILE, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hfile == INVALID_HANDLE_VALUE)
        MessageBoxW(0, L"Could not open log file", 0, 0);

    DWORD written;
    WriteFile(hfile, text.c_str(), (DWORD)text.length(), &written, NULL);
    WriteFile(hfile, "\r\n", 2, &written, NULL);
    CloseHandle(hfile);
}

// Helper function to open a new console window and redirect stdout there
void ue_new_console()
{
    AllocConsole();
    freopen_s((FILE **)stdin, "CONIN$", "r", stdin);
    freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE **)stderr, "CONOUT$", "w", stderr);
    SetConsoleOutputCP(CP_UTF8);
}

#if _MSC_VER >= 1920

#endif

pair<int, int> GetForegroundWindowClientSize()
{
    HWND hwnd = GetForegroundWindow();
    if (!hwnd)
        return {0, 0};

    RECT rect{};
    if (!GetClientRect(hwnd, &rect))
        return {0, 0};

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    return {width, height};
}



GameServerRegion get_game_server()
{
    // 获取配置文件路径
    string ConfigPath = get_em_directory() + "\\Config\\DefaultGameConfigSetttings.ini";
    
    // 检查配置文件是否存在
    if (std::filesystem::exists(ConfigPath)) {
        // 读取文件内容
        ifstream configFile(ConfigPath);
        if (configFile.is_open()) {
            string line;
            while (getline(configFile, line)) {
                // 查找 PakBranch 配置项
                if (line.find("PakBranch=") != string::npos) {
                    // 提取 PakBranch 的值
                    size_t pos = line.find("PakBranch=");
                    if (pos != string::npos) {
                        string pakBranchValue = line.substr(pos + 10); 
                        // 根据不同的 PakBranch 值判断服务器区域
                        if (pakBranchValue == "PC_OBT_CN_Pub") {
                            LOG_INFO("Game Server is CN");
                            curr_server = GameServerRegion::CN;
                            return GameServerRegion::CN;
                        }
                        else if (pakBranchValue == "PC_OBT_Global_Pub") {
                            LOG_INFO("Game Server is GLOBAL");
                            curr_server = GameServerRegion::CN;
                            return GameServerRegion::GLOBAL;
                        }
                    }
                    break;
                }
            }
            configFile.close();
        }
    }

    LOG_INFO("Unknown game server region");
    return GameServerRegion::UNKNOWN;
}
