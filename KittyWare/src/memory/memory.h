#pragma once
#include <Windows.h>
#include <string>
#include <Psapi.h>
#include <TlHelp32.h>
#include <tchar.h>

namespace Gate {
	inline std::uint32_t process_id = 0;
	inline HANDLE handle;

    std::string GetProcessNameViaHWND(HWND hwnd);
	int ExtractNumber(const std::string & str);
	ULONG64 GetModuleBaseAddress(DWORD process_id, const char* module_name);
}