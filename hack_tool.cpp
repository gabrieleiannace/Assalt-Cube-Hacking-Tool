#include "mem.h"
#include "procedure.h"
#include <iostream>

#define LOCAL_PLAYER_OFFSET 0x10f4f4
#define HEALTH_OFFSET {0xf8}
#define SHOT_AMMO_DEC_OFFSET 0x637e9
#define RECOIL_FUNC_OFFSET 0x63786

int main(int argc, char *argv[]){
   
    DWORD procId = 0;
    procId = GetProcId("ac_client.exe");

    uintptr_t moduleBaseAddress = 0, healthAddress = 0, localPlayer = 0;
    bool bAmmo = false, bRecoil = false, bHealth = false;

    const int newValue = 2001;
    
    HANDLE hProc = 0;

    if(procId){

        moduleBaseAddress = GetModuleBaseAddress(procId, "ac_client.exe");

        hProc = OpenProcess(PROCESS_ALL_ACCESS, false, procId);

        localPlayer = moduleBaseAddress + LOCAL_PLAYER_OFFSET;

        healthAddress = FindDMAAddy(hProc, localPlayer, HEALTH_OFFSET);

    }
    else{

        std::cout<< "Process not found, press ENTER to close\n";
        getchar();
        return 1;

    }


    DWORD procExit = 0;
    //Stay sure that the process is still running.
    while(GetExitCodeProcess(hProc, &procExit) && procExit == STILL_ACTIVE){

        if(GetAsyncKeyState(VK_F1)){
            bHealth = !bHealth;
        }
        else if(GetAsyncKeyState(VK_F2)){
            bAmmo = !bAmmo;

            if(bAmmo){
                //xff x06 = inc [esi]
                mem::PatchEx((BYTE *)moduleBaseAddress + SHOT_AMMO_DEC_OFFSET,(BYTE *) "\xFF\x06", 2, hProc);
            }
            else{
                //xff x0E = dec [esi]
                mem::PatchEx((BYTE *)moduleBaseAddress + SHOT_AMMO_DEC_OFFSET, (BYTE *)"\xFF\x0E", 2, hProc);
            }
        }
        else if(GetAsyncKeyState(VK_F3)){
            bRecoil = !bRecoil;

            if(bRecoil){
                mem::NopEx((BYTE *)moduleBaseAddress + RECOIL_FUNC_OFFSET, 10, hProc);
            }
            else{
                mem::PatchEx((BYTE *)moduleBaseAddress + RECOIL_FUNC_OFFSET, (BYTE *)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProc);
            }
        }
        if(bHealth){
            mem::PatchEx((BYTE *)healthAddress, (BYTE *)&newValue, sizeof(newValue), hProc);
        }
        else{
            
        }
        Sleep(5);
    }



    return 0;
}