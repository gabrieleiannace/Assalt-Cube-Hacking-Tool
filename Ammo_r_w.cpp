#include "procedure.h"
#include <iostream>

int main(int argc, char *argv[]){
    
    DWORD procId = GetProcId("ac_client.exe");

    uintptr_t moduleBaseAddress = GetModuleBaseAddress(procId, "ac_client.exe");

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, procId);

    uintptr_t dynamicAddress = moduleBaseAddress + 0x10F4F4;

    uintptr_t ammoAddress = FindDMAAddy(hProc, dynamicAddress, {0x374, 0x14, 0x0});

    while(true){
        int ammoValue = 0;
        ReadProcessMemory(hProc, (BYTE *)ammoAddress, &ammoValue, sizeof(ammoValue), 0);

        std::cout<< "Ammo Value : " << ammoValue << std::endl;
        std::cout << "Press Enter to close..\n";
        Sleep(1000);
    }
    return 0;
}