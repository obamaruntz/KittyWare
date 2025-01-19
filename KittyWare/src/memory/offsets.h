#pragma once
#include <stdint.h>

namespace Offsets
{
    inline uint64_t world_pointer, view_port_pointer, replay_interface_pointer, view_angles_pointer, weapon_manager, net_id_offset,
        bone_offset, freeze_offset, playerinfo_offset, armor_offset, vehicle_offset, vehicle_lock_offset, c_weapon_offset, handling_offset,
        run_speed_offset, frame_flags_offset, recoil_offset, spread_offset, range_offset, task_offset, waypoint_offset, globals_offset, ammo_offset,
        time_nop_x, time_nop_y, freecam_nop, time_offset, reload_multiplier_offset, body_health, tank_health, driver_offset;

    inline void Set(int build)
    {
        switch (build)
        {
        case 1604:
            world_pointer = 0x247F840;
            view_port_pointer = 0x2087780;
            replay_interface_pointer = 0x1EFD4C8;
            view_angles_pointer = 0x2088360;
            weapon_manager = 0x10D8;
            bone_offset = 0x430;
            armor_offset = 0x14B8;
            vehicle_offset = 0xD28;
            vehicle_lock_offset = 0x1350;
            waypoint_offset = 0x2093320;
            ammo_offset = 0x1C2E81C;
            body_health = 0x820;
            tank_health = 0x824;
            driver_offset = 0xC28;
            break;
        case 2060:
            world_pointer = 0x24C8858;
            view_port_pointer = 0x1F6A7E0;
            replay_interface_pointer = 0x1EC3828;
            view_angles_pointer = 0x1F6B3C0;
            weapon_manager = 0x10D8;
            bone_offset = 0x430;
            armor_offset = 0x14E0;
            vehicle_offset = 0xD28;
            vehicle_lock_offset = 0x1390;
            waypoint_offset = 0x1F76370;
            ammo_offset = 0x1C72E04;
            body_health = 0x840;
            tank_health = 0x844;
            driver_offset = 0xC68;
            break;
        case 2189:
            world_pointer = 0x24E6D90;
            view_port_pointer = 0x1F888C0;
            replay_interface_pointer = 0x1EE18A8;
            view_angles_pointer = 0x1F89498;
            weapon_manager = 0x10D8;
            bone_offset = 0x430;
            armor_offset = 0x14E0;
            vehicle_offset = 0xD30;
            vehicle_lock_offset = 0x1390;
            waypoint_offset = 0x1F94460;
            ammo_offset = 0x1C8F364;
            body_health = 0x840;
            tank_health = 0x844;
            driver_offset = 0xC68;
            break;
        case 2372:
            world_pointer = 0x252DCD8;
            view_port_pointer = 0x1F9E9F0;
            replay_interface_pointer = 0x1F05208;
            weapon_manager = 0x10D8;
            view_angles_pointer = 0x1F9F5C8;
            bone_offset = 0x430;
            armor_offset = 0x14E0;
            vehicle_offset = 0xD30;
            vehicle_lock_offset = 0x1390;
            waypoint_offset = 0x1FBF2B0;
            body_health = 0x840;
            tank_health = 0x844;
            ammo_offset = 0x1CD8F04;
            driver_offset = 0xC68;
            break;
        case 2545:
            world_pointer = 0x25667E8;
            view_port_pointer = 0x1FD6F70;
            replay_interface_pointer = 0x1F2E7A8;
            view_angles_pointer = 0x1FD7B48;
            weapon_manager = 0x10D8;
            bone_offset = 0x430;
            armor_offset = 0x1530;
            vehicle_offset = 0xD30;
            vehicle_lock_offset = 0x1390;
            waypoint_offset = 0x1FF7A10;
            ammo_offset = 0x1D00E04;
            body_health = 0x840;
            tank_health = 0x844;
            driver_offset = 0xC68;
            break;
        case 2612:
            world_pointer = 0x2567DB0;
            view_port_pointer = 0x1FD8570;
            replay_interface_pointer = 0x1F77EF0;
            view_angles_pointer = 0x1FD9148;
            weapon_manager = 0x10D8;
            bone_offset = 0x430;
            armor_offset = 0x1530;
            vehicle_offset = 0xD30;
            vehicle_lock_offset = 0x1390;
            waypoint_offset = 0x1FF9010;
            ammo_offset = 0x1D01E34;
            body_health = 0x840;
            tank_health = 0x844;
            driver_offset = 0xCF0;
            break;
        case 2699:
            world_pointer = 0x26684D8;
            view_port_pointer = 0x20D8C90;
            replay_interface_pointer = 0x20304C8;
            view_angles_pointer = 0x20D9868;
            weapon_manager = 0x10D8;
            bone_offset = 0x430;
            armor_offset = 0x1530;
            vehicle_offset = 0xD30;
            vehicle_lock_offset = 0x1390;
            waypoint_offset = 0x20F9750;
            ammo_offset = 0x1E01EC4;
            body_health = 0x840;
            tank_health = 0x844;
            driver_offset = 0xC68;
            break;
        case 2802:
            world_pointer = 0x254D448;
            view_port_pointer = 0x1FBC100;
            replay_interface_pointer = 0x1D76838;
            view_angles_pointer = 0x1FBCCD8;
            weapon_manager = 0x10B8;
            bone_offset = 0x410;
            armor_offset = 0x150C;
            vehicle_offset = 0xD10;
            vehicle_lock_offset = 0x1370;
            waypoint_offset = 0x1FDD510;
            ammo_offset = 0x1D2EAE4;
            body_health = 0x820;
            tank_health = 0x824;
            driver_offset = 0xC48;
            break;
        case 2944:
            world_pointer = 0x257BEA0;
            view_port_pointer = 0x1FEAAC0;
            replay_interface_pointer = 0x1F42068;
            view_angles_pointer = 0x1FEB698;
            weapon_manager = 0x10B8;
            bone_offset = 0x410;
            armor_offset = 0x150C;
            vehicle_offset = 0xD10;
            waypoint_offset = 0x200BEB0;
            vehicle_lock_offset = 0x1370;
            ammo_offset = 0x1D59BD4;
            body_health = 0x820;
            tank_health = 0x824;
            driver_offset = 0xC48;
            break;
        case 3095:
            world_pointer = 0x2593320;
            view_port_pointer = 0x20019E0;
            replay_interface_pointer = 0x1F58B58;
            view_angles_pointer = 0x20025B8;
            weapon_manager = 0x10B8;
            bone_offset = 0x410;
            armor_offset = 0x150C;
            vehicle_offset = 0xD10;
            vehicle_lock_offset = 0x1370;
            globals_offset = 0x161E258;
            waypoint_offset = 0x2022DE0;
            ammo_offset = 0x1D701B4;
            body_health = 0x820;
            tank_health = 0x824;
            driver_offset = 0xC98;
            break;
        case 3274:
            world_pointer = 0x25B24D8;
            replay_interface_pointer = 0x1F76428;
            view_angles_pointer = 0x201F848;
            view_port_pointer = 0x201EC20;
            weapon_manager = 0x10B8;
            bone_offset = 0x410;
            armor_offset = 0x150C;
            vehicle_offset = 0xD10;
            vehicle_lock_offset = 0x1370;
            globals_offset = 0x161E258;
            waypoint_offset = 0x2022DE0;
            ammo_offset = 0x1D701B4;
            body_health = 0x820;
            tank_health = 0x824;
            driver_offset = 0xC98;
        }

        if (build <= 2699)
            net_id_offset = 0x88;
        else if (build > 2699)
            net_id_offset = 0xE8;


        if (build <= 2372)
            freeze_offset = 0x218;
        else if (build > 2372)
            freeze_offset = 0x238;

        if (build <= 2699)
            playerinfo_offset = 0x10C8;
        else if (build > 2699)
            playerinfo_offset = 0x10A8;

        if (build <= 2699)
            c_weapon_offset = 0x340;
        else if (build > 2699)
            c_weapon_offset = 0x320;

        if (build == 1604)
            handling_offset = 0x918;
        if (build == 2060)
            handling_offset = 0x908;
        if (build >= 2189)
            handling_offset = 0x938;
        if (build >= 2802)
            handling_offset = 0x918;
        if (build == 3095)
            handling_offset = 0x960;

        if (build == 2944)
            run_speed_offset = 0xD40;
        else
            run_speed_offset = 0xCF0;

        if (build == 1604)
            frame_flags_offset = 0x1F8;
        if (build > 1604)
            frame_flags_offset = 0x218;
        if (build >= 2802)
            frame_flags_offset = 0x270;

        if (build == 1699)
            recoil_offset = 0x2F8;
        if (build > 1699)
            recoil_offset = 0x2F4;

        if (build == 1699)
            spread_offset = 0x74;
        if (build > 1699)
            spread_offset = 0x84;

        if (build == 1699)
            range_offset = 0x25C;
        if (build > 1699)
            range_offset = 0x28C;

        if (build >= 2802)
            task_offset = 0x144B;
        else if (build < 2802)
            task_offset = 0x146B;

        if (build == 1699)
            reload_multiplier_offset = 0x12C;
        else
            reload_multiplier_offset = 0x134;
    }
}