#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <stdint.h>
#include "..\G12Dll\MemoryMgr.h"

#define G12DLL_NAME "G2Spells"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G2.hpp"
#include "..\G12Dll\G2.h"
#include "g2spells.hpp"

hCFXScanner::hCFXScanner()
{
	enabled = FALSE;
	initVob = NULL;
	alpha = 255;
	scannerVobs[0] = NULL;
	scannerVobs[1] = NULL;
	scannerVobs[2] = NULL;
}

hCFXScanner::~hCFXScanner()
{
	for (int i = 0; i < 3; i++)
	{
		if (scannerVobs[i])
		{
			scannerVobs[i]->RemoveVobFromWorld();
			scannerVobs[i]->Release();
		}
	}
}

void hCVisFX_Lightning::InitValues()
{
	dScriptEnd = TRUE;

	burnVobs = new zCArray<zCVob *>();
	burnNodes = new zCArray<zCModelNodeInst *>();
	decalVobs = new zCArray<zCVob *>();
	electricFX = new zCArray<oCVisualFX *>();

	scanner = new hCFXScanner();

	targetNode = NULL;
	phase = 0;
	showScanner = FALSE;
	investedNext = FALSE;
	castOnSelf = FALSE;
	lightRange = 1000.0f;

	dontWriteIntoArchive = TRUE;
}

void hCVisFX_Lightning::DeinitValues(zCVob *orgVob, bool recalcTrj)
{
	if (this->dScriptEnd)
	{
		oCNpc *originNpc = NULL;

		if (zCObject::CheckInheritance(&oCNpc::classDef, this->origin->_GetClassDef()))
		{
			originNpc = (oCNpc *)origin;
		}

		for (int i = 0; i < burnVobs->numInArray; i++)
		{
			oCNpc *burnNpc = NULL;

			if (zCObject::CheckInheritance(&oCNpc::classDef, burnVobs->array[i]->_GetClassDef()))
			{
				burnNpc = (oCNpc *)burnVobs->array[i];

				if (originNpc && burnNpc) burnNpc->AssessStopMagic_S(originNpc, spellType);
			}
		}

		for (int i = 0; i < decalVobs->numInArray; i++)
		{
			decalVobs->array[i]->RemoveVobFromWorld();
			decalVobs->array[i]->Release();
		}

		for (int i = 0; i < electricFX->numInArray; i++)
		{
			electricFX->array[i]->Stop(TRUE);
			if (electricFX->array[i]) electricFX->array[i]->Release();
		}

		delete burnVobs;
		delete burnNodes;
		delete decalVobs;
		delete electricFX;

		delete scanner;
	}

	this->oCVisualFX::SetOrigin(orgVob, recalcTrj);
}

hCVisFX_Lightning *hCVisFX_Lightning::_CreateNewInstance()
{
	// This whole shit dont work
	// Idea: keep existing vftable, hook functions that need to be overriden
	// Call original function if not lightning, do own stuff if lightning
	// Use new isLightning or something to figure out the difference
	
	Patch(0x0049A247 + 1, sizeof(hCVisFX_Lightning));
	hCVisFX_Lightning *vfxLightning = (hCVisFX_Lightning *)oCVisualFX::_CreateNewInstance();
	Patch(0x0049A247 + 1, sizeof(oCVisualFX));

	vfxLightning->InitValues();

	return vfxLightning;
}

void hCVisualFX::InitValues()
{
	this->oCVisualFX::InitValues();

	this->dScriptEnd = FALSE;
}

void hCNpc::OnDamage_Events(oSDamageDescriptor & descDamage)
{
	this->oCNpc::OnDamage_Events(descDamage);

	hCSpell *spell = NULL;
	oCNpc *npcTarget = NULL;
	oCNpc *npcAttacker = descDamage.pNpcAttacker;

	if (npcAttacker)
	{
		spell = (hCSpell *)npcAttacker->IsSpellActive(SPL_CONTROL);
		npcTarget = spell ? spell->spellTargetNpc : NULL;
	}

	if (spell && npcTarget && this->attribute[0] <= 0)
	{
		spell->EndTimedEffect();
	}

	if (this->HasBodyStateModifier(BS_MOD_CONTROLLING))
	{
		spell = (hCSpell *)npcAttacker->IsSpellActive(SPL_CONTROL);

		if (spell)
		{
			spell->EndTimedEffect();
		}
	}
}

void hCSpell::DoLogicInvestEffect()
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
			move -= right;
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

