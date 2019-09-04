#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <stdint.h>
#include "..\G12Dll\MemoryMgr.h"

#define G12DLL_NAME "G2Fixes"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G2.hpp"
#include "..\G12Dll\G2.h"
#include "g2fixes.hpp"

void hNpc::CreateVobList(float max_dist)
{
	zCVob *vob;
	oCMobInter *mob;
	oCNpc *npc;
	zVEC3 trafo_vec;
	zTBBox3D bbox;
	zCClassDef *classDef;
	int i;

	int delete_vob;

	if (this->homeWorld)
	{
		this->ClearVobList();

		this->trafoObjToWorld.GetTranslation(trafo_vec);

		bbox.maxs = trafo_vec + max_dist;
		bbox.mins = trafo_vec - max_dist;

		this->homeWorld->bspTree.bspRoot->CollectVobsInBBox3D(this->vobList, bbox);

		for (i = 0; i < this->vobList.numInArray; i++)
		{
			delete_vob = FALSE;

			vob = this->vobList.array[i];
			classDef = vob->_GetClassDef();

			if (vob == this)
			{
				delete_vob = TRUE;
			}

			if (zCObject::CheckInheritance(&oCMobInter::classDef, classDef))
			{
				mob = (oCMobInter *)vob;

				if (mob->IsOccupied())
				{
					delete_vob = TRUE;
				}
			}

			if (zCObject::CheckInheritance(&oCNpc::classDef, classDef))
			{
				npc = (oCNpc *)vob;

				if (npc->attribute[0] <= 0 && npc->inventory2.IsEmpty(TRUE, TRUE))
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
				vob->refCtr++;
			}
		}
	}
}

// SPL_* should probably be read from INI, just using unused values for now
#define SPL_LIGHTNINGFLASH 29
#define SPL_PYROKINESIS 37
#define SPL_CONTROL 64
#define SPL_TELEKINESIS 65
#define SPL_TELEKINESIS2 66
#define SPL_CHAINLIGHTNING 67
#define SPL_HEAL 68

#define BS_MOD_CONTROLLED 2048
#define BS_MOD_CONTROLLING 8192

void hNpc::OnDamage_Events(oSDamageDescriptor &descDamage)
{
	this->oCNpc::OnDamage_Events(descDamage);

	hSpell *spell = NULL;
	oCNpc *npcTarget = NULL;
	oCNpc *npcAttacker = descDamage.pNpcAttacker;

	if (npcAttacker)
	{
		spell = (hSpell *)npcAttacker->IsSpellActive(SPL_CONTROL);
		npcTarget = spell ? spell->spellTargetNpc : NULL;
	}

	if (spell && npcTarget && this->attribute[0] <= 0)
	{
		spell->EndTimedEffect();
	}

	if (this->HasBodyStateModifier(BS_MOD_CONTROLLING))
	{
		spell = (hSpell *)npcAttacker->IsSpellActive(SPL_CONTROL);

		if (spell)
		{
			spell->EndTimedEffect();
		}
	}
}

#define GAME_LEFT 1
#define GAME_RIGHT 2
#define GAME_UP 3

