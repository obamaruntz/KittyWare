#pragma once
#include <d3d9.h>
#include <memory/memory.h>
#include <misc/elements.h>
#include <cache.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>
#include <raw/logo.h>
#include <d3dx9tex.h>

HWND hwnd;
ImGuiIO* hook_io = nullptr;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImVec4 fix(int w, int x, int y, int z) {
    return ImVec4(w * (1.0f / 255.0f), x * (1.0f / 255.0f), y * (1.0f / 255.0f), z * (1.0f / 255.0f));
}

enum heads {
    HEAD_LOCAL,
    HEAD_VISUAL,
    HEAD_AIM,
    HEAD_TELEPORT,
    HEAD_WORLD,
    HEAD_MISCELLANEOUS,
    HEAD_SETTINGS,
    HEAD_EXIT
};

auto* Logo_Texture = LPDIRECT3DTEXTURE9();

inline void Render() {
    using namespace ImGui;

    static bool min_size_set = false;
    static bool is_menu_visible = true;

	Cache::menu_hwnd = hwnd;

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
    }

    IMGUI_CHECKVERSION();
    CreateContext();
    ImGuiIO& io = GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    hook_io = &GetIO();

    StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImGuiStyle& style = GetStyle();
    style.Colors[ImGuiCol_WindowBg] = fix(15, 15, 15, 255);

    style.Colors[ImGuiCol_Button] = fix(23, 23, 23, 255);
    style.Colors[ImGuiCol_ButtonHovered] = fix(28, 28, 28, 255);
    style.Colors[ImGuiCol_ButtonActive] = fix(33, 33, 33, 255);

    style.Colors[ImGuiCol_FrameBg] = fix(23, 23, 23, 255);
    style.Colors[ImGuiCol_FrameBgHovered] = fix(28, 28, 28, 255);
    style.Colors[ImGuiCol_FrameBgActive] = fix(33, 33, 33, 255);

    style.Colors[ImGuiCol_ScrollbarGrab] = fix(33, 33, 33, 255);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = fix(38, 38, 38, 255);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = fix(43, 43, 43, 255);

    style.Colors[ImGuiCol_PopupBg] = fix(17, 17, 17, 255);
    style.Colors[ImGuiCol_Border] = fix(37, 37, 37, 255);

    style.PopupRounding = 8.f;

    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    io.IniFilename = nullptr; // no imgui.ini file will be created

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, Logo, sizeof Logo, &Logo_Texture);

    static heads tab = HEAD_LOCAL;

    bool z = false;
    while (!z) {

        if (GetAsyncKeyState(VK_END) & 1 || GetAsyncKeyState(VK_INSERT) & 1) {
            long lWinLong = GetWindowLong(hwnd, GWL_EXSTYLE);
            if (!is_menu_visible)
            {
                is_menu_visible = !is_menu_visible;
                if (lWinLong != (WS_EX_LAYERED | WS_EX_TOOLWINDOW))
                    SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW);
                SetForegroundWindow(hwnd);
            }
            else
            {
                is_menu_visible = !is_menu_visible;

                if (lWinLong != (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT))
                    SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT);
            }
        }

        if (z) break;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        NewFrame();

        POINT p;

        if (GetCursorPos(&p))
        {
            io.MousePos.x = p.x;
            io.MousePos.y = p.y;
        }

        io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
        io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
        io.MouseDown[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
        io.MouseDown[3] = GetAsyncKeyState(VK_XBUTTON1) & 0x8000;
        io.MouseDown[4] = GetAsyncKeyState(VK_XBUTTON2) & 0x8000;

        static bool first_show = false;
        if (is_menu_visible) {
            if (!first_show) first_show = true;

            HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);
            SetCursor(hCursor);
            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            Begin(
                "KittyWare",
                NULL,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize
            );

            auto window = ImGui::GetCurrentWindow();
            auto draw = window->DrawList;
            auto pos = window->Pos;
            auto size = window->Size;

            BeginGroup(); {
                Image(reinterpret_cast<void*>(Logo_Texture), ImVec2(300, 300));

                if (Elements::Tab("Local", tab == HEAD_LOCAL), tab == HEAD_LOCAL);
                NewLine();
                if (Elements::Tab("Visual", tab == HEAD_VISUAL), tab == HEAD_VISUAL);
                NewLine();
                if (Elements::Tab("Aim", tab == HEAD_AIM), tab == HEAD_AIM);
                NewLine();
                if (Elements::Tab("World", tab == HEAD_WORLD), tab == HEAD_WORLD);
                NewLine();
                if (Elements::Tab("Misc", tab == HEAD_MISCELLANEOUS), tab == HEAD_MISCELLANEOUS)
                NewLine();
                if (Elements::Tab("Settings", tab == HEAD_SETTINGS), tab == HEAD_SETTINGS)

                SetCursorPos({ size.x - 35, 13 });
                if (Elements::Tab("Exit", tab == HEAD_EXIT), tab == HEAD_EXIT);
            }
            EndGroup();

            switch(tab) {
            case HEAD_LOCAL:

                break;
            case HEAD_VISUAL:
                break;
            case HEAD_AIM:
                break;
            case HEAD_TELEPORT:
                break;
            case HEAD_WORLD:
                break;
            case HEAD_MISCELLANEOUS:
                break;
            case HEAD_SETTINGS:
                break;
            case HEAD_EXIT:
                break;
            }
        }
    }
}

inline bool CreateDeviceD3D(HWND hWnd) {
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    g_d3dpp.BackBufferCount = 2;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // vsync
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D() {
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice() {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}