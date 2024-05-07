#include "imports.h"
#include "globals.h"
#include "utils/security.h"

#include "internal/hooks.hpp"
HINSTANCE DllHandle;

#define WIN32_LEAN_AND_MEAN

DWORD WINAPI InitExec() {
    std::cout << xorstr_("[-] MainThread Started\n");
    if (MH_Initialize() != MH_OK) {
        return 0;
    }
    std::cout << xorstr_("[HOOKLIB] Initialized\n");


    /* Security Based Hooks */
    if (MH_CreateHookApi(L"kernel32.dll", xorstr_("GetThreadContext"), &hookedGetThreadContext, reinterpret_cast<void**>(&pGetThreadContext)) != MH_OK) {
        MessageBoxA(NULL, xorstr_("Failed To Hook GetThreadContext"), "", MB_OK);
    }
    else {
        std::cout << xorstr_("[security-enabled] GetThreadContext->dr. for HW Breakpoints\n");
    }

    if (MH_CreateHookApi(L"ntdll.dll", xorstr_("NtRaiseHardError"), &hookedNtRaiseHardError, reinterpret_cast<void**>(&pNtRaiseHardError)) != MH_OK) {
        MessageBoxA(NULL, xorstr_("Failed To Hook NtRaisedHardError"), "", MB_OK);
    }
    else {
        std::cout << xorstr_("[security-enabled] GetThreadContext->dr. for HW Breakpoints\n");
    }

    /* RPM / WPM Hooks */
    if (MH_CreateHookApi(L"kernel32.dll", xorstr_("WriteProcessMemory"), &hookedWriteProcessMemory, reinterpret_cast<void**>(&pWriteProcessMemory)) != MH_OK) {
        MessageBoxA(NULL, xorstr_("Failed To Hook WriteProcessMemory"), "", MB_OK);
    }
    else {
        std::cout << xorstr_("[enabled] WriteProcessMemory Dumper\n");
    }

    if (MH_CreateHookApi(L"kernel32.dll", xorstr_("ReadProcessMemory"), &hookedReadProcessMemory, reinterpret_cast<void**>(&pReadProcessMemory)) != MH_OK) {
        MessageBoxA(NULL, xorstr_("Failed To Hook ReadProcessMemory"), "", MB_OK);
    }
    else {
        std::cout << xorstr_("[enabled] ReadProcessMemory Dumper\n");
    }

    if (MH_CreateHookApi(L"kernel32.dll", xorstr_("DeleteFileW"), &hookedDeleteFileW, reinterpret_cast<void**>(&pDeleteFileW)) != MH_OK) {
        MessageBoxA(NULL, xorstr_("Failed To Hook DeleteFileW"), "", MB_OK);
    }
    else {
        std::cout << xorstr_("[enabled] DeleteFileW Dumper\n");
    }

    if (MH_CreateHookApi(L"kernel32.dll", xorstr_("DeleteFileA"), &hookedDeleteFileA, reinterpret_cast<void**>(&pDeleteFileA)) != MH_OK) {
        MessageBoxA(NULL, xorstr_("Failed To Hook DeleteFileA"), "", MB_OK);
    }
    else {
        std::cout << xorstr_("[enabled] DeleteFileA Dumper\n");
    }
    


    MH_EnableHook(MH_ALL_HOOKS);
    while (true) {
        Sleep(50);
        if (GetAsyncKeyState(VK_END) & 1) {
            break;
        }
    }

    return 0;
}

int __stdcall DllMain(const HMODULE hModule, const std::uintptr_t reason, const void* reserved) {
    if (reason == 1) {
        /* Alocate Console */
        if (globals::AllocateConsole == true) {
            AllocConsole();
            FILE* fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
        }

        DisableThreadLibraryCalls(hModule);
        std::cout << xorstr_("[-] AllocConsole - freopen_s | SET\n");
        DllHandle = hModule;

        hyde::CreateThread(InitExec, DllHandle);
        std::cout << xorstr_("[-] Started Main Thread...\n");

        //CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(InitExec), hModule, 0, nullptr);
        return true;
    }
    return true;
}

