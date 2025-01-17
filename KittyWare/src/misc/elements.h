#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Elements {
    bool Tab(const char* name, bool boolean);
    inline float accent_color_element[4] = { 0.f, 132.f / 255.f, 1.f, 1.f };
}
