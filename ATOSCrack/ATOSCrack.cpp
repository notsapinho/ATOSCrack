#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

PROCESS_INFORMATION startup(LPCSTR lpApplicationName)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    CreateProcessA
    (
        lpApplicationName,  
        NULL,                
        NULL,                 
        NULL,                
        FALSE,                  
        0,     
        NULL,           
        NULL,  
        &si,            
        &pi        
    );
  
    return pi;
}

std::string getExeRoot() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

std::vector<DWORD> ProcIdFromParentProcId(DWORD parentProcId) {
    std::vector<DWORD> vec; int i = 0;
    HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hp, &pe)) {
        do {
            if (pe.th32ParentProcessID == parentProcId) {
                vec.push_back(pe.th32ProcessID); i++;
            }
        } while (Process32Next(hp, &pe));
    }
    CloseHandle(hp);
    
    return vec;
}

int main()
{
    PROCESS_INFORMATION p = startup("atos.exe");

    Sleep(3000);

    std::string path = getExeRoot() + "\\ATOSCrackDLL.dll";

    std::vector<DWORD> childProcesses = ProcIdFromParentProcId(p.dwProcessId);

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, childProcesses[0]);

    LPVOID Alloc = VirtualAllocEx(hProc, NULL, path.length() + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    WriteProcessMemory(hProc, Alloc, path.c_str(), path.length() + 1, 0);
   
    CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(LoadLibrary(L"kernel32"), "LoadLibraryA"), Alloc, 0, 0);
    
    CloseHandle(p.hProcess);
    CloseHandle(p.hThread);
    CloseHandle(hProc);

    system("cls");

    std::cout << R"(      |       |        |       | |
 ' |   |   |     '  |      '      
              |           |     | 
 '     |  _,..--I--..,_ |         
   / _.-`` _,-`   `-,_ ``-._ \ 
     `-,_,_,.,_   _,.,_._,-`      
|  | '   '     `Y` __ '     '     
  '|        ,-. I /  \       |  | 
 |    |    /   )I \  /     '   |  
'  '      /   / I_.""._           
|  |    ,l  .'..`      `.   ' |  |
 |     / | /   \        l         
      /, '"  .  \      ||   |   | 
 |  ' ||      |"|      ||   |     
'     ||      | |      ||       | 
|     \|      | '.____,'/  |  |   
   |   |      |  |    |F   '    | 
 | '   |      |  | |\ |     ' |   
       |      |  | || |      |    
|  |   |      |  | || |    |    | 
       |      |  | || |      |    
 ' |   '.____,'  \_||_/   |    |  
         |/\|    [_][_]      |    
"""""""""""""""""""""""""""""""""")" << std::endl;

    std::cout << "nao tenho nada pra fazer e perdi meu tempo fazendo isso" << 
        std::endl << "tchau agora vou chorar pq ela nao me ama" << 
        std::endl << "aperta enter e seja feliz" << std::endl;
}
