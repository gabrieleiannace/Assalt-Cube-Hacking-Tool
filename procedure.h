#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_


#include <windows.h>
#include <tlhelp32.h>
#include <cstdint>
#include <vector>


DWORD GetProcId(const char *procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const char *moduleName);

uintptr_t FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets);


#endif