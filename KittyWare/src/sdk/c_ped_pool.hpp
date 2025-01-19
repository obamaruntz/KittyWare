#pragma once
#include <memory/driver.h>

namespace sdk
{
    class c_ped_pool {
    private:
        uint32_t size;
        uint64_t bit_array;
        uint32_t item_size;
        uint64_t ped_pool_base;
    public:
        c_ped_pool(uint64_t ped_pool) {
            size = Gate::Read<uint32_t>(ped_pool + 0x10);
            bit_array = Gate::Read<uint64_t>(ped_pool + 0x8);
            item_size = Gate::Read<uint32_t>(ped_pool + 0x14);
            ped_pool_base = ped_pool;
        }


        bool is_valid() {
            return ped_pool_base != 0;
        }

        uint32_t get_size() {
            return size;
        }

        uint64_t get_ped(int index) {
            if (index < 0 || index >= size) {
                return 0;
            }

            BYTE flag = Gate::Read<BYTE>(bit_array + index);
            __int64 mask = ~((__int64)(flag & 0x80 | (unsigned __int64)-(__int64)(flag & 0x80)) >> 63);
            uint64_t CPed = mask & (Gate::Read<uint64_t>(ped_pool_base) + item_size * index);
            return CPed;
        }
    };
}
