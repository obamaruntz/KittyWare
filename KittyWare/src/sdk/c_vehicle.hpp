#pragma once
#include <memory/driver.h>
#include <sdk/other/vector.h>
#include <memory/Offsets.h>
#include <sdk/c_ped.hpp>

enum eCarLockState
{
    CARLOCK_NONE = 0,
    CARLOCK_UNLOCKED = 1,
    CARLOCK_LOCKED = 2,
    CARLOCK_LOCKOUT_PLAYER_ONLY = 3,
    CARLOCK_LOCKED_PLAYER_INSIDE = 4,
    CARLOCK_LOCKED_INITIALLY = 5,
    CARLOCK_FORCE_SHUT_DOORS = 6,
    CARLOCK_LOCKED_BUT_CAN_BE_DAMAGED = 7,
    CARLOCK_LOCKED_BUT_BOOT_UNLOCKED = 8,
    CARLOCK_LOCKED_NO_PASSENGERS = 9,
};

struct c_vehicle_occupants {
    uint64_t driver, passengers[15];
};

namespace sdk
{
    namespace c_vehicle {

        Vector3 get_origin(uint64_t c_vehicle)
        {
            return Gate::Read<Vector3>(c_vehicle + 0x90);
        }

        float get_health(uint64_t c_vehicle)
        {
            return Gate::Read<float>(c_vehicle + 0x280);
        }

        float get_max_health(uint64_t c_vehicle)
        {
            return Gate::Read<float>(c_vehicle + 0x2A0);
        }


        uint64_t get_navigation(uint64_t c_vehicle)
        {
            return Gate::Read<uint64_t>(c_vehicle + 0x30);
        }

        void set_position(uint64_t c_vehicle, Vector3 v_position)
        {
            Gate::Write<Vector3>(c_vehicle + 0x90, v_position);
            uint64_t c_navigation = get_navigation(c_vehicle);
            Gate::Write<Vector3>(c_navigation + 0x50, v_position);
        }

        float get_tank_health(uint64_t c_vehicle)
        {
            return Gate::Read<float>(c_vehicle + Offsets::tank_health);
        }

        bool is_locked(uint64_t c_vehicle)
        {
            uint32_t m_door_lock_status = Gate::Read<uint32_t>(c_vehicle + Offsets::vehicle_lock_offset);

            return m_door_lock_status != CARLOCK_UNLOCKED;
        }

        bool has_driver(uint64_t c_vehicle)
        {
            uint64_t driver = Gate::Read<uintptr_t>(c_vehicle + 0xC98);

            if (sdk::c_ped::in_vehicle(driver) && driver)
                return true;

            return false;
        }

        void set_godmode(uint64_t c_vehicle, bool godmode)
        {
            Gate::Write<bool>(c_vehicle + 0x189, godmode);
        }

        void set_tank_health(uint64_t c_vehicle, float value)
        {
            Gate::Write<float>(c_vehicle + Offsets::tank_health, value);
        }

        void set_body_health(uint64_t c_vehicle, float value)
        {
            Gate::Write<float>(c_vehicle + Offsets::body_health, value);
        }

        bool get_godmode(uint64_t c_vehicle)
        {
            return Gate::Read<bool>(c_vehicle + 0x189);
        }

        c_vehicle_occupants get_vehicle_occupants(uint64_t c_vehicle)
        {
            return Gate::Read<c_vehicle_occupants>(c_vehicle + Offsets::driver_offset);
        }

        namespace c_handling {
            uint64_t get_handling(uint64_t c_vehicle)
            {
                return Gate::Read<uint64_t>(c_vehicle + Offsets::handling_offset);
            }

            void set_acceleration(uint64_t c_vehicle, float acceleration)
            {
                uint64_t handling = get_handling(c_vehicle);

                Gate::Write<float>(handling + 0x4C, acceleration);
            }

            void set_brake_force(uint64_t c_vehicle, float brake_force)
            {
                uint64_t handling = get_handling(c_vehicle);

                Gate::Write<float>(handling + 0x6C, brake_force);
            }

            void set_down_force(uint64_t c_vehicle, float down_force)
            {
                uint64_t handling = get_handling(c_vehicle);

                Gate::Write<float>(handling + 0x14, down_force);
            }
        }

        namespace c_model_info {
            uint64_t get_model_info(uint64_t c_vehicle)
            {
                return Gate::Read<uint64_t>(c_vehicle + 0x20);
            }

            std::string get_vehicle_name(uint64_t c_vehicle) {
                uint64_t model_info = get_model_info(c_vehicle);

                struct vehicle_name {
                    char name[12];
                };

                vehicle_name name = Gate::Read<vehicle_name>(model_info + 0x298);

                return std::string(name.name);
            }
        }

        namespace c_vehicle_draw_handler {
            uint64_t get_vehicle_draw_handler(uint64_t c_vehicle)
            {
                return Gate::Read<uint64_t>(c_vehicle + 0x48);
            }

            namespace c_custom_shader_effect_vehicle {
                uint64_t get_custom_shader_effect_vehicle(uint64_t c_vehicle)
                {
                    return Gate::Read<uint64_t>(get_vehicle_draw_handler(c_vehicle) + 0x20);
                }

                void set_primary_vehicle_color(uint64_t c_vehicle, int r, int g, int b) {

                    if (!c_vehicle)
                        return;

                    uint64_t custom_shader_effect_vehicle = get_custom_shader_effect_vehicle(c_vehicle);

                    Gate::Write<int>(custom_shader_effect_vehicle + 0xA4, 0xFF000000 | (r << 16) | (g << 8) | b);
                }

                void set_secondary_vehicle_color(uint64_t c_vehicle, int r, int g, int b) {

                    if (!c_vehicle)
                        return;

                    uint64_t custom_shader_effect_vehicle = get_custom_shader_effect_vehicle(c_vehicle);

                    Gate::Write<int>(custom_shader_effect_vehicle + 0xA8, 0xFF000000 | (r << 16) | (g << 8) | b);
                }
            }
        }
    }
}
