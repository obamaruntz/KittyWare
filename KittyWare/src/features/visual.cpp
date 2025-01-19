#include <cache.h>
#include <features/visual.h>
#include <windows.h>

inline ImDrawList* draw = nullptr;
inline ImGuiIO io;

void Visual::RenderAllVisuals(ImDrawList* draw_list, ImGuiIO& io_ref) {
	draw = draw_list;
	io = io_ref;

	if (Settings::draw_fov)
		Visual::RenderFOV();
	if (Settings::xhair)
		Visual::RenderXhair();

	RenderESP();
}

void Visual::RenderFOV() {
	ImU32 col = ImColor(Settings::fov_color[0], Settings::fov_color[1], Settings::fov_color[2], Settings::fov_color[3]);
	draw->AddCircle(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), (float)Settings::fov, col, 100, 0.5f);
}

void Visual::RenderXhair() {
	ImU32 col = ImColor(Settings::xhair_color[0], Settings::xhair_color[1], Settings::xhair_color[2], Settings::xhair_color[3]);
	draw->AddCircleFilled(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), 2.5f, col, 100);
}

void Visual::RenderESP() {
	if (GetForegroundWindow() != Cache::hwnd && GetForegroundWindow() != Cache::menu_hwnd)
		return;


}