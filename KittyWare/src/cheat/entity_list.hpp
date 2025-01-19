#pragma once
#include <unordered_map>
#include <d3dx9.h>
#include <memory/memory.h>
#include <memory/Offsets.h>
#include <cache.h>
#include <chrono>
#include <thread>
#include <sdk/calculations.h>
#include <mutex>
#include <cheat/settings.h>
#include <cheat/weapons.h>

enum class ePedBoneType
{
    HEAD,
    L_FOOT,
    R_FOOT,
    L_ANKLE,
    R_ANKLE,
    L_HAND,
    R_HAND,
    NECK,
    ABDOMEN,
    L_FAKE_SHOULDER,
    R_FAKE_SHOULDER,
    L_FAKE_HIP,
    R_FAKE_HIP,
    CLOSEST_BONE,
};

namespace entity_list
{

    class CPedBoneInfo
    {
    public:
        D3DXVECTOR3 model_coords;
        char pad_000C[4];
    };

    class CPedBones
    {
    public:
        CPedBoneInfo HEAD, L_FOOT, R_FOOT, L_ANKLE, R_ANKLE, L_HAND, R_HAND, NECK, ABDOMEN;

        CPedBoneInfo& operator[](int index) {
            switch (index) {
            case 0: return HEAD;
            case 1: return L_FOOT;
            case 2: return R_FOOT;
            case 3: return L_ANKLE;
            case 4: return R_ANKLE;
            case 5: return L_HAND;
            case 6: return R_HAND;
            case 7: return NECK;
            case 8: return ABDOMEN;
            default: return HEAD;
            }
        }
    };


    std::unordered_map<int, Vector3> boneCache;

    CPedBones get_all_bones(uintptr_t ped_ptr) {
        return Gate::Read<CPedBones>(ped_ptr + Offsets::bone_offset);
    }

    void transform_bone(const D3DXMATRIX& matrix, const D3DXVECTOR3& bone_untransformed, Vector3& bone_transformed) {
        D3DXVECTOR4 transform_bone;
        D3DXVec3Transform(&transform_bone, &bone_untransformed, &matrix);
        bone_transformed = { transform_bone.x, transform_bone.y, transform_bone.z };
    }

    void transform_special_bone(const D3DXMATRIX& matrix, D3DXVECTOR3 bone_untransformed, float x_offset, float z_offset, Vector3& bone_transformed) {
        bone_untransformed.x += x_offset;
        bone_untransformed.z += z_offset;
        transform_bone(matrix, bone_untransformed, bone_transformed);
    }

    bool all_false(const std::vector<bool>& vec) {
        for (bool value : vec) {
            if (value) {
                return false;
            }
        }
        return true;
    }

