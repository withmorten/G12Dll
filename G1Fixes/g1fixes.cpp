#include "..\G12Dll\G12Core.h"

#define G12DLL_NAME "G1Fixes"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G1.hpp"
#include "..\G12Dll\G1.h"
#include "g1fixes.hpp"

void hCNpc::CreateVobList(float max_dist)
{
	zCVob *vob = NULL;
	oCMobInter *mob = NULL;
	oCNpc *npc = NULL;
	zVEC3 trafo_vec;
	zTBBox3D bbox;
	zCClassDef *classDef;
	int i;

	bool delete_vob;

	if (this->homeWorld)
	{
		this->ClearVobList();

		trafo_vec = this->GetPositionWorld();

		bbox.maxs = trafo_vec + max_dist;
		bbox.mins = trafo_vec - max_dist;

		this->homeWorld->bspTree.bspRoot->CollectVobsInBBox3D(this->vobList, bbox);

		for (i = 0; i < this->vobList.numInArray; i++)
		{
			delete_vob = FALSE;

			vob = this->vobList[i];
			classDef = vob->_GetClassDef();

			if (vob == this)
			{
				delete_vob = TRUE;
			}

			mob = zDYNAMIC_CAST<oCMobInter>(vob);

			if (mob)
			{
				if (mob->IsOccupied())
				{
					delete_vob = TRUE;
				}
			}

			npc = zDYNAMIC_CAST<oCNpc>(vob);

			if (npc)
			{
				if (npc->IsDead() && npc->inventory2.IsEmpty())
				{
					delete_vob = TRUE;
				}
			}

			if (delete_vob)
			{
				this->vobList.RemoveIndex(i--);
			}
			else
			{
				vob->AddRef();
			}
		}
	}
}

void PatchGothic(void)
{

	if (G12GetPrivateProfileBool("HideFocus", FALSE))
	{
		// Unlike HideFocus from Systempack which is sometimes buggy and where vobs can still be focused when turning around quickly and spamming ctrl
		// this patches CreateVobList() to the Sequel variant where a dead, empty NPC does not even end up in the focusable voblist
		InjectHook(0x006909FB, &hCNpc::CreateVobList); // oCNpc::ToggleFocusVob()
		InjectHook(0x00690E43, &hCNpc::CreateVobList); // oCNpc::CollectFocusVob()
		InjectHook(0x006B731F, &hCNpc::CreateVobList); // oCNpc::PerceiveAll()
		InjectHook(0x006B7565, &hCNpc::CreateVobList); // oCNpc::PerceptionCheck()
	}
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
