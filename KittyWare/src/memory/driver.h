#pragma once
#include <windows.h>
#include <iostream>

namespace Gate {
    inline HANDLE driver;

    namespace codes {
        inline constexpr ULONG attach =
            CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
        inline constexpr ULONG read =
            CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
        inline constexpr ULONG write =
            CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
    }

    struct Request {
        HANDLE process_id;
        PVOID target;
        PVOID buffer;
        SIZE_T size;
        SIZE_T return_size;
    };

    inline bool Attach(const DWORD pid) {
        Request r;
        r.process_id = reinterpret_cast<HANDLE>(pid);

        return DeviceIoControl(driver, codes::attach, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
    }

    template <class T>
    inline T Read(const std::uintptr_t addr) {
        T temp = {};

        Request r;
        r.target = reinterpret_cast<PVOID>(addr);
        r.buffer = &temp;
        r.size = sizeof(T);

        DeviceIoControl(driver, codes::read, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);

        return temp;
    }

    template <class T>
    inline void Write(const std::uintptr_t addr, const T& value) {
        Request r;
        r.target = reinterpret_cast<PVOID>(addr);
        r.buffer = (PVOID)&value;
        r.size = sizeof(T);

        DeviceIoControl(driver, codes::write, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
    }
}