bool hCSpell::Invest()
{
	if (!this->effect) return FALSE;

	if (!this->spellCasterNpc || this->spellCasterNpc->attribute[this->spellEnergy] <= 0)
	{
		if (this->manaInvested > 0)
		{
			this->SetReleaseStatus();

			return TRUE;
		}

		return FALSE;
	}

	this->DoLogicInvestEffect();
	this->spellCasterNpc->CreatePassivePerception(NPC_PERC_ASSESSCASTER, NULL, NULL);

	this->manaTimer += this->manaInvested ? ztimer.frameTimeFloat : this->manaInvestTime;

	if (this->manaTimer < this->manaInvestTime) return FALSE;

	this->manaTimer -= this->manaInvestTime;

	if (this->spellStatus != SPL_STATUS_CANINVEST || !this->spellCasterNpc) return FALSE;

	this->CallScriptInvestedMana();

	if (this->spellStatus == SPL_STATUS_CANINVEST || this->spellStatus == SPL_STATUS_CAST || this->spellStatus == SPL_STATUS_NEXTLEVEL)
	{
		if (this->manaInvested) this->spellCasterNpc->ChangeAttribute(this->spellEnergy, -1);

		this->manaInvested++;
	}

	if (this->spellStatus == SPL_STATUS_CAST) return TRUE;
	if (this->spellStatus == SPL_STATUS_STOP) return TRUE;

	if (this->spellStatus == SPL_STATUS_NEXTLEVEL)
	{
		this->spellStatus = SPL_STATUS_CANINVEST;
		this->spellLevel++;

		if (effect) effect->InvestNext();

		return TRUE;
	}

	return TRUE;

	// TODO this doesn't work - find out why
#if 0
	int manaLeft = 0;
	if (this->spellCasterNpc) manaLeft = this->spellCasterNpc->attribute[this->spellEnergy];

	if (manaLeft > 0)
	{
		this->DoLogicInvestEffect();
	}
	else
	{
		if (this->manaInvested > 0)
		{
			this->SetReleaseStatus();

			return TRUE;
		}

		return FALSE;
	}

	this->spellCasterNpc->CreatePassivePerception(NPC_PERC_ASSESSCASTER, NULL, NULL);

	if (this->manaInvested == 0)
	{
		this->manaTimer += this->manaInvestTime;
	}
	else
	{
		this->manaTimer += ztimer.frameTimeFloat;
	}

	if (this->manaTimer >= this->manaInvestTime)
	{
		this->manaTimer -= this->manaInvestTime;

		// if (this->spellStatus == SPL_STATUS_CANINVEST || this->spellStatus == SPL_STATUS_CANINVEST_NO_MANADEC)
		{
			if (this->spellCasterNpc && manaLeft > 0)
			{
				this->CallScriptInvestedMana();

				if (this->spellStatus == SPL_STATUS_CAST || this->spellStatus == SPL_STATUS_CANINVEST || this->spellStatus == SPL_STATUS_CANINVEST_NO_MANADEC || this->spellStatus == SPL_STATUS_NEXTLEVEL)
				{
					if (this->manaInvested)
					{
						this->spellCasterNpc->ChangeAttribute(this->spellEnergy, -1);
						printf("SPL_STATUS_CANINVEST: Changing mana ...\n");
					}

					this->manaInvested++;
				}
				else if (this->spellStatus & SPL_STATUS_FORCEINVEST)
				{
					this->manaInvested = this->spellStatus - SPL_STATUS_FORCEINVEST;
					this->spellStatus = SPL_STATUS_CANINVEST;

					this->spellCasterNpc->ChangeAttribute(this->spellEnergy, -this->manaInvested);
					printf("SPL_STATUS_FORCEINVEST: Changing mana ...\n");
				}

				if (this->spellStatus == SPL_STATUS_CAST)
				{
					return TRUE;
				}

				if (this->spellStatus == SPL_STATUS_STOP)
				{
					return TRUE;
				}

				if (this->spellStatus == SPL_STATUS_NEXTLEVEL)
				{
					this->spellStatus = SPL_STATUS_CANINVEST;
					this->spellLevel++;

					if (this->effect) this->effect->InvestNext();
				}

				return TRUE;
			}
		}
	}

	return FALSE;
#endif
}

