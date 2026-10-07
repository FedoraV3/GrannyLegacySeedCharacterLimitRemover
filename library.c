#include <stdint.h>
#include <stdio.h>
#include <Windows.h>

static uint8_t bytes[] = {0xFF, 0xFF, 0xFF, 0x7F};
#define bytes_size 4

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved )  // reserved
{
    if (fdwReason == DLL_PROCESS_ATTACH) {
        uint64_t base = (uint64_t)GetModuleHandleW(L"GameAssembly.dll");
        uint64_t address = 0x000000000024B795;

        uint64_t target_address = (base + address);

        DWORD old_protect = 0;
        if (VirtualProtect(
            (LPVOID)target_address,
            4,
            PAGE_EXECUTE_READWRITE,
            &old_protect
        ) == 0) {
            return FALSE;
        }

        memcpy((void*)target_address, bytes, bytes_size);

        DWORD old_old_protect = 0;
        if (VirtualProtect(
            (LPVOID)target_address,
            bytes_size,
            old_protect,
            &old_old_protect
        ) == 0) {
            return FALSE;
        }
    }

    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}