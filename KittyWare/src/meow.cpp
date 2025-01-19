// Global Headers
#include <windows.h>
#include <dwmapi.h>
#include <thread>

// Project Specific
#include <logger.h>
#include <cache.h>
#include <mapper/kdmapper.hpp>
#include <raw/driver.h>
#include <memory/memory.h>
#include <memory/offsets.h>
#include <memory/driver.h>
#include <render/render.h>
#include <cheat/entity_list.hpp>
#include <cheat/vehicle_list.hpp>

// Libraries
#pragma comment(lib, "dwmapi.lib")

void GetOverlay();
void MapDriver();
HWND overlay_hwnd;
HANDLE iqvw64e_device_handle;

auto main() -> int {
    ShowWindow(GetConsoleWindow(), SW_SHOW);

	GetOverlay();
	MapDriver();
    
    Console::Log<Console::PrintType::LOADING>("grcwindow not found. Please open FiveM...\n");
    while (Cache::hwnd == nullptr) {
        Cache::hwnd = FindWindowA("grcwindow", nullptr);
        if (Cache::hwnd != nullptr) break;
        else Sleep(100);
    }

    std::string process_name = Gate::GetProcessNameViaHWND(Cache::hwnd);
    Console::Log<Console::PrintType::DEBUG>("Raw Process Name: " + process_name + "\n");
    size_t pos = process_name.find_last_of("\\");
    process_name = process_name.substr(pos + 1);

    Cache::game_build = Gate::ExtractNumber(process_name);

    // default game build
    if (process_name == "FiveM_GameProcess.exe" || process_name == "FiveM_GTAProcess.exe")
        Cache::game_build = 1604;

    DWORD process_id;
    GetWindowThreadProcessId(Cache::hwnd, &process_id);
    Console::Log<Console::PrintType::DEBUG>("HWND Process ID: " + std::to_string(process_id) + "\n");

    Gate::process_id = process_id;
    Cache::base = Gate::GetModuleBaseAddress(Gate::process_id, process_name.c_str());
    Console::Log<Console::PrintType::DEBUG>("FiveM Image Base: " + std::to_string(Cache::base) + "\n");

    Offsets::Set(Cache::game_build);
    if (Gate::process_id != 0 && Cache::base != 0 && Cache::hwnd != 0)
        Console::Log<Console::PrintType::OK>("FiveM found successfully - b" + std::to_string(Cache::game_build) + "\n");
    else {
        Console::Log<Console::PrintType::FAIL>("0xDEAD Something went severly wrong. Refer to the wiki. The process was either killed or an anticheat/antivirus is interfering. If this is a recurring problem, contact support via discord asap. The software will close in 10 seconds.\n");
        Sleep(10000);
        exit(0);
        return 0;
    }

    Gate::handle = CreateFile("\\\\.\\Krypted", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (Gate::handle == INVALID_HANDLE_VALUE) {
        Console::Log<Console::PrintType::FAIL>("0xEAC Run KittyWare as Administrator. Refer to the wiki. If it is already running as Administrator, check if FACEIT anticheat is running, if so right click and disable it. Or if vanguard is running, stop that. This error usually happens due to anticheats. If the problem persisnts, contact us on discord. The software will close in 10 seconds.\n");
        Sleep(10000);
        exit(0);
        return 0;
    }

    Gate::driver = Gate::handle;
    Console::Log<Console::PrintType::OK>("Driver has been mapped successfully!\n");

    if (!Gate::Attach(Gate::process_id)) {
        Console::Log<Console::PrintType::FAIL>("0xEAF Something went wrong. The driver failed to attach to the process. Please refer to wiki. If the problem persisnts, contact us on discord. The software will close in 10 seconds.\n");
        Sleep(10000);
        exit(0);
        return 0;
    }
    Console::Log<Console::PrintType::OK>("Driver attached to FiveM successfully!\n");

    std::thread(Render).detach();
    //std::thread(Cache::cache).detach();
    //std::thread(entity_list::ped_space).detach();
    //std::thread(vehicle_list::vehicle_space).detach();

    while (true) {
        Sleep(10);
    }

    return 0;
}

void GetOverlay()
{
    const MARGINS Margin = { -1 };

    overlay_hwnd = FindWindowA("MedalOverlayClass", "MedalOverlay");
    if (!overlay_hwnd) {
        ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
        Console::Log<Console::PrintType::FAIL>("0x01 Medal wasn't found. Make sure Medal is open and running. Please refer to the wiki if you need help. Restart your computer if the problem persists, otherwise contact us on discord. The software will close in 10 seconds.\n");
        Sleep(10000);
        exit(0);
        return;
    }

    Console::Log<Console::PrintType::OK>("Medal Overlay has been found successfully!\n");

    if (DwmExtendFrameIntoClientArea(overlay_hwnd, &Margin) != S_OK) {
        ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
        Console::Log<Console::PrintType::FAIL>("0x01 Failed to initialize the Medal overlay. Please refer to the wiki if you need help. Restart your computer if the problem persists, otherwise contact us on discord. The software will close in 10 seconds.\n");
        Sleep(10000);
        exit(0);
        return;
    }

    Cache::menu_hwnd = overlay_hwnd;
    Console::Log<Console::PrintType::OK>("Medal Overlay has been extended into client area via DWM successfully!\n");

    SetWindowPos(overlay_hwnd, NULL, 0, 0, 1920, 1080, SWP_HIDEWINDOW);
    UpdateWindow(overlay_hwnd);
    SetWindowLong(overlay_hwnd, GWL_EXSTYLE, (int)GetWindowLong(overlay_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
    SetLayeredWindowAttributes(overlay_hwnd, 0, 255, LWA_ALPHA);
    SetLayeredWindowAttributes(overlay_hwnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
    UpdateWindow(overlay_hwnd);
}

bool GhostCallback(ULONG64* param1, ULONG64* param2, ULONG64 allocationPtr, ULONG64 allocationSize, ULONG64 mdlptr) {
    UNREFERENCED_PARAMETER(param1);
    UNREFERENCED_PARAMETER(param2);
    UNREFERENCED_PARAMETER(allocationPtr);
    UNREFERENCED_PARAMETER(allocationSize);
    UNREFERENCED_PARAMETER(mdlptr);

    return true;
}

void MapDriver()
{
    bool free = false, mdlMode = false, indPagesMode = false, passAllocationPtr = false;

    iqvw64e_device_handle = intel_driver::Load();

    if (iqvw64e_device_handle == INVALID_HANDLE_VALUE)
        return;

    std::vector<uint8_t> raw_image = driver_bytes;

    kdmapper::AllocationMode mode = kdmapper::AllocationMode::AllocatePool;

    if (mdlMode && indPagesMode) {
        intel_driver::Unload(iqvw64e_device_handle);
        return;
    }
    else if (mdlMode) {
        mode = kdmapper::AllocationMode::AllocateMdl;
    }
    else if (indPagesMode) {
        mode = kdmapper::AllocationMode::AllocateIndependentPages;
    }

    NTSTATUS exitCode = 0;
    if (!kdmapper::MapDriver(iqvw64e_device_handle, raw_image.data(), 0, 0, free, true, mode, passAllocationPtr, GhostCallback, &exitCode)) {
        intel_driver::Unload(iqvw64e_device_handle);
        return;
    }
}