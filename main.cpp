#include <windows.h>
#include "main.h"

DWORD DLL_EXPORT Trace(DWORD hFileMap, DWORD dwSizeMap, DWORD dwTimeOut, DWORD dwToTrace,
					  DWORD dwExactCall)
{
	DWORD* dwPtrOutput = (DWORD*)MapViewOfFile((HANDLE)hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (!dwPtrOutput)
	{
		return (201);
	}

	if (dwSizeMap < 4)
	{
		UnmapViewOfFile((LPCVOID)dwPtrOutput);
		CloseHandle((HANDLE)hFileMap);
		return (203);
	}

	if (IsBadReadPtr((VOID*)dwToTrace, 4))
	{
		UnmapViewOfFile((LPCVOID)dwPtrOutput);
		CloseHandle((HANDLE)hFileMap);
		return (205);
	}

	BYTE *to_trace = (BYTE*)dwToTrace;

	while (!IsBadReadPtr((VOID*)to_trace, 4))
	{
		if ((to_trace[0] == 0x40 && to_trace[1] == 0xFF && to_trace[2] == 0x30))
		{
			DWORD address = *((DWORD*)(to_trace-4));
            address++;
			if (!IsBadReadPtr((VOID*)address, 4))
			{
				*dwPtrOutput = *((DWORD*)address);
				UnmapViewOfFile((LPCVOID)dwPtrOutput);
				CloseHandle((HANDLE)hFileMap);
				return (200);
			}
		}
		to_trace++;
	}
	UnmapViewOfFile((LPCVOID)dwPtrOutput);
	CloseHandle((HANDLE)hFileMap);
	return (200);
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
