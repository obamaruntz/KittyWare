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
#include <raw/veh.h>
#include <cheat/settings.h>
#include <features/visual.h>
#include <raw/espfont.h>
#include <raw/gear.h>
#include <functional>
#include <render/hotkey.h>

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
ImVec2 main_pos;
ImVec2 main_size;

void NavbarMenu();
void RenderPedESPPreview();
void RenderVehESPPreview();
void SettingsButton(const char* PopupName, const ImVec2& PopupSize, std::function<void()> ContentProvider);
void MultiSelectCombo(const char* label, std::vector<const char*>& items, std::vector<bool>& selection);

ImFont* fa;
ImFont* espfont;

auto* Logo_Texture = LPDIRECT3DTEXTURE9();
auto* Veh_Texture = LPDIRECT3DTEXTURE9();
auto* Cog_Texture = LPDIRECT3DTEXTURE9();

ImVec4 fix(int w, int x, int y, int z) {
    return ImVec4(w * (1.0f / 255.0f), x * (1.0f / 255.0f), y * (1.0f / 255.0f), z * (1.0f / 255.0f));
}

ImU32 f_2_imcol(const float color[4]) {
    return IM_COL32(
        (int)(color[0] * 255),
        (int)(color[1] * 255),
        (int)(color[2] * 255),
        (int)(color[3] * 255)
    );
}

int calc_text_size(double distance) {
    const int maxTextSize = 20;
    const int minTextSize = 14;

    int textSize = static_cast<int>((maxTextSize - minTextSize) * (1.0 / (1 + distance)) + minTextSize);

    textSize = std::min<int>(maxTextSize, std::max<int>(minTextSize, textSize));

    return textSize;
}

