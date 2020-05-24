#ifndef _MEM_H_
#define _MEM_H_

#include "mem.h"

void mem::PatchEx(BYTE *dst, BYTE *src, unsigned int size, HANDLE hProc){
    DWORD oldProtection;
    VirtualProtectEx(hProc, dst,size,PAGE_EXECUTE_READWRITE, &oldProtection);
    WriteProcessMemory(hProc, dst, src, size, 0);
    VirtualProtectEx(hProc, dst, size, oldProtection,&oldProtection);
}

void mem::NopEx(BYTE *dst, unsigned int size, HANDLE hProc){
    BYTE *nopArray = new BYTE[size];
    memset(nopArray, 0x90, size);
    PatchEx(dst, nopArray, size, hProc);
    delete[] nopArray;
}

#endif