    void calculate_bone_positions(D3DXMATRIX matrix, uintptr_t ped_base, std::vector<std::pair<Vector2, Vector2>>& bone_positions_screen, Vector3& aim_bone, Vector3& head, Vector3& foot, Vector3& neck) {
        std::unordered_map<int, Vector2> bones_screen;


        CPedBones bone_positions = get_all_bones(ped_base);

        int bones[][2] = {
            { int(ePedBoneType::HEAD), int(ePedBoneType::NECK) },
            { int(ePedBoneType::NECK), int(ePedBoneType::ABDOMEN) },
            { int(ePedBoneType::L_ANKLE), int(ePedBoneType::L_FAKE_HIP) },
            { int(ePedBoneType::R_ANKLE), int(ePedBoneType::R_FAKE_HIP) },
            { int(ePedBoneType::L_FAKE_HIP), int(ePedBoneType::ABDOMEN) },
            { int(ePedBoneType::R_FAKE_HIP), int(ePedBoneType::ABDOMEN) },
            { int(ePedBoneType::R_ANKLE), int(ePedBoneType::R_FOOT) },
            { int(ePedBoneType::L_ANKLE), int(ePedBoneType::L_FOOT) },
            { int(ePedBoneType::L_HAND), int(ePedBoneType::L_FAKE_SHOULDER) },
            { int(ePedBoneType::R_HAND), int(ePedBoneType::R_FAKE_SHOULDER) },
            { int(ePedBoneType::L_FAKE_SHOULDER), int(ePedBoneType::NECK) },
            { int(ePedBoneType::R_FAKE_SHOULDER), int(ePedBoneType::NECK) }
        };

        for (int i = 0; i < 12; ++i) {
            Vector3 bone[2];

            for (int j = 0; j < 2; ++j)
            {
                if (bones[i][j] <= 8) {
                    transform_bone(matrix, bone_positions[bones[i][j]].model_coords, bone[j]);
                }
                else {
                    D3DXVECTOR3 neck_untransformed = bone_positions[7].model_coords;
                    D3DXVECTOR3 abdomen_untransformed = bone_positions[8].model_coords;

                    switch (bones[i][j]) {
                    case 9:
                        transform_special_bone(matrix, neck_untransformed, -0.2f, -0.05f, bone[j]);
                        break;
                    case 10:
                        transform_special_bone(matrix, neck_untransformed, 0.18f, -0.05f, bone[j]);
                        break;
                    case 11:
                        transform_special_bone(matrix, abdomen_untransformed, -0.15f, -0.1f, bone[j]);
                        break;
                    case 12:
                        transform_special_bone(matrix, abdomen_untransformed, 0.15f, -0.1f, bone[j]);
                        break;
                    }
                }
            }

            Vector2 screen_1 = calc::world_to_screen(bone[0]);
            Vector2 screen_2 = calc::world_to_screen(bone[1]);

            if (Settings::bone == (int)ePedBoneType::CLOSEST_BONE)
            {
                if (bones_screen.find(bones[i][0]) == bones_screen.end() && bones[i][1] <= 8) {
                    if (Settings::closest_bones_filter[bones[i][0]] || all_false(Settings::closest_bones_filter))
                        bones_screen[bones[i][0]] = screen_1;
                }

                if (bones_screen.find(bones[i][1]) == bones_screen.end() && bones[i][1] <= 8) {
                    if (Settings::closest_bones_filter[bones[i][1]] || all_false(Settings::closest_bones_filter))
                        bones_screen[bones[i][1]] = screen_2;
                }
            }


            bone_positions_screen.push_back(std::make_pair(screen_1, screen_2));
        }

        transform_bone(matrix, bone_positions[int(ePedBoneType::HEAD)].model_coords, head);
        if (Settings::bone != (int)ePedBoneType::CLOSEST_BONE)
            transform_bone(matrix, bone_positions[Settings::bone].model_coords, aim_bone);



        if (Settings::bone == (int)ePedBoneType::CLOSEST_BONE)
        {
            std::unordered_map<int, float> bones_distance;

            for (auto& bone : bones_screen)
            {
                if (Settings::closest_bones_filter[bone.first])
                {
                    float distance = calc::distance(Globals::screen_center, bone.second);

                    bones_distance[bone.first] = distance;
                }

            }

            auto first_valid = std::find_if(bones_distance.begin(), bones_distance.end(),
                [](const std::pair<int, float>& p) { return p.first <= 8; });

            auto min_element = std::min_element(first_valid, bones_distance.end(),
                [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                    if (b.first > 8) return true;  // Consider `a` as smaller if `b`'s int part is greater than 8
                    return a.second < b.second;
                });

            if (min_element->first <= 8)
                transform_bone(matrix, bone_positions[min_element->first].model_coords, aim_bone);

        }

        transform_bone(matrix, bone_positions[int(ePedBoneType::L_FOOT)].model_coords, foot);
        transform_bone(matrix, bone_positions[int(ePedBoneType::NECK)].model_coords, neck);
    }

    struct style {
        BYTE hat_id, hat_texture;
    };

    struct target
    {
        Vector2 target_bone_screen, origin_screen;
        Vector3 target_bone, origin;
        int health, armor, pixel_distance, net_id, distance;
        bool godmode, invisible, is_friend, in_own_vehicle;
        std::string weapon_name, player_name;
        uint64_t m_pointer;
    };

    struct Bounds {
        int top, bottom, left, right;
    };


    target target_ped;

    struct ped_struct {
        Vector2 screen_head, screen_position, screen_foot;
        Vector3 head, origin, look_line;
        float height, width, width_real, angle;
        int distance, health, armor, net_id, index;
        std::vector<std::pair<Vector2, Vector2>> bones;
        uint32_t weapon_name_hash;
        std::string weapon_name, player_name, license;
        bool is_npc, is_visible, is_friend, is_self, is_invisible, is_looking_at_you;
        uintptr_t m_pointer;
        Bounds bounds;
        Vector3 v_bones[8];
    };

    std::vector<ped_struct> ped_list;

    int get_index_by_net_id(int target_net_id) {
        for (size_t i = 0; i < ped_list.size(); ++i) {
            if (ped_list[i].net_id == target_net_id) {
                return ped_list[i].index;
            }
        }
        return -1;
    }

    int get_list_index_by_net_id(int target_net_id) {
        for (size_t i = 0; i < ped_list.size(); ++i) {
            if (ped_list[i].net_id == target_net_id) {
                return i;
            }
        }
        return -1;
    }

    int get_list_index_by_pointer(uint64_t m_pointer) {
        for (size_t i = 0; i < ped_list.size(); ++i) {
            if (ped_list[i].m_pointer == m_pointer) {
                return i;
            }
        }
        return -1;
    }

