#include "..\G12Dll\G12Core.h"

#define G12DLL_NAME "G1Barrier"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G1.hpp"
#include "..\G12Dll\G1.h"
#include "g1barrier.hpp"

void PatchGothic(void)
{

}

void Init(void)
{
	if (GOTHIC108KMOD)
	{
		G12AllocConsole();
		PatchGothic();
	}
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Init();
	}

	return TRUE;
}
