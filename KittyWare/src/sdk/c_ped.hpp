#pragma once
#include <memory/driver.h>
#include <sdk/other/vector.h>
#include <memory/offsets.h>

namespace sdk
{
    namespace c_ped
    {
        Vector3 get_origin(uint64_t c_ped)
        {
            return Gate::Read<Vector3>(c_ped + 0x90);
        }

        bool in_vehicle(uint64_t c_ped)
        {
            uint8_t ped_task = Gate::Read<uint8_t>(c_ped + Offsets::task_offset);

            return ped_task & 64;
        }

        uint64_t get_vehicle(uint64_t c_ped)
        {
            return Gate::Read<uint64_t>(c_ped + Offsets::vehicle_offset);
        }

        uint64_t get_navigation(uint64_t c_ped)
        {
            return Gate::Read<uint64_t>(c_ped + 0x30);
        }

        float get_health(uint64_t c_ped)
        {
            return Gate::Read<float>(c_ped + 0x280);
        }

        float get_armor(uint64_t c_ped)
        {
            return Gate::Read<float>(c_ped + Offsets::armor_offset);
        }

        void set_origin(uint64_t c_ped, Vector3 v_position)
        {
            Gate::Write<Vector3>(c_ped + 0x90, v_position);
            uint64_t c_navigation = get_navigation(c_ped);
            Gate::Write<Vector3>(c_navigation + 0x50, v_position);
        }
    }
}