    const float PI = 3.14159265358979323846f;


    float RAD2DEG(float radians) {
        return radians * (180.0f / PI);
    }

    Bounds get_bounds(std::vector<std::pair<Vector2, Vector2>> bones)
    {
        Bounds bounds = { bones[0].first.y, bones[0].first.y, bones[0].first.x, bones[0].first.x };

        for (const auto& bone : bones)
        {
            const Vector2& p1 = bone.first;
            const Vector2& p2 = bone.second;

            if (p1.x < bounds.left)
                bounds.left = p1.x;
            else if (p1.x > bounds.right)
                bounds.right = p1.x;
            if (p1.y < bounds.top)
                bounds.top = p1.y;
            else if (p1.y > bounds.bottom)
                bounds.bottom = p1.y;

            if (p2.x < bounds.left)
                bounds.left = p2.x;
            else if (p2.x > bounds.right)
                bounds.right = p2.x;
            if (p2.y < bounds.top)
                bounds.top = p2.y;
            else if (p2.y > bounds.bottom)
                bounds.bottom = p2.y;
        }

        int expand = 4;

        bounds.top += expand;
        bounds.bottom += expand;
        bounds.left += expand;
        bounds.right += expand;

        return bounds;
    }

    void ped_space()
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            float closest_distance = FLT_MAX;

            Cache::local_screen_position = calc::world_to_screen(Cache::local_position);

            std::lock_guard<std::mutex> lock(Mutex::entity_list_mutex);
            std::lock_guard<std::mutex> lock_target_bone(Mutex::target_bone_mutex);

            target_ped.target_bone = { -1 , -1 };
            target_ped.origin_screen = { -1 , -1 };
            target_ped.m_pointer = 0;

            ped_list.clear();

            boneCache.clear();

            Vector3 current_viewangles = Gate::Read<Vector3>(Cache::view_angles + 0x3D0);

            if (current_viewangles.is_zero())
            {
                current_viewangles = Gate::Read<Vector3>(Cache::view_angles + 0x40);
            }


            Vector3 cam_pos = Gate::Read<Vector3>(Cache::view_angles + 0x60);

            Cache::ped_interface = Gate::Read<uintptr_t>(Cache::replay_interface + 0x18);
            Cache::ped_list = Gate::Read<uintptr_t>(Cache::ped_interface + 0x100);