struct ped_struct {
    Vector2 screen_head, screen_position, screen_foot;
    Vector3 head, origin;
    float height, width, width_real, angle;
    int distance, health, armor, net_id, index;
    std::vector<std::pair<Vector2, Vector2>> bones;
    uint32_t weapon_name_hash;
    std::string weapon_name, player_name, license;
    bool is_npc, is_visible, is_friend;
};

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

    ImFont* default_f = io.Fonts->AddFontFromMemoryTTF(Sans, sizeof Sans, 20.0f);
    ImFont* small_f = io.Fonts->AddFontFromMemoryTTF(Sans, sizeof Sans, 16.0f);
    ImFont* larger_sf = io.Fonts->AddFontFromMemoryTTF(Sans, sizeof Sans, 22.0f);
    ImFont* larger_f = io.Fonts->AddFontFromMemoryTTF(Sans, sizeof Sans, 26.0f);
    espfont = io.Fonts ->AddFontFromMemoryTTF(esp_font, sizeof esp_font, 14.0f);

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
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.729411780834198f, 0.7490196228027344f, 0.7372549176216125f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08627451211214066f, 0.08627451211214066f, 0.08627451211214066f, 0.9399999976158142f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5400000214576721f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.1764705926179886f, 0.4000000059604645f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 0.6700000166893005f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 0.6523605585098267f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 0.6700000166893005f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.0f, 0.0f, 0.5411764979362488f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.1764705926179886f, 0.4000000059604645f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 0.6705882549285889f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2705882489681244f, 0.2705882489681244f, 0.2705882489681244f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3529411852359772f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.1502146124839783f, 0.1502131074666977f, 0.1502131074666977f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.9227467775344849f, 0.9227375388145447f, 0.9227375388145447f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.2196078449487686f, 0.2196078449487686f, 0.2196078449487686f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2901960909366608f, 0.2901960909366608f, 0.2901960909366608f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.1764705926179886f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1974248886108398f, 0.1974229067564011f, 0.1974229067564011f, 0.9700000286102295f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7081544995307922f, 0.708147406578064f, 0.708147406578064f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.364705890417099f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.4763948321342468f, 0.4763900637626648f, 0.4763900637626648f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2627451121807098f, 0.6352941393852234f, 0.8784313797950745f, 0.4377682209014893f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 0.9656652212142944f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
    
    style.TabRounding = 20.f;
    style.FrameRounding = 20.f;
    style.GrabRounding = 20.f;
    style.WindowRounding = 20.f;
    style.PopupRounding = 20.f;
    style.ChildRounding = 12.f;
    style.WindowPadding = ImVec2(5.f, 5.f);
    style.DisplayWindowPadding = ImVec2(5.f, 5.f);
    style.DisplaySafeAreaPadding = ImVec2(5.f, 5.f);

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.6000000238418579f;
    style.WindowPadding = ImVec2(6.0f, 3.0f);
    style.WindowRounding = 10.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 6.0f;
    style.ChildBorderSize = 0.0f;
    style.PopupRounding = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(5.0f, 1.0f);
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.CellPadding = ImVec2(4.0f, 2.0f);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 13.0f;
    style.ScrollbarRounding = 16.0f;
    style.GrabMinSize = 20.0f;
    style.GrabRounding = 2.0f;
    style.TabRounding = 4.0f;
    style.TabBorderSize = 1.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, Logo, sizeof Logo, &Logo_Texture);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, VehESPPreview_Image, sizeof VehESPPreview_Image, &Veh_Texture);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, Gear_Data, sizeof Gear_Data, &Cog_Texture);

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

        Visual::RenderAllVisuals(ImGui::GetBackgroundDrawList(), ImGui::GetIO());

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
            SetNextWindowPos(ImVec2(menu_pos.x, menu_pos.y + 80), ImGuiCond_Always);
            Begin(
                "KittyWare",
                NULL,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_AlwaysUseWindowPadding
            );

            main_pos = GetWindowPos();
            main_size = GetWindowSize();

            NavbarMenu();

            switch (tab) {
            case HEAD_LOCAL: {
                ImGui::BeginChild("##local", ImVec2(ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight() - 20));
                float spacing = 10.f;
                float totalSpacing = spacing * 2;
                float windowWidth = (ImGui::GetContentRegionAvail().x - totalSpacing) / 3.0f;
                PushStyleColor(ImGuiCol_ChildBg, fix(25, 25, 25, 255));

                BeginChild("Self", ImVec2(windowWidth, 0));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Self");
                PopFont();
                Separator();

                Indent(10);
                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Text("Run Speed Modifier");
                SliderFloat("##runsm", &Settings::run_speed, 0.1f, 5.f, "%.1fx");
                Text("Swim Speed Modifier");
                SliderFloat("##swimsm", &Settings::swim_speed, 0.1f, 5.f, "%.1fx");
                Separator();
                Checkbox("True Godmode", &Settings::godmode);
                Checkbox("Demi Godmode", &Settings::demigod);
                Checkbox("Superjump", &Settings::superjump);
                Checkbox("Heal Key", &Settings::healkey);
                ImGui::SameLine();
                Hotkeys::Hotkey(&Keys::heal_key, "##dddddsa", &Hotkeys::heal_key_bool, "f", 15);
                SameLine(GetWindowWidth() - 30);
                SettingsButton("HealthKeyPopup", { 200, 150 }, [larger_sf, small_f]() {
                    PushFont(larger_sf);
                    Text("Health Key Settings");
                    PopFont();
                    Separator();
                    PushItemWidth(180.f);
                    PushFont(small_f);
                    Text("Health Amount");
                    SliderInt("##ham", &Settings::healkey_health_amount, 1, 200);
                    Text("Armour Amount");
                    SliderInt("##aam", &Settings::healkey_armor_amount, 1, 100);
                    PopFont();
                    PopItemWidth();
                });

                PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));
                Separator();
                PopStyleVar();

                Checkbox("Noclip", &Settings::noclip);
                Text("Noclip Speed");
                SliderFloat("##nclipsped", &Settings::noclip_speed, 0.1f, 5.f, "%.1fx");
                Separator();
                if (Button("Teleport to Waypoint", ImVec2(GetContentRegionAvail().x - 10, 0))) {}
                if (Button("Fill Health", ImVec2(GetContentRegionAvail().x - 10, 0))) {}
                if (Button("Fill Armour", ImVec2(GetContentRegionAvail().x - 10, 0))) {}
                if (Button("Suicide", ImVec2(GetContentRegionAvail().x - 10, 0))) {}
                PopItemWidth();
                EndChild();

                SameLine(windowWidth + spacing);
                BeginChild("Weapon", ImVec2(windowWidth, 0));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Weapon");
                PopFont();
                Separator();
                Indent(10);

                Text("Weapon Damage Modifier");
                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                SliderFloat("##wdm", &Settings::weap_dmg, 0.1f, 5.f, "%.1fx");
                Text("Melee Damage Modifier");
                SliderFloat("##mdm", &Settings::melee_dmg, 0.1f, 5.f, "%.1fx");
                Checkbox("Remove Recoil", &Settings::no_recoil);
                Checkbox("Remove Spread", &Settings::no_spread);
                Checkbox("Remove Reload", &Settings::no_reload);
                Checkbox("Quick Reload", &Settings::fast_reload);
                Checkbox("Remove Range Limit", &Settings::no_range_limit);
                PopItemWidth();
                EndChild();

                SameLine((windowWidth + spacing) * 2);
                BeginChild("Vehicle", ImVec2(windowWidth, 0));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Vehicle");
                PopFont();
                Separator();
                Indent(10);
                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Checkbox("Vehicle Godmode", &Settings::veh_godmode);
                Checkbox("Horn Boost", &Settings::horn_boost);
                Text("Accelaration Modifier");
                SliderFloat("##accmd", &Settings::veh_accel, 1.f, 1000.f, "%.1fx");
                Text("Break Force Modifier");
                SliderFloat("##dddd", &Settings::veh_break, 1.f, 1000.f, "%.1fx");
                Text("Downforce Modifier");
                SliderFloat("##downmd", &Settings::veh_down, 1.f, 1000.f, "%.1fx");
                PopItemWidth();
                EndChild();

                PopStyleColor();
                EndChild();
                break;
            }
            case HEAD_AIM: {
                ImGui::BeginChild("##aim", ImVec2(ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight() - 20));
                float windowWidth = (ImGui::GetContentRegionAvail().x - 10.f) / 2.0f;
                PushStyleColor(ImGuiCol_ChildBg, fix(25, 25, 25, 255));

                BeginChild("AimA", ImVec2(windowWidth, 0));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Aim Assistance");
                PopFont();
                Separator();

                Indent(10);
                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Checkbox("Aimbot (Hold)", &Settings::godmode);
                ImGui::SameLine(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize("[...]").x - 10);
                ImGui::Text("[...]");
                Checkbox("Magic Bullet", &Settings::demigod);
                Text("Force Body Aim");
                ImGui::SameLine(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize("[...]").x - 10);
                ImGui::Text("[...]");
                Checkbox("Ignore Dead", &Settings::ignore_dead_aim);
                Checkbox("Ignore NPCs", &Settings::ignore_npc_aim);
                Checkbox("Ignore Friends", &Settings::ignore_friend_aim);
                Checkbox("Ignore Unarmed", &Settings::ignore_without_weapon_aim);
                PopItemWidth();
                EndChild();

                SameLine(windowWidth + 10.f);
                BeginChild("AimS", ImVec2(windowWidth, 0));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Aim Settings");
                PopFont();
                Separator();
                Indent(10);

                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Text("Field Of View");
                SliderInt("##fov", &Settings::fov, 1, ImGui::GetIO().DisplaySize.x / 2, "%dpx");
                Text("Horizontal Smoothing");
                SliderInt("##hs", &Settings::smoothing_x, 0, 100, "%d%%");
                Text("Vertical Smoothing");
                SliderInt("##vs", &Settings::smoothing_y, 0, 100, "%d%%");

                Text("Maximum Distance");
                SliderInt("##axd", &Settings::aim_max_distance, 1, 1500, "%d%m");
                Text("Miss Chance");
                SliderInt("##missc", &Settings::miss_chance, 0, 100, "%d%%");

                Text("Target Bone");
                const char* bonesNames[] = {
                                "Head",
                                "Left Foot",
                                "Right Foot",
                                "Left Ankle",
                                "Right Ankle",
                                "Left Hand",
                                "Right Hand",
                                "Neck",
                                "Abdomen",
                                "Left Shoulder",
                                "Right Shoulder",
                                "Left Hip",
                                "Right Hip",
                                "Closest Bone",
                };

                Combo("##eee", &Settings::bone, bonesNames, IM_ARRAYSIZE(bonesNames));
                
                Text("Closest Bones Filter");
                static std::vector<const char*> closestBonesNames = { "Head","Left Foot","Right Foot","Left Ankle","Right Ankle","Left Hand","Right Hand","Neck","Abdomen"};
                MultiSelectCombo("##clclwcju", closestBonesNames, Settings::closest_bones_filter);

                PopItemWidth();
                EndChild();

                PopStyleColor();
                EndChild();
                break;
            }
            case HEAD_VISUAL: {
                ImGui::BeginChild("##visual", ImVec2(ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight() - 20));
                float spacing = 10.f;
                float totalSpacing = spacing * 2;
                float windowWidth = (ImGui::GetContentRegionAvail().x - totalSpacing) / 3.0f;
                PushStyleColor(ImGuiCol_ChildBg, fix(25, 25, 25, 255));

                RenderPedESPPreview();
                RenderVehESPPreview();

                BeginChild("PEDESP", ImVec2(windowWidth, 0));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Ped ESP");
                PopFont();
                Separator();

                Indent(10);
                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Text("Maximum Distance");
                SliderInt("##axdzz", &Settings::esp_max_distance, 1, 1500, "%d%m");
                Checkbox("Player Names", &Settings::draw_player_name);
                Checkbox("Player Weapons", &Settings::draw_weapon_name);
                Checkbox("Player Distance", &Settings::draw_distance);
                Checkbox("Player NetID", &Settings::draw_net_id);
                Checkbox("Ignore Self", &Settings::ignore_self);
                Checkbox("Ignore NPCs", &Settings::ignore_npcs);
                Checkbox("Ignore Dead", &Settings::ignore_dead);
                Checkbox("Bounding Box", &Settings::bounding_box);
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##fzff", Settings::bounding_box_col, ImGuiColorEditFlags_NoInputs);
                Checkbox("Filled Box", &Settings::filled_box);
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##f22ff", Settings::filled_box_col, ImGuiColorEditFlags_NoInputs);
                Checkbox("Skeleton", &Settings::skeletons);
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##fdwff", Settings::skeleton_col, ImGuiColorEditFlags_NoInputs);
                Checkbox("Healthbar", &Settings::healthbar);
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##fwwff", Settings::healthbar_col, ImGuiColorEditFlags_NoInputs);
                Checkbox("Armourbar", &Settings::armourbar);
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##ffddf", Settings::armourbar_col, ImGuiColorEditFlags_NoInputs);
                Checkbox("Hide Bar when empty", &Settings::emptybars);
                PopItemWidth();
                EndChild();

                SameLine(windowWidth + spacing);
                BeginChild("VEHESP", ImVec2(windowWidth, ImGui::GetContentRegionAvail().x - 424));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Vehicle ESP");
                PopFont();
                Separator();
                Indent(10);

                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Text("Maximum Distance");
                SliderInt("##axzzdzdsz", &Settings::veh_esp_max_distance, 1, 1500, "%d%m");
                Checkbox("Ignore Own Vehicle", &Settings::no_recoil);
                Checkbox("Driver Status", &Settings::v_driver_status);
                Checkbox("Nametags", &Settings::v_nametags);
                Checkbox("Lock Status", &Settings::v_lockstatus);
                Checkbox("Distance", &Settings::v_distance);
                PopItemWidth();
                EndChild();

                SameLine(windowWidth + spacing);
                SetCursorPosY(ImGui::GetCursorPosY() + 235);

                BeginChild("otherexsp", ImVec2(windowWidth, 148));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Friend ESP");
                PopFont();
                Separator();
                Indent(10);

                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Checkbox("Friend Override", &Settings::friend_override);
                Text("Bounding Box");
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##fwddwff", Settings::friend_bounding_box, ImGuiColorEditFlags_NoInputs);
                Text("Filled Box");
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##fw22wff", Settings::friend_filled_box, ImGuiColorEditFlags_NoInputs);
                Text("Skeleton");
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##fwzxwff", Settings::friend_skeleton, ImGuiColorEditFlags_NoInputs);
                PopItemWidth();
                EndChild();

                SameLine(windowWidth + spacing);
                SetCursorPosY(ImGui::GetCursorPosY() + 393);

                BeginChild("otherssexsp", ImVec2(windowWidth, 0));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Aim Options");
                PopFont();
                Separator();
                Indent(10);

                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Checkbox("Draw FOV", &Settings::draw_fov);
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##fwwff", Settings::fov_color, ImGuiColorEditFlags_NoInputs);
                PopItemWidth();
                EndChild();

                SameLine((windowWidth + spacing) * 2);
                BeginChild("extraopts", ImVec2(windowWidth, ImGui::GetContentRegionAvail().x - 145));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Extra Options");
                PopFont();
                Separator();
                Indent(10);
                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Checkbox("Draw Player Info Widget", &Settings::draw_ped_info);
                Checkbox("Draw Spectator List", &Settings::draw_spec_list);
                Checkbox("Draw Crosshair (dot)", &Settings::xhair);
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
                ImGui::ColorEdit4("##fwwfdddf", Settings::xhair_color, ImGuiColorEditFlags_NoInputs);
                Checkbox("Draw Players Looking Warning", &Settings::draw_look_warn);
                Checkbox("Draw Hitmarkers", &Settings::hitmarkers);
                PopItemWidth();
                EndChild();

                SameLine((windowWidth + spacing) * 2);
                SetCursorPosY(ImGui::GetCursorPosY() + 180);

                BeginChild("otherszzsexsp", ImVec2(windowWidth, 130));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Damage Numbers");
                PopFont();
                Separator();
                Indent(10);

                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Checkbox("Draw Damage Numbers", &Settings::draw_dmg_nums);
                Text("Damage Number Duration");
                SliderInt("##ddd", &Settings::dmg_nums_length, 1, 10, "%ds");
                PopItemWidth();
                EndChild();


                SameLine((windowWidth + spacing) * 2);
                SetCursorPosY(ImGui::GetCursorPosY() + 316);

                BeginChild("otherszzzzzsexsp", ImVec2(windowWidth, 0));
                SetCursorPosY(ImGui::GetCursorPosY() + 5);
                SetCursorPosX(ImGui::GetCursorPosX() + 10);
                PushFont(larger_f);
                Text("Bullet Tracers");
                PopFont();
                Separator();
                Indent(10);

                PushItemWidth(GetContentRegionAvail().x - 10.0f);
                Checkbox("Enable Tracers", &Settings::tracers);
                Text("Damage Number Duration");
                SliderInt("##ddd", &Settings::tracers_length, 1, 10, "%ds");
                PopItemWidth();
                EndChild();

                PopStyleColor();
                EndChild();
                break;
            }
            case HEAD_WORLD: {
                ImGui::BeginChild("##world", ImVec2(ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight() - 20));
                {
                    float windowWidth = (ImGui::GetContentRegionAvail().x - 10.f) / 2.0f;
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertFloat4ToU32(ImVec4(25 / 255.f, 25 / 255.f, 25 / 255.f, 1.0f)));

                    if (ImGui::BeginChild("PEDList", ImVec2(windowWidth, 0), true))
                    {
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                        ImGui::PushFont(larger_f);
                        ImGui::Text("Ped List");
                        ImGui::PopFont();
                        ImGui::Separator();
                        ImGui::Indent(3);

                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 3);
                        if (ImGui::BeginListBox("##ped list", ImVec2(windowWidth - 6.f, ImGui::GetContentRegionAvail().y - 105.f)))
                        {
                            ImGui::EndListBox();
                        }
                        ImGui::PopItemWidth();

                        ImGui::Button("Teleport to Ped", ImVec2(GetContentRegionAvail().x - 4, 0));
                        ImGui::Button("Add to Friend List", ImVec2(GetContentRegionAvail().x - 4, 0));

                        ImGui::Text("Friend Type");
                        const char* friendType[] = {
                            "License (reliable)",
                            "Net ID",
                        };
                        ImGui::Combo("##eezz22e", &Settings::f_type, friendType, IM_ARRAYSIZE(friendType));
                    }
                    ImGui::EndChild();

                    ImGui::SameLine(0, 10.f);
                    if (ImGui::BeginChild("vehlist", ImVec2(windowWidth, 0), true))
                    {
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                        ImGui::PushFont(larger_f);
                        ImGui::Text("Vehicle List");
                        ImGui::PopFont();
                        ImGui::Separator();
                        ImGui::Indent(3);

                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15);
                        if (ImGui::BeginListBox("##veh list", ImVec2(windowWidth - 6.f, ImGui::GetContentRegionAvail().y - 133.f)))
                        {
                            ImGui::EndListBox();
                        }
                        ImGui::PopItemWidth();

                        ImGui::Button("Unlock Vehicle", ImVec2(GetContentRegionAvail().x - 4, 0));
                        ImGui::Button("Lock Vehicle", ImVec2(GetContentRegionAvail().x - 4, 0));
                        ImGui::Button("Teleport to Vehicle", ImVec2(GetContentRegionAvail().x - 4, 0));
                        ImGui::Button("Teleport into Air", ImVec2(GetContentRegionAvail().x - 4, 0));
                        ImGui::Button("Set On Fire", ImVec2(GetContentRegionAvail().x - 4, 0));
                    }
                    ImGui::EndChild();

                    ImGui::PopStyleColor();
                }
                ImGui::EndChild();
                break;
            }
            case HEAD_SETTINGS: {
                ImGui::BeginChild("##world", ImVec2(ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight() - 20));
                {
                    float windowWidth = (ImGui::GetContentRegionAvail().x - 10.f) / 2.0f;
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertFloat4ToU32(ImVec4(25 / 255.f, 25 / 255.f, 25 / 255.f, 1.0f)));


                    ImGui::SameLine(0, 10.f);
                    if (ImGui::BeginChild("configmenu", ImVec2(windowWidth, 0), true))
                    {
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                        ImGui::PushFont(larger_f);
                        ImGui::Text("Config Manager");
                        ImGui::PopFont();
                        ImGui::Separator();
                        ImGui::Indent(3);

                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15);
                        if (ImGui::BeginListBox("##veh list", ImVec2(windowWidth - 6.f, ImGui::GetContentRegionAvail().y - 133.f)))
                        {
                            ImGui::EndListBox();
                        }
                        ImGui::PopItemWidth();

                        ImGui::Button("Unlock Vehicle", ImVec2(GetContentRegionAvail().x - 4, 0));
                        ImGui::Button("Lock Vehicle", ImVec2(GetContentRegionAvail().x - 4, 0));
                        ImGui::Button("Teleport to Vehicle", ImVec2(GetContentRegionAvail().x - 4, 0));
                        ImGui::Button("Teleport into Air", ImVec2(GetContentRegionAvail().x - 4, 0));
                        ImGui::Button("Set On Fire", ImVec2(GetContentRegionAvail().x - 4, 0));
                    }
                    ImGui::EndChild();

                    ImGui::PopStyleColor();
                }
                ImGui::EndChild();
                break;
            }
            case HEAD_EXIT:
                ShowWindow(hwnd, SW_HIDE);
                exit(0);
                break;
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
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
    PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    Begin(
        "NavbarMenu",
        nullptr,
        ImGuiWindowFlags_NoDecoration
    );
    menu_pos = GetWindowPos();
    menu_size = GetWindowSize();

    SetWindowSize({ 1000, 70 });

    Image(reinterpret_cast<ImTextureID>(Logo_Texture), ImVec2(100, 50));

    float windowHeight = GetWindowHeight();
    float buttonHeight = 50;
    float verticalOffset = (windowHeight - buttonHeight) * 0.5f;

    SameLine();
    SetCursorPosX(GetWindowWidth() - 350);
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
    RenderTabButton(HEAD_SETTINGS, ICON_FA_COG, "Settings");
    RenderTabButton(HEAD_EXIT, ICON_FA_SIGN_OUT_ALT, "Exit");

    ImGui::PopFont();

    End();
    PopStyleVar(3);
    PopStyleColor(3);
}

