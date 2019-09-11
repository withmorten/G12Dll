#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <stdint.h>
#include "..\G12Dll\MemoryMgr.h"

#define G12DLL_NAME "S1Fixes"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\S1.hpp"
#include "..\G12Dll\S1.h"
#include "s1fixes.hpp"

int hMesh::LoadMSH(zCFileBIN &file, int tryToShare)
{
	int loadOK = this->zCMesh::LoadMSH(file, tryToShare);

	if (loadOK)
	{
		if (zoptions->Parm(zSTRING("zSaveMapMSH")))
		{
			this->SaveMSH(zSTRING("0000000000000000.MSH"));
		}
	}

	return loadOK;
}

void hWorld::Unarchive(zCArchiver &arc)
{
	this->zCWorld::Unarchive(arc);
}

void PatchSpacer(void)
{
	// InjectHook(0x004CB600, &hMesh::LoadMSH);
	// InjectHook(0x007073A2, &hWorld::Unarchive);
}

void Init(void)
{
	if (SPACER150)
	{
		G12AllocConsole();
		PatchSpacer();
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
