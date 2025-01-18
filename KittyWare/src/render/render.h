#pragma once
#include <d3d9.h>
#include <memory/memory.h>
#include <cache.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>
#include <raw/logo.h>
#include <raw/mont.h>
#include <d3dx9tex.h>
#include <raw/icons.h>
#include <imgui/imgui_internal.h>
#include <raw/hashes.h>

HWND hwnd;
ImGuiIO* hook_io = nullptr;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImVec2 menu_pos;
ImVec2 menu_size;
void NavbarMenu();

ImFont* fa;

ImVec4 fix(int w, int x, int y, int z) {
    return ImVec4(w * (1.0f / 255.0f), x * (1.0f / 255.0f), y * (1.0f / 255.0f), z * (1.0f / 255.0f));
}

enum heads {
    HEAD_LOCAL,
    HEAD_AIM,
    HEAD_VISUAL,
    HEAD_WORLD,
    HEAD_MISCELLANEOUS,
    HEAD_SETTINGS,
    HEAD_EXIT
};

static heads tab = HEAD_LOCAL;

auto* Logo_Texture = LPDIRECT3DTEXTURE9();

inline ImFont* icons_font = nullptr;

inline int Render() {
    using namespace ImGui;

    static bool min_size_set = false;
    static bool is_menu_visible = true;

    hwnd = Cache::menu_hwnd;

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    CreateContext();
    ImGuiIO& io = GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = nullptr;    io.LogFilename = nullptr;

    io.Fonts->AddFontFromMemoryTTF(Sans, sizeof Sans, 22.0f);

    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3;
    icons_config.OversampleV = 3;
    fa = io.Fonts->AddFontFromMemoryTTF(&font_awesome, sizeof font_awesome, 26.f, &icons_config, icon_ranges);

    hook_io = &GetIO();

    StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImGuiStyle& style = GetStyle();
    style.Colors[ImGuiCol_WindowBg] = fix(35, 35, 35, 255);

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

    style.TabRounding = 20.f;
    style.FrameRounding = 20.f;
    style.GrabRounding = 20.f;
    style.WindowRounding = 20.f;
    style.PopupRounding = 20.f;

    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, Logo, sizeof Logo, &Logo_Texture);

    bool done = false;
    while (!done) {
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            long lWinLong = GetWindowLong(hwnd, GWL_EXSTYLE);
            if (!is_menu_visible) {
                is_menu_visible = !is_menu_visible;
                if (lWinLong != (WS_EX_LAYERED | WS_EX_TOOLWINDOW))
                    SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW);
                SetForegroundWindow(hwnd);
            }
            else {
                is_menu_visible = !is_menu_visible;

                if (lWinLong != (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT))
                    SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT);
            }
        }

        if (done)
            break;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        NewFrame();

        POINT p;

        if (GetCursorPos(&p)) {
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
            if (!first_show) {
                ShowWindow(::GetConsoleWindow(), SW_HIDE);
                ShowWindow(hwnd, SW_SHOW);
                first_show = true;
            }
            HCURSOR hCursor = LoadCursor(NULL, IDC_CROSS);
            SetCursor(hCursor);

            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

            SetNextWindowSize(ImVec2(1000, 500));
            Begin(
                "KittyWare",
                NULL,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize
            );

            menu_pos = GetWindowPos();
            menu_size = GetWindowSize();

            NavbarMenu();
            
            switch (tab) {
            case HEAD_LOCAL:
                ImGui::BeginChild("##local", ImVec2(ImGui::GetWindowWidth() - 19, ImGui::GetWindowHeight() - 55));

                float childHeight = ImGui::GetContentRegionAvail().y / 3.0f;

                ImGui::BeginChild("Self", ImVec2(0, childHeight));
                ImGui::Text("Self Content Here");
                ImGui::EndChild();

                ImGui::Spacing();

                ImGui::BeginChild("Weapon", ImVec2(0, childHeight));
                ImGui::Text("Weapon Content Here");
                ImGui::EndChild();

                ImGui::Spacing();

                ImGui::BeginChild("Vehicle", ImVec2(0, childHeight));
                ImGui::Text("Vehicle Content Here");
                ImGui::EndChild();

                ImGui::EndChild();
            case HEAD_AIM:
            case HEAD_VISUAL:
            case HEAD_WORLD:
            case HEAD_MISCELLANEOUS:
            case HEAD_EXIT:
                printf("ew");
            }
            
            End();

            PopStyleVar();
        }

        EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0) {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(GetDrawData());
            g_pd3dDevice->EndScene();
        }


        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    DestroyContext();

    //UnhookWindowsHookEx(hMouseHook);

    CleanupDeviceD3D();
    DestroyWindow(hwnd);

    return 0;
}
inline void NavbarMenu() {
    using namespace ImGui;
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
    PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    SetNextWindowPos(ImVec2(menu_pos.x, menu_pos.y - 80), ImGuiCond_Always);
    Begin(
        "NavbarMenu",
        nullptr,
        ImGuiWindowFlags_NoDecoration
    );
    SetWindowSize({ 1000, 70 });

    Image(reinterpret_cast<ImTextureID>(Logo_Texture), ImVec2(100, 50));

    float windowHeight = GetWindowHeight();
    float buttonHeight = 50;
    float verticalOffset = (windowHeight - buttonHeight) * 0.5f;

    SameLine();
    SetCursorPosX(GetWindowWidth() - 410);
    SetCursorPosY(verticalOffset);

    ImGui::PushFont(fa);

    auto RenderTabButton = [&](heads tabType, const char* icon, const char* label) {
        if (tab == tabType) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        }

        if (ImGui::Button(icon, ImVec2(50, buttonHeight))) {
            tab = tabType;
        }
        ImGui::PopStyleColor();
        ImGui::SameLine();
    };

    RenderTabButton(HEAD_LOCAL, ICON_FA_USER, "Local");
    RenderTabButton(HEAD_AIM, ICON_FA_CROSSHAIRS, "Aim");
    RenderTabButton(HEAD_VISUAL, ICON_FA_EYE, "Visual");
    RenderTabButton(HEAD_WORLD, ICON_FA_GLOBE_AMERICAS, "World");
    RenderTabButton(HEAD_MISCELLANEOUS, ICON_FA_BOXES, "Miscellaneous");
    RenderTabButton(HEAD_SETTINGS, ICON_FA_COG, "Settings");
    RenderTabButton(HEAD_EXIT, ICON_FA_SIGN_OUT_ALT, "Exit");

    ImGui::PopFont();

    End();
    PopStyleVar(2);
    PopStyleColor(3);
}

inline bool CreateDeviceD3D(HWND hWnd) {
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
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