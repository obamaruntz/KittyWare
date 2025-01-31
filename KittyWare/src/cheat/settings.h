#pragma once
#include <mutex>
#include <vector>
#include <sdk/other/vector.h>

namespace Globals {
	inline Vector2 screen = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	inline Vector2 screen_center = { GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2 };
}

namespace Mutex {
	inline std::mutex vehicle_list_mutex;
	inline std::mutex entity_list_mutex;
	inline std::mutex target_bone_mutex;
	inline std::mutex freecam_mutex;
	inline std::mutex tracer_mutex;
	inline std::mutex bullet_impacts_mutex;
}

namespace Settings {
	inline bool godmode = false;
	inline bool demigod = false;
	inline float run_speed = 1.f;
	inline float swim_speed = 1.f;
	inline bool superjump = false;
	inline bool healkey = false;
	inline bool noclip = false;
	inline float noclip_speed = 1.f;

	inline bool horn_boost = false;
	inline bool veh_godmode = false;
	inline float veh_accel = 1.f;
	inline float veh_break = 1.f;
	inline float veh_down = 1.f;

	inline float weap_dmg = 1.f;
	inline float melee_dmg = 1.f;
	inline bool no_recoil = false;
	inline bool no_spread = false;
	inline bool no_reload = false;
	inline bool fast_reload = false;
	inline bool no_range_limit = false;

	inline int smoothing_x = 0;
	inline int smoothing_y = 0;
	inline int fov = 50;
	inline int aim_max_distance = 500;
	inline int miss_chance = 0;
	inline bool ignore_npc_aim = 0;
	inline bool ignore_dead_aim = 0;
	inline bool ignore_friend_aim = 0;
	inline bool ignore_without_weapon_aim = 0;
	inline int bone = 0;
	inline std::vector<bool> closest_bones_filter(9, true);
	
	inline int esp_max_distance = 500;
	inline bool ignore_self = false;
	inline bool ignore_dead = false;
	inline bool ignore_npcs = false;
	inline bool player_names = false;
	inline bool bounding_box = false;
	inline int box_type = 0;
	inline bool filled_box = false;
	inline bool skeletons = false;
	inline bool healthbar = false;
	inline bool armourbar = false;
	inline bool emptybars = false;
	inline bool draw_player_name = false;
	inline bool draw_weapon_name = false;
	inline bool draw_distance = false;
	inline bool draw_net_id = false;
	inline float bounding_box_col[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	inline float filled_box_col[4] = { 1.0f, 1.0f, 1.0f, 0.3f };
	inline float skeleton_col[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	inline float healthbar_col[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	inline float armourbar_col[4] = { 0.0f, 0.5f, 1.0f, 1.0f };
	inline int veh_esp_max_distance = 500;
	inline bool v_driver_status = false;
	inline bool v_nametags = false;
	inline bool v_lockstatus = false;
	inline bool v_distance = false;
	inline bool friend_override = true;
	inline float friend_bounding_box[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	inline float friend_filled_box[4] = { 0.0f, 1.0f, 0.0f, 0.3f };
	inline float friend_skeleton[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	inline bool draw_fov = true;
	inline float fov_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	inline bool draw_ped_info = false;
	inline bool draw_spec_list = false;
	inline bool xhair = false;
	inline float xhair_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	inline bool draw_look_warn = false;
	inline bool hitmarkers = false;
	inline bool draw_dmg_nums = false;
	inline int dmg_nums_length = 2;
	inline bool tracers = false;
	inline int tracers_length = 1;

	inline int f_type = 0;
	inline std::vector<std::string> friend_list;
	inline std::vector<int> friend_list_id;
	inline int npc_identify_type = 0;

	inline int healkey_health_amount = 200;
	inline int healkey_armor_amount = 100;
}

namespace Hotkeys
{
	inline bool heal_key_bool = false;
	inline bool noclip_key_bool = false;
	inline bool explosive_melee_bool = false;
	inline bool explosive_ammo_bool = false;
	inline bool fire_ammo_bool = false;
	inline bool aim_key_bool = false;
	inline bool triggerbot_key_bool = false;
	inline bool force_body_key_bool = false;
	inline bool menu_key_bool = false;
	inline bool exit_key_bool = false;
	inline bool magic_bullet_key_bool = false;
	inline bool chams_key_bool = false;
}

namespace Keys
{
	inline int aim_key = 0x2;
	inline int triggerbot_key = 0x2;
	inline int magic_bullet_key = 0x0;
	inline int chams_key = 0x0;
	inline int force_body_key = 0x0;
	inline int heal_key = 0x0;
	inline int zoom_key = 0x0;
	inline int noclip_key = 0x0;
	inline int explosive_melee_key = 0x0;
	inline int explosive_ammo_key = 0x0;
	inline int fire_ammo_key = 0x0;
}