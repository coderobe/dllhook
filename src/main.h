#ifndef _MAIN_H_
#define _MAIN_H_

#pragma once

#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

DWORD WINAPI dwMain(LPVOID lpArg);
BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved);

/*
	Attempts to find address based on a matching (@pattern, and @mask),
	starts search at @start and ends at @start+@size

	@param start
	@param size
	@param pattern
	@param mask

	@return byte* match | NULL no match
*/
byte* GetAddressByPattern(byte* start, int size, byte* pattern, char* mask){
	for (int i = 0; i < size; i++)
	{
		bool found = true;
		for (int j = 0; mask[j]; j++)
		{
			if (mask[j] != '?' && pattern[j] != start[i + j])
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return (byte*)&start[i];
		}
	}

	return NULL;
}

char* GetAddressByData(const char* data, size_t len){
    HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
    if(process)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        MEMORY_BASIC_INFORMATION info;
        std::vector<char> chunk;
        char* p = 0;
        while(p < si.lpMaximumApplicationAddress)
        {
            if(VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info))
            {
                p = (char*)info.BaseAddress;
                chunk.resize(info.RegionSize);
                SIZE_T bytesRead;
                if(ReadProcessMemory(process, p, &chunk[0], info.RegionSize, &bytesRead))
                {
                    for(size_t i = 0; i < (bytesRead - len); ++i)
                    {
                        if(memcmp(data, &chunk[i], len) == 0)
                        {
                            return (char*)p + i;
                        }
                    }
                }
                p += info.RegionSize;
            }
        }
    }
    return 0;
}

/*
	Creates a JMP in code in memory from the @origin to the @destination, 
	return address and function return are @length+@origin

	@param origin
	@param destination
	@parmam length

	@return byte* retrun address
*/
byte* CreateJumpTo(byte* orgin, byte* destination, int length)
{
	DWORD oldProtect;
	VirtualProtect(orgin, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

	*orgin = 0xE9; //JMP
	*(DWORD*)(orgin + 1) = (DWORD)(destination - orgin - 5);

	VirtualProtect(orgin, 5, oldProtect, &oldProtect);

	return orgin + length;
}

#endif
