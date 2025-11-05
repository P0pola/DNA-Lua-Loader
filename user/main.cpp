// Generated C++ file by Il2CppInspectorPro - https://github.com/jadis0x
// Custom injected code entry point

#include "pch-ue.h"

#define WIN32_LEAN_AND_MEAN



#include "main.h"
#include <ue-init.h>


#include "../appdata/Earlyhook.h"


using namespace app;

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"Logs.txt";

HMODULE hModule;
HANDLE hUnloadEvent;

DWORD WINAPI UnloadWatcherThread(LPVOID lpParam)
{
	HANDLE hEvent = static_cast<HANDLE>(lpParam);

	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		std::cout << "[WARNING] Unload signal received.." << std::endl;
		fclose(stdout);
		FreeConsole();
		CloseHandle(hUnloadEvent);
		FreeLibraryAndExitThread(hModule, 0);
	}
	return 0;
}


// Custom injected code entry point
void Run(LPVOID lpParam)
{
	hModule = (HMODULE)lpParam;

	// If you would like to write to a log file, specify the name above and use il2cppi_log_write()
	// il2cppi_log_write("Startup");

	// If you would like to output to a new console window, use il2cppi_new_console() to open one and redirect stdout
	//il2cppi_new_console();
	//Logger_::attach().showFileName().showLineNumber().consoleOnly().enableColors();
	//SetConsoleTitleA("UnrealEngine Game Console");
	//HWND hwnd = GetConsoleWindow();
	//if (hwnd != NULL)
	//{
	//	ShowWindow(hwnd, settings.show_console ? SW_SHOW : SW_HIDE);
	//}

	//hookEnumProcessModules();
	while (!FindWindowA("UnrealWindow", nullptr))
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	get_game_server();
	init_ue();
	//init_external_script_map();
	enabled_hook_early();



	
	LOG_INFO("Initialized");

	



	//hUnloadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//if (hUnloadEvent == NULL) {
	//	std::cout << "Failed to create unload event! Error code: " << GetLastError() << std::endl;
	//	return;
	//}


	//DWORD dwThreadId;
	//HANDLE hWatcherThread = CreateThread(NULL, 0, UnloadWatcherThread, hUnloadEvent, 0, &dwThreadId);
	//if (hWatcherThread != NULL)
	//{
	//	CloseHandle(hWatcherThread);
	//}
	//else
	//{
	//	std::cout << "[ERROR] Unable to create unload monitor thread! Error code: " << GetLastError() << std::endl;
	//}
}

