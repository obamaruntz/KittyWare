#pragma once
#include <imgui/imgui.h>
#include <cheat/settings.h>

namespace Visual {
	void RenderAllVisuals(ImDrawList* draw, ImGuiIO& io);
	void RenderFOV();
	void RenderXhair();
	void RenderESP();
}