bool hCSpell::IsInvestSpell()
{
	switch (this->spellID)
	{
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

bool hCSpell::CastSpecificSpell()
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

void hCSpell::EndTimedEffect()
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

void hCSpell::DoTimedEffect()
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

bool hCSpell::IsValidTarget(zCVob *v)
{
	bool rV = oCSpell::IsValidTarget(v);

	if (rV)
	{
		zCClassDef *classDef = v->_GetClassDef();

		if (this->spellID == SPL_TELEKINESIS)
		{
			if (!zCObject::CheckInheritance(&oCItem::classDef, classDef) && !zCObject::CheckInheritance(&oCMOB::classDef, classDef) ||
				(zCObject::CheckInheritance(&oCMOB::classDef, classDef) && !((oCMOB *)v)->IsMovable())) // do we need to allow MOBs to be moved?
			{
				this->spellStatus = SPL_STATUS_DONTINVEST;
				return FALSE;
			}
		}
		if (this->spellID == SPL_TELEKINESIS2) // TODO: Check for oCNpc, what is IsMovable()
		{
			if (!zCObject::CheckInheritance(&oCNpc::classDef, classDef))
			{
				this->spellStatus = SPL_STATUS_DONTINVEST;
				return FALSE;
			}
		}
	}

	return rV;
}

void hCSpell::StopTargetEffects(zCVob *vob)
{
	if (this->spellID == SPL_TELEKINESIS || this->spellID == SPL_TELEKINESIS2)
	{
		if (vob && vob->homeWorld)
		{
			zVEC3 positionWorld = vob->trafoObjToWorld.GetTranslation();
			zVEC3 groundVec;

			groundVec.n[0] = 0.0f;
			groundVec.n[1] = -(positionWorld.n[1] - vob->bbox3D.mins.n[1]) - 1.0f;
			groundVec.n[2] = 0.0f;

			if (!vob->homeWorld->TraceRayNearestHit(positionWorld, groundVec, vob, 2049))
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
		call hCSpell::StopTargetEffects
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
		call hCSpell::StopTargetEffects
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
		call hCSpell::StopTargetEffects
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
		call hCSpell::StopTargetEffects
	}

	RET(0x00485841);
}

void hCVisualFX::SetCollisionEnabled(bool en)
{
	if (this->collDetectionDynamic || this->collDetectionStatic)
	{
		this->emCheckCollision = TRUE;

		return;
	}

	this->emCheckCollision = en;

	this->collDetectionStatic = en && this->emActionCollDyn != TACTION_COLL_NONE;
	this->collDetectionDynamic = en && this->emActionCollDyn != TACTION_COLL_NONE;

	if (!this->visual) return;
	if (!this->emCheckCollision) return;

	if (this->bIsProjectile)
	{
		this->SetCollisionClass(&zCCollObjectProjectile::s_oCollObjClass);

		return;
	}

	switch (this->spellType)
	{
	case SPL_FIREBALL:
	case SPL_WINDFIST:
	case SPL_DESTROYUNDEAD:
	case SPL_FIREBOLT:
	case SPL_THUNDERBOLT:
	case SPL_THUNDERBALL:
	case SPL_ICECUBE:
	case SPL_BREATHOFDEATH:
	case SPL_NEW1:
		this->SetCollisionClass(&zCCollObjectProjectile::s_oCollObjClass);

		break;
	case SPL_FIRESTORM:
		if (this->visName_S.Contains("_SPREAD")) this->SetCollisionClass(&zCCollObjectBoxPassThrough::s_oCollObjClass);
		else this->SetCollisionClass(&zCCollObjectProjectile::s_oCollObjClass);

		break;
	default:
		this->SetCollisionClass(&zCCollObjectBoxPassThrough::s_oCollObjClass);

		break;
	}
}

oCVisualFX *hCSpell::CreateEffect()
{
	// return this->oCSpell::CreateEffect();

	if (this->spellID == SPL_CHAINLIGHTNING) this->effect = hCVisFX_Lightning::_CreateNewInstance();
	else if (this->isMultiEffect) this->effect = oCVisFX_MultiTarget::_CreateNewInstance();
	else this->effect = oCVisualFX::_CreateNewInstance();

	this->effect->SetSpellTargetTypes(this->targetCollectType);
	this->effect->SetDamage((float)this->damagePerLevel);
	this->effect->SetDamageType(this->damageType);
	this->effect->SetSpellType(this->spellID);
	this->effect->SetSpellCat(this->spellType);
	// this->effect->SetByScript("SPELLFX_" + this->GetSpellInstanceName(this->spellID));
	this->effect->SetByScript("SPELLFX_ChainLightning");

	return this->effect;
}

ASM(oCSpell_InitByScript_Hook)
{
	__asm
	{
		push edi
		call hCSpell::CreateEffect
		pop edi
	}

	RET(0x0048472E);
}

ASM(oCSpell_EndTimedEffect_Hook)
{
	__asm
	{
		push esi
		call hCSpell::CreateEffect
		pop esi
	}

	RET(0x00487114);
}

void PatchSpells(void)
{
	// Does this even work? No way to check currently ...
	InjectHook(0x006665BF, &hCNpc::OnDamage_Events); // oCNpc::OnDamage()
	InjectHook(0x004854CF, &hCSpell::CastSpecificSpell); // oCSpell::Cast()
	InjectHook(0x0043BEB0, &hCSpell::EndTimedEffect); // oCTriggerChangeLevel::TriggerTarget()
	InjectHook(0x0047214D, &hCSpell::EndTimedEffect); // oCAIHuman::CheckActiveSpells()
	InjectHook(0x004872BB, &hCSpell::EndTimedEffect); // oCSpell::DoTimedEffect()
	InjectHook(0x0073D1F9, &hCSpell::DoTimedEffect); // oCNpc::DoActiveSpells()
	InjectHook(0x00484C04, &hCSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00484C22, &hCSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00484C4D, &hCSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00484CCB, &hCSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00484E48, &hCSpell::IsValidTarget); // oCSpell::Setup()
	InjectHook(0x00733FEC, &hCSpell::IsValidTarget); // oCNpc::CollectFocusVob()
	InjectHook(0x00473154, &hCSpell::IsInvestSpell); // oCAIHuman::MagicMode()
	InjectHook(0x00473303, &hCSpell::IsInvestSpell); // oCAIHuman::MagicMode()
	InjectHook(0x0047668E, &hCSpell::Invest); // oCMag_Book::Spell_Invest()

	// Hooks for hSpell::StopTargetEffects
	InjectHook(0x00484A77, oCSpell_Setup_Hook, PATCH_JUMP); // oCSpell::Setup()
	InjectHook(0x00485417, oCSpell_Cast_Hook, PATCH_JUMP); // oCSpell::Cast()
	InjectHook(0x00485626, oCSpell_Stop_Hook, PATCH_JUMP); // oCSpell::Stop()
	InjectHook(0x004857D7, oCSpell_Kill_Hook, PATCH_JUMP); // oCSpell::Kill()

	// oCVisualFX::SetCollisionEnabled()
	Patch(0x00830374, &hCVisualFX::_SetCollisionEnabled); // oCVisualFX::`vftable'
	Patch(0x0083060C, &hCVisualFX::_SetCollisionEnabled); // oCVisFX_MultiTarget::`vftable'

	// oCSpell::CreateEffect()
	// For some reason the game compiler did something really weird here and just kinda expects certain registers are intact
	// Modern VS creates the function differently, so we have to push and pop the register before and after the function call via asm
	InjectHook(0x00484729, oCSpell_InitByScript_Hook, PATCH_JUMP); // oCSpell::InitByScript()
	InjectHook(0x0048710F, oCSpell_EndTimedEffect_Hook, PATCH_JUMP); // oCSpell::EndTimedEffect()

	// Init dScriptEnd to FALSE
	InjectHook(0x00489EFD, &hCVisualFX::InitValues); // oCVisualFX::oCVisualFX()

	// Destroy lightning stuff if lightning
	InjectHook(0x0048A704, &hCVisFX_Lightning::DeinitValues); // oCVisualFX::~oCVisualFX()

	// Remove unneeded (?) collsion enabling in oCVisualFX::InitEffect()
	PatchJump(0x00494B56, 0x00494BAC); // oCVisualFX::InitEffect()
}

void PatchGothic2(void)
{
	if (G12GetPrivateProfileBool("G1Spells", FALSE))
	{
		// An attempt at reintroducing the hardcoded spells from Gothic 1 (Telekinesis, Control)
		// TODO SPL_TRANSFORM transforms into copy of NPC in focus ... see oCSpell::CastSpecificSpell()
		// TODO oCSpell::CreateEffect() ChainLightning
		// TODO Telekinesis scroll doesn't get taken
		PatchSpells();
	}
}

void Init(void)
{
	if (GOTHIC26)
	{
		G12AllocConsole();
		PatchGothic2();
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
