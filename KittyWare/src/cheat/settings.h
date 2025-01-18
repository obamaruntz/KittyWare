#pragma once

namespace Settings {
	inline bool godmode = false;
	inline bool demigod = false;
	inline float run_speed = 1.f;
	inline float swim_speed = 1.f;
	inline bool superjump = false;
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
	inline int fov = 0;
	inline int aim_max_distance = 0;
	inline int miss_chance = 0;
	inline bool ignore_npc_aim = 0;
	inline bool ignore_dead_aim = 0;
	inline bool ignore_friend_aim = 0;
	inline bool ignore_without_weapon_aim = 0;
	inline int bone = 0;
}