void hSpell::DoLogicInvestEffect()
{
	if (!this->spellCasterNpc)
	{
		return;
	}

	if (this->spellID == SPL_TELEKINESIS || this->spellID == SPL_TELEKINESIS2)
	{
		if (!this->manaInvested)
		{
			this->spellCasterNpc->CheckForOwner(this->spellTarget);
		}

		if (!this->spellCasterNpc->GetModel()->IsAniActive(zSTRING("S_TELSHOOT")))
		{
			return;
		}

		zVEC3 move, add, right, tmp, pos, curPos;
		float fac;
		int inMove;

		float speed = 75.0f / 1000.0f;

		move.n[0] = 0.0f;
		move.n[1] = 0.0f;
		move.n[2] = 0.0f;

		add = this->spellTarget->trafoObjToWorld.GetTranslation() - this->spellCasterNpc->trafoObjToWorld.GetTranslation();
		add.n[1] = 0.0f;
		add.NormalizeApprox();

		right.n[0] = 0.0f;
		right.n[1] = 1.0f;
		right.n[2] = 0.0f;
		right = right ^ add;

		if (zinput->GetState(GAME_UP))
		{
			if ((this->spellTarget->trafoObjToWorld.GetTranslation()
				- this->spellCasterNpc->trafoObjToWorld.GetTranslation()).LengthApprox() > 200.0f)
			{
				move += -add;
			}
		}

		if (zinput->GetState(GAME_LEFT))
		{
			move += -right;
		}
		else if (zinput->GetState(GAME_RIGHT))
		{
			move += right;
		}

		fac = speed * ztimer.frameTimeFloat;

		if (this->up < 150.0f)
		{
			tmp.n[0] = 0.0f;
			tmp.n[1] = 1.0f;
			tmp.n[2] = 0.0f;

			move += tmp;
			up += fac;
		}

		pos = this->spellTarget->trafoObjToWorld.GetTranslation();
		pos += move * fac;

		if (this->up >= 150.0f)
		{
			this->hoverY += this->hoverDir * ztimer.frameTimeFloat * speed / 4.0f;

			if (this->hoverY <= -5.0f || this->hoverY >= 5.0f)
			{
				this->hoverDir = -this->hoverDir;
			}

			move.n[1] += this->hoverY - this->hoverOld;
			this->hoverOld = this->hoverY;
		}

		curPos = pos;
		curPos += move;

		inMove = (this->spellCasterNpc->isInMovementMode != 0);

		if (inMove)
		{
			this->spellCasterNpc->EndMovement(TRUE); // possibly FALSE
		}

		this->spellTarget->SetPositionWorld(curPos);

		if (inMove)
		{
			this->spellCasterNpc->BeginMovement();
		}
	}
}

static int investHookRead = FALSE;
static BYTE oCSpell_Invest_Hook_Bytes[HOOK_SIZE];

void oCSpell_Invest_Hook_End(void)
{
	PatchBytes(0x004851F9, oCSpell_Invest_Hook_Bytes, sizeof(oCSpell_Invest_Hook_Bytes)); // oCSpell::Invest()
}

ASM(oCSpell_Invest_Hook_Start)
{
	__asm
	{
		mov ecx, ebp
		call hSpell::DoLogicInvestEffect
		pushad // needed ?
		call oCSpell_Invest_Hook_End
		popad // needed ?
	}

	RET(0x004851F9)
}

void oCSpell_Invest_Hook(void)
{
	if (!investHookRead)
	{
		investHookRead = TRUE;
		ReadBytes(0x004851F9, oCSpell_Invest_Hook_Bytes, sizeof(oCSpell_Invest_Hook_Bytes)); // oCSpell::Invest()
	}

	InjectHook(0x004851F9, oCSpell_Invest_Hook_Start, PATCH_JUMP); // oCSpell::Invest()
}

