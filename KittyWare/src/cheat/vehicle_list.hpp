#pragma once
#include <sdk/other/vector.h>
#include <string>
#include <sdk/c_vehicle.hpp>
#include <vector>
#include <thread>
#include <chrono>
#include <memory/driver.h>
#include <mutex>
#include <cache.h>
#include <sdk/calculations.h>
#include <memory/offsets.h>
#include <cheat/settings.h>

namespace vehicle_list
{
	struct target
	{
		Vector2 screen_position;
		Vector3 origin;
		uint32_t weapon_name_hash;
		std::string weapon_name;
	};

	target target_ped;

	struct vehicle_struct {
		Vector2 screen_position;
		Vector3 origin;
		int index;
		std::string vehicle_name;
		float distance, health, max_health;
		bool is_locked, has_driver, is_own_vehicle;
        c_vehicle_occupants vehicle_occupants;
        uintptr_t ptr;
	};

	std::vector<vehicle_struct> vehicle_list;

	void vehicle_space()
	{
		while (true)
		{
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

			float closest_distance = FLT_MAX;

			std::lock_guard<std::mutex> lock_vehicle(Mutex::vehicle_list_mutex);

			vehicle_list.clear();

			Cache::vehicle_interface = Gate::Read<uintptr_t>(Cache::replay_interface + 0x10);
			Cache::vehicle_list = Gate::Read<uintptr_t>(Cache::vehicle_interface + 0x180);

			uintptr_t local_vehicle = Gate::Read<uintptr_t>(Cache::local_player + Offsets::vehicle_offset);

			for (int vehicle_index = 0; vehicle_index < 256; vehicle_index++)
			{
				uintptr_t vehicle = Gate::Read<uintptr_t>(Cache::vehicle_list + vehicle_index * 0x10);

				if (!vehicle)
					continue;

				Vector3 origin = sdk::c_vehicle::get_origin(vehicle);

				if (origin.is_zero())
					continue;

				Vector2 screen_position = calc::world_to_screen(origin);

				if (screen_position.x == -1 && screen_position.y == -1)
					continue;

				float distance = calc::distance_v3(Cache::local_position, origin);

				float pixel_distance = calc::distance({ float(Globals::screen.x / 2), float(Globals::screen.y / 2) }, screen_position);

				uintptr_t handling = Gate::Read<uintptr_t>(vehicle + 0x938);

                std::string vehicle_name = sdk::c_vehicle::c_model_info::get_vehicle_name(vehicle);

				vehicle_struct new_vehicle;

				new_vehicle.distance = distance;
				new_vehicle.origin = origin;
				new_vehicle.screen_position = screen_position;
				new_vehicle.vehicle_name = vehicle_name;
				new_vehicle.is_locked = sdk::c_vehicle::is_locked(vehicle);
				new_vehicle.has_driver = sdk::c_vehicle::has_driver(vehicle);
				new_vehicle.is_own_vehicle = local_vehicle == vehicle;
				new_vehicle.index = vehicle_index;
				new_vehicle.health = sdk::c_vehicle::get_health(vehicle);
				new_vehicle.max_health = sdk::c_vehicle::get_max_health(vehicle);
				new_vehicle.vehicle_occupants = sdk::c_vehicle::get_vehicle_occupants(vehicle);
				new_vehicle.ptr = vehicle;

				vehicle_list.push_back(new_vehicle);

			}
		}
	}
}
