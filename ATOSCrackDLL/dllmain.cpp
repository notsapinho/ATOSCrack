#include "pch.h"
#include <Windows.h>
#include <string>

DWORD WINAPI MainThread(HMODULE hModule)
{
    int(__cdecl * ensureGIL)() = reinterpret_cast<int(__cdecl*)()>(GetProcAddress(GetModuleHandleA("Python310.dll"), "PyGILState_Ensure"));
    void(__cdecl * releaseGIL)(int) = reinterpret_cast<void(__cdecl*)(int)>(GetProcAddress(GetModuleHandleA("Python310.dll"), "PyGILState_Release"));
    void(__cdecl * runString)(char*) = reinterpret_cast<void(__cdecl*)(char*)>(GetProcAddress(GetModuleHandleA("Python310.dll"), "PyRun_SimpleString"));

    std::string code = R"(import os
class pwinput:
    def pwinput(prompt='Password: ', mask='*'):
        globals()['autor'] = "cracked_by_sapinho"
        os.system("cls")
        menu())";

    int state = ensureGIL();
    runString((char*)code.c_str());
    releaseGIL(state);

    FreeLibraryAndExitThread(hModule, 0);
    CloseHandle(hModule);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, NULL);
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}