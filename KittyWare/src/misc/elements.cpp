#include <misc/elements.h>

struct tab_element {
    float element_opacity;
};

bool Elements::Tab(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = ImGui::CalcTextSize(name, NULL, true);
    ImVec2 pos = window->DC.CursorPos;

    const float padding_x = style.FramePadding.x;
    const float padding_y = style.FramePadding.y;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x + padding_x * 2, pos.y + label_size.y + padding_y * 2));
    ImGui::ItemSize(rect, padding_y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.8f : hovered ? 0.6f : 0.4f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    ImColor rect_color = ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity);
    if (boolean)
        rect_color = ImColor((float)accent_color_element[0], (float)accent_color_element[1], (float)accent_color_element[2], it_anim->second.element_opacity);

    if ((boolean || hovered) && name != "Exit")
    {
        float fill_opacity = boolean ? 0.8f : 0.4f;
        ImColor fill_color = ImColor(33.f / 255.f, 33.f / 255.f, 33.f / 255.f, fill_opacity);
        window->DrawList->AddRectFilled(rect.Min, rect.Max, fill_color, 4.f);
    }

    window->DrawList->AddText(pos + ImVec2(padding_x, padding_y), rect_color, name);

    return pressed;
}

