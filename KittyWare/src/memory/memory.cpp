#pragma once
#include <memory/memory.h>

std::string Gate::GetProcessNameViaHWND(HWND hwnd) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess == NULL) {
        return "";
    }

    char buffer[MAX_PATH];
    if (GetModuleFileNameEx(hProcess, NULL, buffer, MAX_PATH) == 0) {
        CloseHandle(hProcess);
        return "";
    }

    CloseHandle(hProcess);
    return std::string(buffer);
}

int Gate::ExtractNumber(const std::string& str) {
    int result = 0;

    for (char ch : str) {
        if (std::isdigit(ch)) {
            result = result * 10 + (ch - '0');
        }
    }

    return result;
}

ULONG64 Gate::GetModuleBaseAddress(DWORD process_id, const char* module_name) {
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hModuleSnap, &me32)) {
		CloseHandle(hModuleSnap);
		return 0;
	}

	do {
		if (_tcsicmp(me32.szModule, module_name) == 0) {
			CloseHandle(hModuleSnap);
			return (ULONG64)me32.modBaseAddr;
		}
	} while (Module32Next(hModuleSnap, &me32));

	CloseHandle(hModuleSnap);
	return 0;
}