int hSpell::IsInvestSpell()
{
	switch (this->spellID)
	{
	case SPL_LIGHTNINGFLASH:
	case SPL_PYROKINESIS:
	case SPL_CHAINLIGHTNING:
	case SPL_HEAL:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

int hSpell::CastSpecificSpell()
{
	int rV = this->oCSpell::CastSpecificSpell();

	if (this->spellID == SPL_CONTROL)
	{
		if (this->spellTargetNpc)
		{
			if (this->saveNpc)
			{
				this->saveNpc->Release();

				this->saveNpc = NULL;
			}

			this->saveNpc = this->spellCasterNpc;
			this->saveNpc->refCtr++;

			this->spellCasterNpc->ModifyBodyState(BS_MOD_CONTROLLING, 0);

			this->spellTargetNpc->anictrl->SearchStandAni(0);
			this->spellTargetNpc->SetAsPlayer();
			this->spellTargetNpc->SetBodyStateModifier(BS_MOD_CONTROLLED);

			oCMsgWeapon *msgWeapon = oCMsgWeapon::_CreateNewInstance();
			msgWeapon->subType = 3;

			// Hopefully this works ...
			this->spellCasterNpc->GetEM(FALSE)->OnMessage(msgWeapon, this->spellCasterNpc);

			this->spellTargetNpc->state.StartAIState(zSTRING("ZS_CONTROLLED"), FALSE, 0, 0.0f, FALSE);

			this->spellTargetNpc->InsertActiveSpell(this);
			this->canBeDeleted = FALSE;

			return FALSE;
		}
	}

	return rV;
}

void hSpell::EndTimedEffect()
{
	this->oCSpell::EndTimedEffect();

	if (this->spellID == SPL_CONTROL)
	{
		if (this->saveNpc && this->spellTargetNpc)
		{
			this->saveNpc->SetAsPlayer();
			this->saveNpc->ModifyBodyState(0, BS_MOD_CONTROLLING);

			this->spellTargetNpc->state.EndCurrentState();

			this->saveNpc->state.ClearAIState();
			this->saveNpc->StandUp(FALSE, TRUE);
			this->saveNpc->SetSleeping(FALSE);

			if (this->controlWarnFX)
			{
				this->controlWarnFX->Stop(TRUE);

				if (this->controlWarnFX)
				{
					this->controlWarnFX->Release();

					this->controlWarnFX = NULL;
				}

				this->effect = this->CreateEffect();
				this->effect->Init(this->saveNpc, NULL, NULL);
				this->effect->SetSpellTargetTypes(this->targetCollectType);
				this->effect->Cast(TRUE);

				if (this->effect)
				{
					this->effect->Release();

					this->effect = NULL;
				}
			}
		}
	}
}

void hSpell::DoTimedEffect()
{
	this->oCSpell::DoTimedEffect();

	if (this->spellID == SPL_CONTROL)
	{
		if (this->saveNpc && this->spellTargetNpc)
		{
			if (this->spellTargetNpc->attribute[0] <= 0
				|| this->spellTargetNpc->GetDistanceToVob2(*this->saveNpc) >= 4000.0f * 4000.0f)
			{
				this->canBeDeleted = TRUE;

				this->EndTimedEffect();
			}
			else if (this->spellTargetNpc->GetDistanceToVob2(*this->saveNpc) >= 3500.0f * 3500.0f)
			{
				if (!this->controlWarnFX)
				{
					this->controlWarnFX = oCVisualFX::CreateAndPlay(zSTRING("CONTROL_LEAVERANGEFX"), this->spellTargetNpc->trafoObjToWorld.GetTranslation(), NULL, 1, 0, 0, FALSE);
				}
			}
			else if (this->controlWarnFX)
			{
				this->controlWarnFX->Stop(TRUE);

				if (this->controlWarnFX)
				{
					controlWarnFX->Release();

					controlWarnFX = NULL;
				}
			}
		}
	}
}

int hSpell::IsValidTarget(zCVob *v)
{
	int rV = oCSpell::IsValidTarget(v);

	if (this->spellID == SPL_TELEKINESIS)
	{
		if (!dynamic_cast<oCItem *>(v) && !dynamic_cast<oCMOB *>(v) ||
			(dynamic_cast<oCMOB *>(v) && !static_cast<oCMOB *>(v)->IsMovable())) // do we need to allow MOBs to be moved?
		{
			this->spellStatus = 0;
			return FALSE;
		}
	}
	else if (this->spellID == SPL_TELEKINESIS2) // TODO: Check for oCNpc, what is IsMovable()
	{
		if (!dynamic_cast<oCNpc *>(v))
		{
			this->spellStatus = 0;
			return FALSE;
		}
	}

	return rV;
}

void hSpell::StopTargetEffects(zCVob *vob)
{
	if (this->spellID == SPL_TELEKINESIS || this->spellID == SPL_TELEKINESIS2)
	{
		zCWorld *homeWorld = vob->homeWorld;

		if (vob && vob->homeWorld)
		{
			zVEC3 positionWorld = vob->trafoObjToWorld.GetTranslation();
			zVEC3 groundVec;

			groundVec.n[0] = 0.0f;
			groundVec.n[1] = -(positionWorld.n[1] - vob->bbox3D.mins.n[1]) - 1.0f;
			groundVec.n[2] = 0.0f;

			if (!homeWorld->TraceRayNearestHit(positionWorld, groundVec, vob, 2049))
			{
				zVEC3 velocity;

				velocity.n[0] = 0.0f;
				velocity.n[1] = -100.0f;
				velocity.n[2] = 0.0f;

				vob->SetPhysicsEnabled(TRUE);
				vob->rigidBody->gravityOn = TRUE;
				vob->SetSleeping(FALSE);
				vob->rigidBody->SetVelocity(velocity);
			}
		}
	}
}

ASM(oCSpell_Setup_Hook)
{
	__asm
	{
		mov ecx, [ebp + 0x38]
		push ecx
		mov ecx, ebp
		call hSpell::StopTargetEffects
	}

	RET(0x00484AE6);
}

ASM(oCSpell_Cast_Hook)
{
	__asm
	{
		mov ecx, [esi + 0x38]
		push ecx
		mov ecx, esi
		call hSpell::StopTargetEffects
	}

	RET(0x00485481);
}

ASM(oCSpell_Stop_Hook)
{
	__asm
	{
		mov ecx, [ebp + 0x38]
		push ecx
		mov ecx, ebp
		call hSpell::StopTargetEffects
	}

	RET(0x004856B1);
}

ASM(oCSpell_Kill_Hook)
{
	__asm
	{
		mov ecx, [esi + 0x38]
		push ecx
		mov ecx, esi
		call hSpell::StopTargetEffects
	}

	RET(0x00485841);
}

void PatchSpells(void)
{
	// Does this even work? No way to check currently ...
	InjectHook(0x006665BF, &hNpc::OnDamage_Events); // oCNpc::OnDamage()
	InjectHook(0x004854CF, &hSpell::CastSpecificSpell); // oCSpell::Cast()
	InjectHook(0x0043BEB0, &hSpell::EndTimedEffect); // oCTriggerChangeLevel::TriggerTarget()
	InjectHook(0x0047214D, &hSpell::EndTimedEffect); // oCAIHuman::CheckActiveSpells()
	InjectHook(0x004872BB, &hSpell::EndTimedEffect); // oCSpell::DoTimedEffect()
	InjectHook(0x0073D1F9, &hSpell::DoTimedEffect); // oCNpc::DoActiveSpells()
	InjectHook(0x00484C04, &hSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00484C22, &hSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00484C4D, &hSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00484CCB, &hSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00484E48, &hSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00733FEC, &hSpell::IsValidTarget); // oCNpc::CollectFocusVob()
	InjectHook(0x00473154, &hSpell::IsInvestSpell); // oCAIHuman::MagicMode()
	InjectHook(0x00473303, &hSpell::IsInvestSpell); // oCAIHuman::MagicMode()

	// Injects hook
	oCSpell_Invest_Hook();

	// Hooks for hSpell::StopTargetEffects
	InjectHook(0x00484A77, oCSpell_Setup_Hook, PATCH_JUMP); // oCSpell::Setup()
	InjectHook(0x00485417, oCSpell_Cast_Hook, PATCH_JUMP); // oCSpell::Cast()
	InjectHook(0x00485626, oCSpell_Stop_Hook, PATCH_JUMP); // oCSpell::Stop()
	InjectHook(0x004857D7, oCSpell_Kill_Hook, PATCH_JUMP); // oCSpell::Kill()

	// oCSpell::SetCollisionEnabled() understand and fix switch
}

void hSkyControler_Outdoor::ReadFogColorsFromINI()
{
	zVEC3 defaultCol0;
	zVEC3 defaultCol1;
	zVEC3 defaultCol2;
	zVEC3 defaultCol3;

	defaultCol0.n[0] = 116;
	defaultCol0.n[1] = 89;
	defaultCol0.n[2] = 75;

	defaultCol1.n[0] = 80;
	defaultCol1.n[1] = 90;
	defaultCol1.n[2] = 80;

	defaultCol2.n[0] = 120;
	defaultCol2.n[1] = 140;
	defaultCol2.n[2] = 180;

	defaultCol3.n[0] = 120;
	defaultCol3.n[1] = 140;
	defaultCol3.n[2] = 180;

	this->zCSkyControler_Outdoor::ReadFogColorsFromINI();

	// Maybe actually read this from ini, but then this whole thing would be unnecessary
	this->fogColorDayVariations.array[0] = defaultCol0;
	this->fogColorDayVariations.array[1] = defaultCol1;
	this->fogColorDayVariations.array[2] = defaultCol2;
	this->fogColorDayVariations.array[3] = defaultCol3;
}

ASM(zCSkyControler_Mid_Hook)
{
	__asm { mov dword ptr [esi + 0x50], 0xFF005078 }

	RET(0x005DFC30);
}

static zCRenderLight *pmLight;
static zTRenderContext *renderContext;
static zVEC3 floorLight;

void zCRenderLightContainer_CollectLights_StatLights(void)
{
	zCPolygon *groundPoly = renderContext->vob->groundPoly;

	if (groundPoly)
	{
		if (groundPoly->material->bspSectorFront->sectorName.Contains("WALD"))
		{
			pmLight->colorDiffuse = floorLight * 0.5f;
		}
	}
}

ASM(zCRenderLightContainer_CollectLights_StatLights_Hook)
{
	__asm
	{
		mov [renderContext], edi
		sub esi, 4
		mov [pmLight], esi
		mov esi, [esp + 0x28]
		mov [floorLight + 0x0], esi
		mov esi, [esp + 0x2C]
		mov [floorLight + 0x4], esi
		mov esi, [esp + 0x30]
		mov [floorLight + 0x8], esi
		pushad
		call zCRenderLightContainer_CollectLights_StatLights
		popad
		mov esi, [pmLight]
		add esi, 4
	}

	RET(0x005D5891);
}

const char *Gothic1AppName = "Gothic - 2.6 (fix)";
const char *Gothic1WorldZen = "WORLD.ZEN";
const char *ItMiNugget = "ItMiNugget";
const char *Erz = "Erz: ";
const char *NotEnoughOre = "Nicht genug Erz um den Gegenstand zu kaufen.";
const char *NoSound = "NEWGAME";
const char *SectorName = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

#define zPARTICLE_MAX_GLOBAL 65536

void PatchGothic2(void)
{
	if (G12GetPrivateProfileBool("NoHardcodedThreatMusic", FALSE))
	{
		// No hardcoded threat music in forest
		Patch(0x00696DBD + 1, SectorName); // oCAIHuman::GetEnemyThreat()
	}

	// Some SystemPack fixes not in Gothic 2
	if (G12GetPrivateProfileBool("MoverBugfix", TRUE))
	{
		// MoverBugfix
		Patch(0x00612316, (BYTE)0xEB); // zCMover::InvertMovement()
	}

	if (G12GetPrivateProfileBool("DisableCacheOut", TRUE))
	{
		// DisableCacheOut
		Nop(0x006489B4, 5); // zCRnd_D3D::XD3D_ClearDevice()
	}

	if (G12GetPrivateProfileBool("PfxFix", TRUE))
	{
		// PfxFix
		Patch(0x005AD563, (BYTE)0xEB); // zCParticleFX::zCStaticPfxList::ProcessList()
		Patch(0x005ACB84 + 1, sizeof(zSParticle) * zPARTICLE_MAX_GLOBAL); // zCParticleFX::InitParticleFX()
		Patch(0x005ACBA1 + 1, zPARTICLE_MAX_GLOBAL); // zCParticleFX::InitParticleFX()
		Patch(0x005ACBC9 + 6, zPARTICLE_MAX_GLOBAL); // zCParticleFX::InitParticleFX()
		Patch(0x005ADA71 + 1, zPARTICLE_MAX_GLOBAL); // zCParticleFX::zCStaticPfxList::ProcessList()
		Patch(0x005ADA88 + 1, zPARTICLE_MAX_GLOBAL); // zCParticleFX::zCStaticPfxList::ProcessList()
	}

	if (G12GetPrivateProfileBool("Disable_HUMANS_SWIM.MDS", FALSE))
	{
		// Disable_HUMANS_SWIM.MDS
		Nop(0x00695330, 8); // oCAIHuman::~oCAIHuman()
		Nop(0x0069A43D, 2); // oCAIHuman::PC_SlowMove()
		Nop(0x0069A5EC, 9); // oCAIHuman::PC_SlowMove()
		Nop(0x0069B00D, 9); // oCAIHuman::Moving()
	}

	{
		// Portal distance for Forests
		float woodPortalDistanceMultiplier = G12GetPrivateProfileFloat("WoodPortalDistanceMultiplier", "1");

		float _DIST_MAX2 = 30250000.0f * woodPortalDistanceMultiplier;
		float _DIST_MIN2 = _DIST_MAX2 - 14250000.0f;

		Patch(0x0052F29C + 4, _DIST_MAX2); // zCBspBase::RenderNodeOutdoor()
		Patch(0x0052F2A4 + 4, _DIST_MIN2); // zCBspBase::RenderNodeOutdoor()

		Patch(0x005348DB + 4, _DIST_MAX2); // zCBspSector::ActivateSectorRec()
		Patch(0x005348E3 + 4, _DIST_MIN2); // zCBspSector::ActivateSectorRec()
	}

	// if (G12GetPrivateProfileBool("ThrowFlag", TRUE))
	{
		// Nop(0x0070E9A8, 6); // oCNpcInventory::HandleEvent()
		// ...
	}

	if (G12GetPrivateProfileBool("HideFocus", FALSE))
	{
		// Unlike HideFocus from Systempack which is sometimes buggy and where vobs can still be focused when turning around quickly and spamming ctrl
		// this patches CreateVobList() to the Sequel variant where a dead, empty NPC does not even end up in the focusable voblist
		InjectHook(0x0073369B, &hNpc::CreateVobList); // oCNpc::ToggleFocusVob()
		InjectHook(0x00733BE9, &hNpc::CreateVobList); // oCNpc::CollectFocusVob()
		InjectHook(0x0075DC54, &hNpc::CreateVobList); // oCNpc::PerceiveAll()
		InjectHook(0x0075DE95, &hNpc::CreateVobList); // oCNpc::PerceptionCheck()
	}

	if (G12GetPrivateProfileBool("Gothic1Mode", FALSE))
	{
		// Fix App Title
		Patch(0x0089D9AC, Gothic1AppName); // APP_NAME

		// New game starts WORLD.ZEN
		Patch(0x00429A23 + 1, Gothic1WorldZen); // CGameManager::Menu()
		Patch(0x00429A52 + 1, Gothic1WorldZen); // CGameManager::Menu()

		// Currency
		Patch(0x00704931 + 1, ItMiNugget); // oCItemContainer::GetCurrencyInstanceName()
		Patch(0x0070493C + 1, ItMiNugget); // oCItemContainer::GetCurrencyInstanceName()

		Patch(0x0070DB01 + 1, Erz); // _GetCategorySelfPlayerPrefix()
		Patch(0x0070DB0C + 1, Erz); // _GetCategorySelfPlayerPrefix()

		Patch(0x0068BDC4 + 1, NotEnoughOre); // oCViewDialogTrade::OnTransferRight()
		Patch(0x0068BDEB + 1, NotEnoughOre); // oCViewDialogTrade::OnTransferRight()

		// No GAMESTART menu "music"
		Patch(0x004DB7EE + 1, NoSound); // zCMenu::Enter()
		Patch(0x004DB815 + 1, NoSound); // zCMenu::Enter()

		// Fix progress bar on Loading Screen
		Patch(0x006C282B + 1, 6600); // oCGame::OpenLoadscreen()
		Patch(0x006C2830 + 1, 6192); // oCGame::OpenLoadscreen()
		Patch(0x006C2835 + 1, 6100); // oCGame::OpenLoadscreen()
		Patch(0x006C283A + 1, 2000); // oCGame::OpenLoadscreen()

		// Fix progress bar on Levelchange Screen
		Patch(0x006C29CC + 1, 6600); // oCGame::OpenLoadscreen()
		Patch(0x006C29D1 + 1, 6192); // oCGame::OpenLoadscreen()
		Patch(0x006C29D6 + 1, 6100); // oCGame::OpenLoadscreen()
		Patch(0x006C29DB + 1, 2000); // oCGame::OpenLoadscreen()

		// Fix underwater color and farZ
		InjectHook(0x005DFC16, zCSkyControler_Mid_Hook, PATCH_JUMP); // zCSkyControler_Mid::zCSkyControler_Mid()
		Patch(0x005DFC3F + 3, 2500.0f); // zCSkyControler_Mid::zCSkyControler_Mid()

		// Patch fogcolors to Gothic 1 fogcolors
		InjectHook(0x005E6443, &hSkyControler_Outdoor::ReadFogColorsFromINI); // zCSkyControler_Outdoor::zCSkyControler_Outdoor()

		// No snow in NCI and NCO
		PatchJump(0x00640811, 0x00640861); // oCZoneMusic::ProcessZoneList()

		// Fix dark trees in WALD sectors
		InjectHook(0x005D57DA, zCRenderLightContainer_CollectLights_StatLights_Hook); // zCRenderLightContainer::CollectLights_StatLights()
	}

	if (G12GetPrivateProfileBool("NoGamestartMusic", FALSE))
	{
		// No GAMESTART menu "music"
		Patch(0x004DB7EE + 1, NoSound); // zCMenu::Enter()
		Patch(0x004DB815 + 1, NoSound); // zCMenu::Enter()
	}

	if (G12GetPrivateProfileBool("DisableAutoCalcObstruction", FALSE))
	{
		// Use empty function
		InjectHook(0x004F16C6, &hActiveSnd::AutoCalcObstruction); // zCSndSys_MSS::PlaySound3D()
		InjectHook(0x004F2080, &hActiveSnd::AutoCalcObstruction); // zCSndSys_MSS::PlaySound3DAmbient()
		InjectHook(0x004F259B, &hActiveSnd::AutoCalcObstruction); // zCSndSys_MSS::UpdateSound3D()
		InjectHook(0x004F3275, &hActiveSnd::AutoCalcObstruction); // zCSndSys_MSS::UpdateSoundPropsAmbient()
	}

	if (G12GetPrivateProfileBool("FixGetDistance", TRUE))
	{
		Patch(0x0047405E + 2, 0x008CDF94 + sizeof(zVEC2)); // oCMagFrontier::GetDistanceNewWorld()
		Patch(0x00474568 + 2, 0x008CDEC0 + sizeof(zVEC2)); // oCMagFrontier::GetDistanceDragonIsland()
		Patch(0x00474728 + 2, 0x008CDE6C + sizeof(zVEC2)); // oCMagFrontier::GetDistanceAddonWorld()
	}

	if (G12GetPrivateProfileBool("G1Spells", FALSE))
	{
		// An attempt at reintroducing the hardcoded spells from Gothic 1 (Telekinesis, Control)
		PatchSpells();
	}
}

void PatchSpacer2(void)
{
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
	if (GOTHIC26)
	{
		G12AllocConsole();
		PatchGothic2();
	}
	else if (SPACER26)
	{
		G12AllocConsole();
		PatchSpacer2();
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
