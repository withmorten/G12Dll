#include "..\G12Dll\G12Core.h"

#define G12DLL_NAME "G101Fixes"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G101.hpp"
#include "..\G12Dll\G101.h"
#include "g101fixes.hpp"

int hCMesh::LoadMSH(zCFileBIN &file)
{
	int loadOK = this->zCMesh::LoadMSH(file);

	if (loadOK)
	{
		if (zoptions->Parm(zSTRING("zSaveMapMSH")))
 		{
			this->SaveMSH(zSTRING("0000000000000000.MSH"));
		}
	}

	return loadOK;
}

void hCWorld::GenerateStaticWorldLighting(int &lightmode, zTBBox3D *updateBBox3D)
{
	int lm = 3;
	this->zCWorld::GenerateStaticWorldLighting(lm, updateBBox3D);
}

void hCGame::LoadWorld(int slotID, zSTRING &levelpath)
{
	this->game_testmode = TRUE;

	this->LoadMatlib();

	this->oCGame::LoadWorld(slotID, levelpath);

	if (zoptions->Parm(zSTRING("zSaveMapZEN")))
	{
		this->SaveWorld(zSTRING("0000000000000000.ZEN"), 1, FALSE);
	}
}

void hCGame::LoadMatlib()
{
	zoptions->ChangeDir(16);

	zFILE *f = zfactory->CreateZFile(zSTRING("matlib.ini"));

	zSTRING slash("\"");
	zSTRING equal("=");

	if (f->Exists() && !f->Open(FALSE))
	{
		f->Reset();

		zSTRING line;
		zSTRING matName;

		do
		{
			f->Read(line);

			if (line.Search("#", 1) > 0)
			{
				if (line.Search("\"", 1) >= 0)
				{
					line.Delete(slash, 3);
					matName = line.PickWord(1, slash, slash);
				}
				else
				{
					matName = line.PickWord(1, equal, equal);
					matName.TrimLeft(' ');
					matName.TrimRight(' ');
				}

				if (_stricmp(matName._Ptr, "matlib"))
				{
					this->LoadPML(matName);
				}
			}
		}
		while (!f->Eof());

		f->Close();
	}

	delete f;
	f = NULL;

	this->CompileWorld();
}

void hCGame::LoadPML(zSTRING &matfilename)
{
	zoptions->ChangeDir(16);
	matfilename += ".pml";

	zFILE *file = zfactory->CreateZFile(matfilename);

	if (file->Exists() && !file->Open(FALSE))
	{
		file->Reset();

		zCArchiver *arch = zarcFactory.CreateArchiverRead(file, 0);

		zSTRING cn, matName;
		unsigned short cv;
		zCMaterial *mat = NULL;
		long pos;

		while (file->Pos() + 3 < file->Size())
		{
			pos = file->Pos();
			arch->ReadChunkStart(cn, cv);
			matName = arch->ReadString("name");
			file->Seek(pos);

			mat = zCMaterial::SearchName(matName);

			if (!mat)
			{
				mat = dynamic_cast<zCMaterial *>(arch->ReadObject(NULL));
			}
			else
			{
				dynamic_cast<zCMaterial *>(arch->ReadObject(NULL));
			}

			if (mat)
			{
				mat->refCtr++;
			}
		}

		arch->Close();

		if (arch)
		{
			arch->Release();

			arch = NULL;
		}

		file->Close();
	}

	delete file;
	file = NULL;
}

void PatchGothic101(void)
{
	// InjectHook(0x00548D07, &hMesh::LoadMSH); // zCBspTree::LoadBIN()
	// InjectHook(0x00666FBA, &hWorld::GenerateStaticWorldLighting); // oCGame::CompileWorld()
	// InjectHook(0x0066411A, &hGame::LoadWorld); // oCGame::LoadGame()

	// What the fuck Windows 10??? Nop SendMessageA call ...
	PatchJump(0x00447345, 0x00447359);

	// write only vertexbuffer gone
	// Patch(0x00759336, (BYTE)0xEB);
}

void Init(void)
{
	if (GOTHIC101E)
	{
		G12AllocConsole();
		PatchGothic101();
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
