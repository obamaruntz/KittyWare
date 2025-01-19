#pragma once
#include <memory/driver.h>
#include <sdk/other/vector.h>
#include <memory/offsets.h>

class ped
{
public:
    uintptr_t address;

    float get_health()
    {
        return Gate::Read<float>(address + 0x280);
    }

    float get_max_health()
    {
        return Gate::Read<float>(address + 0x284);
    }

    float get_armor()
    {
        return Gate::Read<float>(address + Offsets::armor_offset);
    }

    bool in_vehicle()
    {
        uint8_t ped_task = Gate::Read<uint8_t>(address + Offsets::task_offset);

        return ped_task & 64;
    }

    Vector3 get_origin()
    {
        return Gate::Read<Vector3>(address + 0x90);
    }

};

inline ped create_ped(uintptr_t pointer)
{
    ped new_ped;
    new_ped.address = pointer;
    return new_ped;
}