            for (int player_index = 0; player_index < 256; player_index++)
            {
                uintptr_t ped = Gate::Read<uintptr_t>(Cache::ped_list + (player_index * 0x10));

                if (!ped)
                    continue;

                if (ped == Cache::local_player && (Settings::ignore_self))
                    continue;

                Vector3 origin = Gate::Read<Vector3>(ped + 0x90);

                if (origin.is_zero())
                    continue;

                int distance = calc::distance_v3(Cache::local_position, origin);

                Vector2 screen_position = calc::world_to_screen(origin);

                float health = Gate::Read<float>(ped + 0x280);

                float armor = Gate::Read<float>(ped + Offsets::armor_offset);

                CPedBones cPedBones = get_all_bones(ped);

                Vector3 v_bones[8];


                D3DXMATRIX matrix = Gate::Read<D3DXMATRIX>(ped + 0x60);

                for (int bone_index = 0; bone_index < 9; bone_index++)
                {
                    transform_bone(matrix, cPedBones[bone_index].model_coords, v_bones[bone_index]);
                }

                std::vector<std::pair<Vector2, Vector2>> bones;

                Vector3 aim_bone, head, foot, neck;

                calculate_bone_positions(matrix, ped, bones, aim_bone, head, foot, neck);

                Bounds bounds = get_bounds(bones);

                Vector2 screen_bone = calc::world_to_screen(aim_bone);

                float pixel_distance = calc::distance({ float(Globals::screen.x / 2), float(Globals::screen.y / 2) }, screen_bone);

                Vector2 screen_head = calc::world_to_screen(head);
                Vector2 screen_neck = calc::world_to_screen(neck);
                Vector2 screen_foot = calc::world_to_screen(foot);


                uintptr_t player_info = Gate::Read<uintptr_t>(ped + Offsets::playerinfo_offset);

                bool is_npc = false;

                if (Settings::npc_identify_type == 0)
                {
                    int ped_type = Gate::Read<int>(ped + 0x10b8) << 11 >> 25;

                    if (ped_type != 2)
                        is_npc = true;
                }
                else {
                    if (!player_info)
                        is_npc = true;
                }

                Vector3 look_line;
                bool is_looking_at_you;

                if (!is_npc)
                {
                    uintptr_t navigation = Gate::Read<uintptr_t>(ped + 0x30);
                    Rotation rotation = Gate::Read<Rotation>(navigation + 0x30);
                    calc::getLookLine(head, rotation, look_line, .5f);
                    is_looking_at_you = calc::is_looking_at_you(Cache::local_position, origin, rotation);
                }

                //if (globals::toggles::ignore_npc && globals::toggles::ignore_npc_aim && is_npc)
                //    continue;

                std::string weapon_name;
                uint32_t weapon_name_hash = 0;

                uintptr_t weapon_manager = Gate::Read<uintptr_t>(ped + Offsets::weapon_manager);
                uintptr_t weapon_info = Gate::Read<uintptr_t>(weapon_manager + 0x20);
                weapon_name_hash = Gate::Read<uint32_t>(weapon_info + 0x10);

                weapon_name = weapons::get_weapon_name(weapon_name_hash);

                int net_id = Gate::Read<int>(player_info + Offsets::net_id_offset);

                //std::string player_name = (player_names::player_name_map.find(net_id) != player_names::player_name_map.end()) ?
                //    player_names::player_name_map[net_id] :
                //    "";

                //std::string license = (player_names::player_license_map.find(net_id) != player_names::player_license_map.end()) ?
                //    player_names::player_license_map[net_id] :
                //    "";

                std::string player_name = "";
                std::string license = "";

                bool is_friend;

                if (Settings::f_type == 0)
                    is_friend = std::find(Settings::friend_list.begin(), Settings::friend_list.end(), license) != Settings::friend_list.end();
                else
                    is_friend = std::find(Settings::friend_list_id.begin(), Settings::friend_list_id.end(), net_id) != Settings::friend_list_id.end();

                bool is_self = ped == Cache::local_player;

                bool is_invisible = Gate::Read<BYTE>(ped + 0x2C) == 0x1;

                uintptr_t vehicle = Gate::Read<uintptr_t>(ped + Offsets::vehicle_offset);

                bool is_in_own_vehicle = false;

                if (vehicle)
                {
                    if (Cache::local_ped.in_vehicle())
                        is_in_own_vehicle = true;
                }

                if (pixel_distance < closest_distance && Cache::local_player != ped) {

                    if (!(Settings::ignore_npc_aim && is_npc) && !(Settings::ignore_dead_aim && health == 0) && !(Settings::ignore_friend_aim && is_friend) && !(Settings::ignore_without_weapon_aim && weapon_name == "None") && !is_in_own_vehicle)
                    {
                        closest_distance = pixel_distance;
                        target_ped.target_bone_screen = screen_bone;
                        target_ped.target_bone = aim_bone;
                        target_ped.origin_screen = screen_position;
                        target_ped.origin = origin;
                        target_ped.health = health;
                        target_ped.armor = armor;
                        target_ped.godmode = Gate::Read<bool>(ped + 0x189);
                        target_ped.invisible = is_invisible;
                        target_ped.pixel_distance = pixel_distance;
                        target_ped.weapon_name = weapon_name;
                        target_ped.player_name = player_name;
                        target_ped.net_id = net_id;
                        target_ped.distance = distance;
                        if (calc::distance(Globals::screen_center, screen_bone) < Settings::fov)
                            target_ped.m_pointer = ped;
                    }

                }

                int width = bounds.right - bounds.left;
                int height = bounds.bottom - bounds.top;

                auto angle = RAD2DEG(current_viewangles.y) - calc::calc_angles(cam_pos, origin).x - 90;

                ped_struct new_ped;

                new_ped.distance = distance;
                new_ped.bones = bones;
                new_ped.origin = origin;
                new_ped.head = head;
                new_ped.screen_head = screen_head;
                new_ped.screen_position = screen_position;
                new_ped.screen_foot = screen_foot;
                new_ped.armor = armor;
                new_ped.health = health;
                new_ped.height = height;
                new_ped.width = width;
                new_ped.weapon_name_hash = weapon_name_hash;
                new_ped.weapon_name = weapon_name;
                new_ped.is_npc = is_npc;
                new_ped.is_friend = is_friend;
                new_ped.index = player_index;
                new_ped.angle = angle;
                new_ped.net_id = net_id;
                new_ped.player_name = player_name;
                new_ped.license = license;
                new_ped.is_self = is_self;
                new_ped.is_invisible = is_invisible;
                new_ped.is_looking_at_you = is_looking_at_you;
                new_ped.look_line = look_line;
                new_ped.m_pointer = ped;
                new_ped.bounds = bounds;
                std::copy(std::begin(v_bones), std::end(v_bones), std::begin(new_ped.v_bones));

                ped_list.push_back(new_ped);
            }
        }
    }
}