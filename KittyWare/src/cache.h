#pragma once
#include <Windows.h>


namespace Cache {
	inline uintptr_t base = 0;
	inline uintptr_t player_name_base = 0;
	inline HWND hwnd = 0;
	inline HWND menu_hwnd = 0;
	inline int game_build = 0;
	//inline uintptr_t world = 0;
	//inline uintptr_t player_info = 0;
	//inline uintptr_t local_player = 0;
	//inline uintptr_t view_angles = 0;
	//inline Vector3 local_position;
	//inline Vector2 local_screen_position;
	//inline uintptr_t replay_interface = 0;
	//inline uintptr_t ped_interface = 0;
	//inline uintptr_t ped_list = 0;
	//inline uintptr_t vehicle_interface = 0;
	//inline uintptr_t vehicle_list = 0;
	//inline uintptr_t global_ptr = 0;
	//inline int max_players = 0;
	//inline int max_vehicles = 0
	//inline uint8_t framecount_last_visible = 0;
	//inline uintptr_t view_port;
	//inline D3DXMATRIX view_matrix = {};
	//inline ped local_ped;

	void main()
	{
		while (true)
		{
			Sleep(1);
			//world = memory::Read<uintptr_t>(base + offsets::world_pointer);
			//replay_interface = memory::Read<uintptr_t>(base + offsets::replay_interface_pointer);
			//view_port = memory::Read<uintptr_t>(base + offsets::view_port_pointer);
			/*local_player = memory::Read<uintptr_t>(world + 0x8);
			game::view_angles = memory::Read<uintptr_t>(game::base + offsets::view_angles_pointer);
			local_position = memory::Read<Vector3>(local_player + 0x90);
			view_matrix = memory::Read<D3DXMATRIX>(view_port + 0x24C);
			local_ped.address = local_player;*/
		}
	}
}
