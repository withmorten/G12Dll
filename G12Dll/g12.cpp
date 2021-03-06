#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <stdint.h>
#include "MemoryMgr.h"

#define G12DLL_NAME "G12"

#include "G12.h"

void PatchGothic107(void)
{
	// Nop(0x00642EEC, 3); // alphablendfunc barrier
	PatchJump(0x0044A6F7, 0x0044A70C); // SendMessageA

	// wbuffer tests
	// Nop(0x0072CB73);
	// Patch(0x0072CB73 + 1, (BYTE)0xE9);

#if 0
	PatchJump(0x0072C8B6, 0x0072C8D3);
	Nop(0x0072C8B6, 2);
	Patch(0x0072C8B6 + 0, (BYTE)0x31);
	Patch(0x0072C8B6 + 1, (BYTE)0xC0);
	Nop(0x0072C8B8, 2);
	Nop(0x0072C8BA, 2);
	Nop(0x0072C8BC, 10);
	Nop(0x0072C8C6, 5);
	Nop(0x0072C8CB, 2);
	Nop(0x0072C8CD, 6);
#endif
}

void PatchGothic108j(void)
{
	// Patch(0x0071CFA4 + 1, "DEFAULT.TGA");
}

void PatchGothic108k(void)
{
	if (G12GetPrivateProfileBool("BarrierIgnoreSkyEffectsSetting", FALSE))
	{
		// Ignore skyEffects setting for oCSkyControler_Barrier::RenderSkyPre
		Nop(0x00632152, 6);
	}

	if (G12GetPrivateProfileBool("ShowTime", FALSE))
	{
		// Enable Show Time
		Patch(0x006357B8 + 1, (BYTE)0x86);
	}
}

const char *SpacerAppName = "Spacer 1.50";
const char *SpacerAppVersion = "1.50";
const float SpacerFarClipping = 3.9f; // ??? 4.0f

void PatchSpacer150(void)
{
	if (G12GetPrivateProfileBool("SpacerDisableNumlock", FALSE))
	{
		// Disable NumLock
		Patch(0x0073A0DF, (BYTE)0xEB);
		Patch(0x0079E4E0, (BYTE)0xEB);
	}

	if (G12GetPrivateProfileBool("BarrierIgnoreSkyEffectsSetting", FALSE))
	{
		// Ignore skyEffects setting in oCSkyControler_Barrier::RenderSkyPre()
		Nop(0x0065B932, 6);
	}

	if (G12GetPrivateProfileBool("SpacerNoLODGenerate", FALSE))
	{
		// Don't generate LOD polygons
		Patch(0x005A1D13, (BYTE)0xEB);
	}

	if (G12GetPrivateProfileBool("SpacerFastMSHSave", FALSE))
	{
		// Faster mesh saving? Untested ... but seems to work
		Patch(0x004FCF9D, (BYTE)0xEB);
	}

	// Enable saving of (small) meshes ...
	Patch(0x007A5C8A, (BYTE)0xEB);

	// Don't save mesh for compiled ascii
	Patch(0x007A1FD2 + 1, (BYTE)FALSE);

	// Don't save mesh for uncompiled ascii
	Patch(0x007A2095 + 1, (BYTE)FALSE);

	// Don't show "memory leaks found" everytime quitting
	Patch(0x005354A2 + 1, (BYTE)2);

	// BSP vobs ...
	Patch(0x004C10FE + 1, (BYTE)2);

	// BSP lights ...
	Patch(0x004C11E1 + 1, (BYTE)2);

	// APP_NAME
	Patch(0x00946778, SpacerAppName);

	// APP_VERSION
	Patch(0x0094677C, SpacerAppVersion);

	// Max farClipping 4.0f (seems to be 4.1f?) So let's do 3.9f
	// Patch(0x007ADA39 + 2, 0x008C4148);
	Patch(0x007ADA39 + 2, &SpacerFarClipping);

	// No skyplanets
	Nop(0x0056A3F4, 5);
	Nop(0x0056A40A, 5);
	Nop(0x0056B164, 5);

	// No rain, ever
	Nop(0x0056B312, 2);

	// No "CreatingVobtreeItem" zspy message
	Nop(0x007DB049);
	Patch(0x007DB049 + 1, (BYTE)0xE9);

	// No "Merged waypoints"
	Nop(0x007A42D2);
	Patch(0x007A42D2 + 1, (BYTE)0xE9);

	Nop(0x007DC549);
	Patch(0x007DC549 + 1, (BYTE)0xE9);

	// Vob info is not packed
	Patch(0x007B6978 + 1, (BYTE)FALSE);
}

void PatchGothic26(void)
{
	if (G12GetPrivateProfileBool("ShowTime", FALSE))
	{
		// Enable Show Time
		Patch(0x006BF92E + 1, (BYTE)0x86);
	}

	if (G12GetPrivateProfileBool("GodMode", FALSE))
	{
		// oCNpc::godmode
		Patch(0x00AB2660, 1);
	}
}

void PatchSpacer26(void)
{
	if (G12GetPrivateProfileBool("SpacerDisableNumlock", FALSE))
	{
		// Disable NumLock
		Patch(0x0048A63F, (BYTE)0xEB);
		Patch(0x0041C426, (BYTE)0xEB);
	}

	// Fix crash when exiting Spacer2 with log level >= 7
	PatchJump(0x0041BC9D, 0x0041BCBD);
	PatchJump(0x0041BD6D, 0x0041BD92);

	// Enable saving of (small) meshes ...
	Patch(0x00423C8D, (BYTE)0xEB);
}

void Init(void)
{
	if (GOTHIC107C)
	{
		G12AllocConsole();
		PatchGothic107();
	}
	else if (GOTHIC108J)
	{
		G12AllocConsole();
		PatchGothic108j();
	}
	else if (GOTHIC108KM)
	{
		G12AllocConsole();
		PatchGothic108k();
	}
	else if (SPACER150)
	{
		G12AllocConsole();
		PatchSpacer150();
	}
	else if (GOTHIC26)
	{
		G12AllocConsole();
		PatchGothic26();
	}
	else if (SPACER26)
	{
		G12AllocConsole();
		PatchSpacer26();
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
