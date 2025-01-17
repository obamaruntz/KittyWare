#pragma once
#include <iostream>
#include <string>
#include <windows.h>

namespace Console {
    inline void EnableANSI() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    enum class PrintType : uint32_t {
        OK = 0,
        LOADING = 1,
        FAIL = 2,
        DEBUG = 3,
    };

    struct Colors {
        static constexpr const char* GREEN = "\033[92m";
        static constexpr const char* YELLOW = "\033[93m";
        static constexpr const char* RED = "\033[91m";
        static constexpr const char* BLUE = "\033[94m";
        static constexpr const char* RESET = "\033[0m";
    };

    template<PrintType Type>
    void Log(const std::string& message) {
        static bool initialized = false;
        if (!initialized) {
            EnableANSI();
            initialized = true;
        }

        switch (Type) {
        case PrintType::OK:
            std::cout << Colors::GREEN << "[ GOOD ]" << Colors::RESET << " " << message;
            break;
        case PrintType::LOADING:
            std::cout << Colors::YELLOW << "[ <--> ]" << Colors::RESET << " " << message;
            break;
        case PrintType::FAIL:
            std::cout << Colors::RED << "[ FAIL ]" << Colors::RESET << " " << message;
            break;
        case PrintType::DEBUG:
            std::cout << Colors::BLUE << "[ INFO ]" << Colors::RESET << " " << message;
            break;
        }
        std::cout.flush();
    }
}