void RenderPedESPPreview() {
    using namespace ImGui;

    ped_struct ped;

    ped.armor = 50;
    ped.health = 200;

    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));

    SetNextWindowPos(ImVec2(main_pos.x - 210, main_pos.y), ImGuiCond_Always);
    Begin(
        "Ped ESP Preview",
        nullptr,
        ImGuiWindowFlags_NoDecoration
    );
    Text("Ped ESP Preview");
    Separator();

    SetWindowSize({ 200, 300 });
    auto pos = GetWindowPos();
    auto size = GetWindowSize();

    ped.screen_head = { size.x / 2 + pos.x, pos.y + 50 };

    ped.width = size.x - 90;

    ped.height = 60;
    ped.distance = 10;
    ped.weapon_name = "Mosin Nagant";
    ped.player_name = "Stackz <3";

    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2 + pos.x, pos.y + 60 }, Vector2{ size.x / 2 + pos.x, pos.y + 70 }));
    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2 + pos.x, pos.y + 70 }, Vector2{ size.x / 2.45f + pos.x, pos.y + 80 })); // left shoulder
    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2.65f + pos.x, pos.y + 130 }, Vector2{ size.x / 2.45f + pos.x, pos.y + 80 })); // left arm
    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2 + pos.x, pos.y + 70 }, Vector2{ size.x / 2.45f + pos.x + (size.x / 2 - size.x / 2.45f) * 2, pos.y + 80 })); // right shoulder
    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2.65f + pos.x + (size.x / 2 - size.x / 2.65f) * 2, pos.y + 130 }, Vector2{ size.x / 2.45f + pos.x + (size.x / 2 - size.x / 2.45f) * 2, pos.y + 80 })); // right arm
    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2 + pos.x, pos.y + 70 }, Vector2{ size.x / 2 + pos.x, pos.y + 140 })); // spine
    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2.35f + pos.x, pos.y + 150 }, Vector2{ size.x / 2 + pos.x, pos.y + 140 })); // left hip
    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2.35f + pos.x + (size.x / 2 - size.x / 2.35f) * 2, pos.y + 150 }, Vector2{ size.x / 2 + pos.x, pos.y + 140 })); // right hip
    ped.bones.push_back(std::make_pair(Vector2{ (size.x / 2.45f) + (((size.x / 2) - (size.x / 2.45f)) * 2) + pos.x, pos.y + 205 }, Vector2{ size.x / 2.35f + pos.x + (size.x / 2 - size.x / 2.35f) * 2, pos.y + 150 })); // right ankle
    ped.bones.push_back(std::make_pair(Vector2{ (size.x / 2.55f) + (((size.x / 2) - (size.x / 2.55f)) * 2) + pos.x, pos.y + 210 }, Vector2{ (size.x / 2.45f) + (((size.x / 2) - (size.x / 2.45f)) * 2) + pos.x, pos.y + 205 })); // right ankle
    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2.45f + pos.x, pos.y + 205 }, Vector2{ size.x / 2.35f + pos.x, pos.y + 150 })); // left ankle
    ped.bones.push_back(std::make_pair(Vector2{ size.x / 2.45f + pos.x, pos.y + 205 }, Vector2{ size.x / 2.55f + pos.x, pos.y + 210 })); // left foot

    ImDrawList* draw = GetWindowDrawList();

    PushFont(espfont);

    if (Settings::bounding_box)
    {
        if (Settings::box_type == 0)
        {
            draw->AddRect(
                ImVec2(ped.screen_head.x - ped.width / 2, ped.screen_head.y),
                ImVec2(ped.screen_head.x - ped.width / 2 + ped.width, ped.screen_head.y + ped.height * 2.9f),
                f_2_imcol(Settings::bounding_box_col),
                0.0f, 0, 1
            );
        }
        else if (Settings::box_type == 1)
        {
            ImVec2 pos = ImVec2((ped.screen_head.x - ped.width / 2) + ped.width, (ped.screen_head.y) + ped.height * 2.9f);
            ImVec2 dim = ImVec2(ped.width, ped.height * 2.9f);
            float cornersizex = dim.x / 4;
            float cornersizey = dim.y / 10;
            auto col = f_2_imcol(Settings::bounding_box_col);

            draw->AddLine(ImVec2(pos), ImVec2(pos.x - cornersizex, pos.y), col, 1);
            draw->AddLine(ImVec2(pos.x - dim.x, pos.y), ImVec2((pos.x - dim.x) + cornersizex, pos.y), col, 1);
            draw->AddLine(ImVec2(pos.x - dim.x, pos.y), ImVec2(pos.x - dim.x, pos.y - cornersizey), col, 1);
            draw->AddLine(ImVec2(pos.x - dim.x, (pos.y - dim.y)), ImVec2(pos.x - dim.x, (pos.y - dim.y) + cornersizey), col, 1);
            draw->AddLine(ImVec2(pos.x - dim.x, (pos.y - dim.y)), ImVec2((pos.x - dim.x) + cornersizex, pos.y - dim.y), col, 1);
            draw->AddLine(ImVec2(pos.x, (pos.y - dim.y)), ImVec2(pos.x - cornersizex, pos.y - dim.y), col, 1);
            draw->AddLine(ImVec2(pos.x, (pos.y - dim.y)), ImVec2(pos.x, (pos.y - dim.y) + cornersizey), col, 1);
            draw->AddLine(ImVec2(pos.x, pos.y), ImVec2(pos.x, pos.y - cornersizey), col, 1);
        }
    }

    if (Settings::filled_box)
    {
        draw->AddRectFilled(
            ImVec2(ped.screen_head.x - ped.width / 2, ped.screen_head.y),
            ImVec2(ped.screen_head.x - ped.width / 2 + ped.width, ped.screen_head.y + ped.height * 2.9f),
            f_2_imcol(Settings::filled_box_col),
            0.0f, 0
        );
    }

    if (Settings::healthbar)
    {
        if (!(Settings::emptybars && ped.health == 0))
        {
            ImU32 healthbar_color = f_2_imcol(Settings::healthbar_col);

            float a[4] = { 28, 26.f, 26, 140 };
            draw->AddRectFilled(
                ImVec2(ped.screen_head.x - (ped.width / 2 + 10), ped.screen_head.y + (ped.height * (100 - 100) / 100)),
                ImVec2(ped.screen_head.x - (ped.width / 2 + 10) + 3, ped.screen_head.y + (ped.height * (100 - 100) / 100) + ped.height * 2.9f),
                f_2_imcol(a),
                0.0f, 0
            );

            draw->AddRectFilled(
                ImVec2(ped.screen_head.x - (ped.width / 2 + 10), ped.screen_head.y + (ped.height * (100 - 100) / 100)),
                ImVec2(ped.screen_head.x - (ped.width / 2 + 10) + 3, ped.screen_head.y + ((ped.height * 2.9f) * (200 - ped.health) / 200) + (ped.height * 2.9f) - ((ped.height * 2.9f) * (200 - ped.health) / 200)),
                healthbar_color,
                0.0f, 0
            );
        }
    }

    if (Settings::armourbar)
    {
        if (!(Settings::emptybars && ped.health == 0))
        {
            ImU32 armourbar_color = f_2_imcol(Settings::armourbar_col);

            float a[4] = { 28, 26.f, 26, 140 };
            draw->AddRectFilled(
                ImVec2(ped.screen_head.x + (ped.width / 2 + 7), ped.screen_head.y + (ped.height * (100 - 100) / 100)),
                ImVec2(ped.screen_head.x + (ped.width / 2 + 7) + 3, ped.screen_head.y + (ped.height * (100 - 100) / 100) + ped.height * 2.9f),
                f_2_imcol(a),
                0.0f, 0
            );

            draw->AddRectFilled(
                ImVec2(ped.screen_head.x + (ped.width / 2 + 7), ped.screen_head.y + ((ped.height * 2.9f) * (100 - ped.armor) / 100)),
                ImVec2(ped.screen_head.x + (ped.width / 2 + 7) + 3, ped.screen_head.y + ((ped.height * 2.9f) * (100 - ped.armor) / 100) + (ped.height * 2.9f) - ((ped.height * 2.9f) * (100 - ped.armor) / 100)),
                armourbar_color,
                0.0f, 0
            );
        }
    }

    if (Settings::skeletons) {
        for (const auto& bones : ped.bones) {
            draw->AddLine(ImVec2(bones.first.x, bones.first.y), ImVec2(bones.second.x, bones.second.y), f_2_imcol(Settings::skeleton_col), 2);
        }
    }

    if (Settings::draw_weapon_name || Settings::draw_distance || Settings::draw_player_name || Settings::draw_net_id) {
        std::vector<std::string> lines;

        if (Settings::draw_player_name)
            lines.push_back(ped.player_name);

        if (Settings::draw_weapon_name)
            lines.push_back(ped.weapon_name);

        if (Settings::draw_distance)
            lines.push_back("[" + std::to_string(ped.distance) + "m]");

        if (Settings::draw_net_id)
            lines.push_back("ID: " + std::to_string(ped.net_id));

        float text_y = ped.screen_head.y + ped.height * 3.0f;

        float white[4] = { 1, 1, 1, 1 };
        float black[4] = { 0, 0, 0, 1 };

        for (const auto& line : lines) {
            ImVec2 text_size = CalcTextSize(line.c_str());

            float text_x = ped.screen_head.x - (text_size.x / 2.0f);

            draw->AddText(
                espfont,
                calc_text_size(ped.distance),
                ImVec2(text_x, text_y + 1.f),
                f_2_imcol(black),
                line.c_str()
            );

            draw->AddText(
                espfont,
                calc_text_size(ped.distance),
                ImVec2(text_x, text_y),
                f_2_imcol(white),
                line.c_str()
            );

            text_y += text_size.y;
        }
    }

    PopFont();
    End();
    PopStyleVar(2);
}

