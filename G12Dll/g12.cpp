#include "G12Core.h"

#define G12DLL_NAME "G12"

#include "G12.h"

namespace Gothic101e
{
ASM(fix_num_texstages)
{
	__asm
	{
		mov[ebp + 0x4ec], eax
		cmp eax, 4
		jle locret
		mov[ebp + 0x4ec], 4
		locret:
	}

	RET(0x0074B4F9);
}

void Apply(void)
{
	// What the fuck Windows 10??? Nop SendMessageA call ...
	PatchJump(0x00447345, 0x00447359);

	Nop(0x005ED2BA, 7); // fps

	InjectHook(0x0074B4F3, &fix_num_texstages, PATCH_JUMP);
}
}

namespace Gothic104d
{
ASM(fix_num_texstages)
{
	__asm
	{
		mov[ebp + 0x4ec], eax
		cmp eax, 4
		jle locret
		mov[ebp + 0x4ec], 4
		locret:
	}

	RET(0x0074B4F9);
}

void Apply(void)
{
	PatchJump(0x00445125, 0x0044513C); // SendMessageA

	Nop(0x005D110A, 7); // fps

	InjectHook(0x00718F22, &fix_num_texstages, PATCH_JUMP);
}
}

namespace Gothic107
{
ASM(fix_num_texstages)
{
	__asm
	{
		mov[ebp + 0x4ec], eax
		cmp eax, 4
		jle locret
		mov[ebp + 0x4ec], 4
		locret:
	}

	RET(0x0072CE1B);
}

void Apply(void)
{
	PatchJump(0x0044A6F5, 0x0044A70C); // SendMessageA

	Nop(0x005DCECA, 7); // fps

	InjectHook(0x0072CE15, &fix_num_texstages, PATCH_JUMP);
}
}

namespace Gothic108k
{
void Apply(void)
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
}

namespace Gothic112f
{
void Apply(void)
{
	// Fix Fps
	Nop(0x005ED341, 7);
}
}

namespace Spacer142g
{
const char *GameDat = "game.dat";

void Apply(void)
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
}

namespace Spacer150
{
const char *SpacerAppName = "Spacer 1.50";
const char *SpacerAppVersion = "1.50";
const float SpacerFarClipping = 3.9f; // ??? 4.0f

void Apply(void)
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
}

namespace Gothic26
{
void Apply(void)
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
}

namespace Spacer26
{
void Apply(void)
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

	if (G12GetPrivateProfileBool("MergeVobsWithLevel", FALSE))
	{
		Nop(0x007AE26F, 6);
		Nop(0x007AE3D4, 6);

		Nop(0x007AD9A7, 6);
		Nop(0x007AD9C4, 6);
		Nop(0x007AD9D0, 6);
		Nop(0x007AD9DC, 6);
		Nop(0x007AD9F7, 6);
		Nop(0x007ADA3F, 6);
	}
}
}

void Init(void)
{
	G12AllocConsole();

	if (GOTHIC101E)
	{
		Gothic101e::Apply();
	}
	else if (GOTHIC104D)
	{
		Gothic104d::Apply();
	}
	else if (GOTHIC107C)
	{
		Gothic107::Apply();
	}
	else if (GOTHIC108KMOD)
	{
		Gothic108k::Apply();
	}
	else if (GOTHIC112F)
	{
		Gothic112f::Apply();
	}
	else if (SPACER142G)
	{
		Spacer142g::Apply();
	}
	else if (SPACER150)
	{
		Spacer150::Apply();
	}
	else if (GOTHIC26FIX)
	{
		Gothic26::Apply();
	}
	else if (SPACER26MOD)
	{
		Spacer26::Apply();
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
