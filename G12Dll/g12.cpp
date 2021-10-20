#include "G12Core.h"

#define G12DLL_NAME "G12"

#include "G12.h"

void PatchGothic107(void)
{
	PatchJump(0x0044A6F7, 0x0044A70C); // SendMessageA
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

	// Disable rain in vobs
	// Patch(0x005B90A4 + 1, 736);

	// Don't show "memory leaks found" everytime quitting
	Patch(0x0058B7F7 + 1, (BYTE)2);

	// Debug damage (this doesn't work with SystemPack)
	// Patch(0x009D987C, TRUE);
}

void PatchGothic112f(void)
{
	// Fix Fps
	Nop(0x005ED341, 7);
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

	if (G12GetPrivateProfileBool("BarrierIgnoreSkyEffectsSetting", FALSE) || TRUE)
	{
		// Ignore skyEffects setting in oCSkyControler_Barrier::RenderSkyPre()
		Nop(0x0065B932, 6);
	}

	if (G12GetPrivateProfileBool("SpacerNoLODGenerate", TRUE))
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
		Patch(0x00AB2660, TRUE);
	}

	// Disable rain in vobs
	// Patch(0x005E2279 + 1, 736);

	// Debug damage (this doesn't work with SystemPack)
	// Patch(0x00AAC60C, TRUE);
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

	// Gothic 1 uncompiled Zens and Material libraries would be compatible in Spacer2 if the following values were read safely
	// Nopping them results in Spacer2 using the default values instead of overwriting them with garbage

	// Note that the Unarchiver will still complain about many missing entries from other values, but those get read safely and the original value remains intact

	if (G12GetPrivateProfileBool("SpacerG1Pmls", FALSE))
	{
		// Only enable this temporarily if you want to open Gothic 1 PMLs

		// zCMaterial::Unarchive()
		{
			// Some WATER materials will require manual setting of the alphaFunc, as in Gothic 1 it got used differently on WATER

			// detailObjectScale
			PatchJump(0x006F10E5, 0x006F10F7);

			// forceOccluder
			PatchJump(0x006F10F7, 0x006F1112);

			// environmentalMapping
			PatchJump(0x006F1112, 0x006F112D);

			// environmentalMappingStrength
			PatchJump(0x006F112D, 0x006F113C);

			// waveMode
			PatchJump(0x006F113C, 0x006F115A);

			// waveSpeed
			PatchJump(0x006F115A, 0x006F1175);

			// ignoreSunLight
			PatchJump(0x006F1195, 0x006F11B0);

			// alphaFunc
			PatchJump(0x006F11B0, 0x006F11C2);
			Nop(0x006F11C2);
			Patch(0x006F11C2 + 1, (BYTE)0xE9);
			Nop(0x006F127E, 3);
		}
	}

	if (G12GetPrivateProfileBool("SpacerG1Zens", FALSE))
	{
		// Keep this enabled as long as you need to work on a Gothic 1 uncompiled ZEN
		// I would not advise resaving it as an uncompiled Gothic 2 ZEN, because you will lose any items you have not implemented in your scripts yet

		// zCVob::UnarchiveVerbose()
		{
			// visualAniMode
			PatchJump(0x0078A8A0, 0x0078A8B5);

			// visualAniModeStrength
			PatchJump(0x0078A8B5, 0x0078A8C7);

			// vobFarClipZScale
			PatchJump(0x0078A8C7, 0x0078A8D9);
		}
	}
}

void Init(void)
{
	if (GOTHIC107C)
	{
		G12AllocConsole();
		PatchGothic107();
	}
	else if (GOTHIC108KMOD)
	{
		G12AllocConsole();
		PatchGothic108k();
	}
	else if (GOTHIC112F)
	{
		G12AllocConsole();
		PatchGothic112f();
	}
	else if (SPACER150)
	{
		G12AllocConsole();
		PatchSpacer150();
	}
	else if (GOTHIC26FIX)
	{
		G12AllocConsole();
		PatchGothic26();
	}
	else if (SPACER26MOD)
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