void RenderVehESPPreview() {
    using namespace ImGui;
    SetNextWindowPos(ImVec2(main_pos.x + 1010, main_pos.y), ImGuiCond_Always);
    SetWindowSize({ 340, 220 });
    Begin(
        "Vehicle ESP Preview",
        nullptr,
        ImGuiWindowFlags_NoDecoration
    );
    Text("Vehicle ESP Preview");
    Separator();
    Image(reinterpret_cast<ImTextureID>(Veh_Texture), ImVec2(320, 180));

    auto pos = GetWindowPos();
    auto size = GetWindowSize();
    ImDrawList* draw = GetWindowDrawList();

    if (Settings::v_nametags || Settings::v_distance || Settings::v_driver_status || Settings::v_lockstatus)
    {
        std::vector<std::string> lines;

        if (Settings::v_nametags)
            lines.push_back("BALLER3");

        if (Settings::v_driver_status)
            lines.push_back("Has Driver");

        if (Settings::v_lockstatus)
            lines.push_back("Locked");

        if (Settings::v_distance)
            lines.push_back("[412m]");

        float white[4] = { 1, 1, 1, 1 };
        float black[4] = { 0, 0, 0, 1 };

        float text_y = pos.y + 35.f;

        for (const auto& line : lines) {
            ImVec2 text_size = CalcTextSize(line.c_str());

            float text_x = pos.x + 40.f - (text_size.x / 2.0f);

            draw->AddText(
                espfont,
                calc_text_size(412),
                ImVec2(text_x + 10.f, text_y + 1.f),
                f_2_imcol(black),
                line.c_str()
            );

            draw->AddText(
                espfont,
                calc_text_size(412),
                ImVec2(text_x + 10.f, text_y),
                f_2_imcol(white),
                line.c_str()
            );

            text_y += text_size.y - 5;
        }
    }

    End();
}

