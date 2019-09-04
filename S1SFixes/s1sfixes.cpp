#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <stdint.h>
#include "..\G12Dll\MemoryMgr.h"

#define G12DLL_NAME "S1SFixes"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\S1S.hpp"
#include "..\G12Dll\S1S.h"
#include "s1sfixes.hpp"

const char *GameDat = "game.dat";

void PatchSpacerS1S(void)
{
	if (G12GetPrivateProfileBool("DisableNumlock", FALSE))
	{
		// Disable NumLock
		Patch(0x0074D23C, (BYTE)0xEB);
		Patch(0x007B54D6, (BYTE)0xEB);
	}

	if (G12GetPrivateProfileBool("NoLODGenerate", TRUE))
	{
		// Don't generate LOD polygons
		Patch(0x005AAA23, (BYTE)0xEB);
	}

	if (G12GetPrivateProfileBool("FastMSHSave", FALSE))
	{
		// Faster mesh saving? Untested ... but seems to work
		Patch(0x005021F9, (BYTE)0xEB);
	}

	// Enable saving of (indoor) meshes ...
	Patch(0x007BCBEA, (BYTE)0xEB);

	// Don't save mesh for compiled ascii
	Patch(0x007B904F + 1, (BYTE)FALSE);

	// Don't save mesh for uncompiled ascii
	Patch(0x007B90C0 + 1, (BYTE)FALSE);

	// Don't show "memory leaks found" everytime quitting
	Patch(0x0053AED2 + 1, (BYTE)2);

	// BSP vobs ...
	Patch(0x004C4CCE + 1, (BYTE)2);

	// BSP lights ...
	Patch(0x004C4DB1 + 1, (BYTE)2);

	// Use game.dat ...
	Patch(0x00667A02 + 1, GameDat);
	Patch(0x00667A36 + 1, GameDat);
	Patch(0x00667A5D + 1, GameDat);
	Patch(0x00757CD0 + 1, GameDat);
}

void Init(void)
{
	if (SPACER142G)
	{
		G12AllocConsole();
		PatchSpacerS1S();
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