void SettingsButton(const char* PopupName, const ImVec2& PopupSize, std::function<void()> ContentProvider) {

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);

    ImGui::PushFont(fa);

    ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 3);

    if (ImGui::ImageButton(PopupName, reinterpret_cast<ImTextureID>(Cog_Texture), { 14, 14 })) {
        ImGui::OpenPopup(PopupName);
    }
    ImGui::PopFont();

    ImGui::SetNextWindowSize(PopupSize);

    if (ImGui::BeginPopup(PopupName)) {
        ContentProvider();
        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
}

// ======================================================================================================================
// ======================================================================================================================
// ======================================================================================================================

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
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void MultiSelectCombo(const char* label, std::vector<const char*>& items, std::vector<bool>& selection)
{
    std::string comboLabel;
    for (size_t i = 0; i < items.size(); ++i)
    {
        if (selection[i])
        {
            if (!comboLabel.empty())
                comboLabel += ", ";
            comboLabel += items[i];
        }
    }

    if (comboLabel.empty())
        comboLabel = "None";

    if (ImGui::BeginCombo(label, comboLabel.c_str()))
    {
        for (int i = 0; i < items.size(); i++) {
            if (ImGui::Selectable(items[i], selection[i])) {
                selection[i] = !selection[i];
            }
        }
        ImGui::EndCombo();
    }
}