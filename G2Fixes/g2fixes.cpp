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

	bool delete_vob;

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

				if (npc->IsDead() && npc->inventory2.IsEmpty(TRUE, TRUE))
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

// SPL_* should probably be read from INI, just using hardcoded values for now - same as the game does it :)
#define SPL_TELEPORT5 13 // Sumpflager
#define SPL_TELEPORT1 14 // Feuermagier
#define SPL_TELEPORT3 15 // Dämonenbeschwörer
#define SPL_TELEPORT2 16 // Wassermagier
#define SPL_TELEPORT4 17 // Orkisch
#define SPL_LIGHT 18
#define SPL_FIREBOLT 19
#define SPL_THUNDERBOLT 20
#define SPL_CHAINLIGHTNING 24
#define SPL_WINDFIST 26
#define SPL_SLEEP 27
#define SPL_FIREBALL 30
#define SPL_SUMMONSKELETON 31
#define SPL_FEAR 32
#define SPL_ICECUBE 33
#define SPL_THUNDERBALL 34
#define SPL_SUMMONGOLEM 35
#define SPL_DESTROYUNDEAD 36
#define SPL_PYROKINESIS 37
#define SPL_ICEWAVE 39
#define SPL_SUMMONDEMON 40
#define SPL_FIRERAIN 42
#define SPL_BREATHOFDEATH 43
#define SPL_MASSDEATH 44
#define SPL_ARMYOFDARKNESS 45
#define SPL_SHRINK 46
#define SPL_TRF_MEATBUG 47
#define SPL_TRF_SCAVENGER 48
#define SPL_TRF_MOLERAT 49
#define SPL_TRF_CRAWLER 50
#define SPL_TRF_WOLF 51
#define SPL_TRF_WARAN 52
#define SPL_TRF_SNAPPER 53
#define SPL_TRF_ORCDOG 54
#define SPL_TRF_BLOODFLY 55
#define SPL_TRF_LURKER 56
#define SPL_TRF_SHADOWBEAST 57
#define SPL_CHARM 59
#define SPL_NEW1 60
#define SPL_CONTROL 64
#define SPL_TELEKINESIS 65
#define SPL_TELEKINESIS2 66
#define SPL_BERZERK 67
#define SPL_HEAL 68
#define SPL_FIRESTORM  69
#define SPL_STORMFIST 72

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

bool hSpell::Invest()
{
	if (!this->effect) return FALSE;

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

	if (!this->manaInvested)
	{
		this->spellCasterNpc->CreatePassivePerception(NPC_PERC_ASSESSCASTER, NULL, NULL);

		this->manaTimer += this->manaInvestTime;
	}
	else
	{
		this->manaTimer += ztimer.frameTimeFloat;
	}

	if (this->manaTimer >= this->manaInvestTime)
	{
		this->manaTimer -= this->manaInvestTime;

		if (this->spellStatus == SPL_STATUS_CANINVEST || this->spellStatus == SPL_STATUS_CANINVEST_NO_MANADEC)
		{
			if (this->spellCasterNpc && manaLeft > 0)
			{
				this->CallScriptInvestedMana();

				if (this->spellStatus == SPL_STATUS_CAST || this->spellStatus == SPL_STATUS_CANINVEST || this->spellStatus == SPL_STATUS_CANINVEST_NO_MANADEC || this->spellStatus == SPL_STATUS_NEXTLEVEL)
				{
					if (this->manaInvested && this->spellStatus == SPL_STATUS_CANINVEST)
					{
						this->spellCasterNpc->attribute[this->spellEnergy]--;
					}

					this->manaInvested++;
				}
				else if (this->spellStatus & SPL_STATUS_FORCEINVEST)
				{
					this->manaInvested = this->spellStatus - SPL_STATUS_FORCEINVEST;
					this->spellStatus = SPL_STATUS_CANINVEST;

					this->spellCasterNpc->attribute[this->spellEnergy] -= this->manaInvested;
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
}

bool hSpell::IsInvestSpell()
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

bool hSpell::CastSpecificSpell()
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

bool hSpell::IsValidTarget(zCVob *v)
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

void hSpell::StopTargetEffects(zCVob *vob)
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

static int DAM_CRITICAL_MULTIPLIER = -1;
static int NPC_MINIMAL_DAMAGE = -1;

void hNpc::OnDamage_Hit(oSDamageDescriptor &descDamage)
{
	// this->oCNpc::OnDamage_Hit(descDamage);

	// return;

	if (DAM_CRITICAL_MULTIPLIER == -1)
	{
		zCPar_Symbol *pSymbol = parser.GetSymbol("DAM_CRITICAL_MULTIPLIER");
		if (pSymbol) pSymbol->GetValue(DAM_CRITICAL_MULTIPLIER, 0);
		else DAM_CRITICAL_MULTIPLIER = 2; // default value
	}

	if (NPC_MINIMAL_DAMAGE == -1)
	{
		zCPar_Symbol *pSymbol = parser.GetSymbol("NPC_MINIMAL_DAMAGE");
		if (pSymbol) pSymbol->GetValue(NPC_MINIMAL_DAMAGE, 0);
		else NPC_MINIMAL_DAMAGE = 10; // default value
	}

	printf("descDamage:\n");
	printf("aryDamage Edge: %d\n", descDamage.aryDamage[oEDamageIndex_Edge]);
	printf("fDamageTotal: %f\n", descDamage.fDamageTotal);
	printf("fDamageMultiplier: %f\n", descDamage.fDamageMultiplier);
	printf("\n");

	oCNpc *pNpcAttacker = descDamage.pNpcAttacker;

	if (pNpcAttacker && !pNpcAttacker->IsMonster())
	{
		int talentNr = NPC_TAL_INVALID;

		if (pNpcAttacker->GetWeaponMode() == NPC_WEAPON_1HS) talentNr = NPC_TAL_1H;
		else if (pNpcAttacker->GetWeaponMode() == NPC_WEAPON_2HS) talentNr = NPC_TAL_2H;
		else if (pNpcAttacker->GetWeaponMode() == NPC_WEAPON_BOW) talentNr = NPC_TAL_BOW;
		else if (pNpcAttacker->GetWeaponMode() == NPC_WEAPON_CBOW) talentNr = NPC_TAL_CROSSBOW;

		if (talentNr != NPC_TAL_INVALID)
		{
			printf("talentNr != NPC_TAL_INVALID:\n");

			int nChance = pNpcAttacker->GetTalentValue(talentNr);
			printf("nChance: %d\n", nChance);

			int nPercentage = zRand(100) + 1;
			printf("nPercentage: %d\n", nPercentage);

			if (nPercentage <= nChance)
			{
				descDamage.fDamageMultiplier *= DAM_CRITICAL_MULTIPLIER;
			}

			printf("fDamageMultiplier: %f\n", descDamage.fDamageMultiplier);
			printf("\n");
		}
	}

	if (descDamage.fDamageMultiplier != 1.0f)
	{
		for (int nIndex = 0; nIndex < oEDamageIndex_MAX; nIndex++)
		{
			descDamage.aryDamage[nIndex] *= (unsigned int)descDamage.fDamageMultiplier;
		}

		descDamage.fDamageTotal *= descDamage.fDamageMultiplier;
	}

	printf("descDamage.fDamageMultiplier != 1.0f\n");
	printf("aryDamage Edge: %d\n", descDamage.aryDamage[oEDamageIndex_Edge]);
	printf("fDamageTotal: %f\n", descDamage.fDamageTotal);
	printf("fDamageMultiplier: %f\n", descDamage.fDamageMultiplier);
	printf("\n");

	int nDamageTotal = 0;

	for (int nIndex = 0; nIndex < oEDamageIndex_MAX; nIndex++)
	{
		if (this->HasFlag(descDamage.enuModeDamage, 1 << nIndex))
		{
			nDamageTotal += descDamage.aryDamage[nIndex];
		}
	}

	printf("for (int nIndex = 0; nIndex < oEDamageIndex_MAX; nIndex++)\n");
	printf("aryDamage Edge: %d\n", descDamage.aryDamage[oEDamageIndex_Edge]);
	printf("fDamageTotal: %f\n", descDamage.fDamageTotal);
	printf("fDamageMultiplier: %f\n", descDamage.fDamageMultiplier);
	printf("nDamageTotal: %d, %d\n", nDamageTotal, !nDamageTotal);
	printf("\n");

	bool bDivideTotalDamage = !nDamageTotal;
	bool bIsSemiHuman = pNpcAttacker && pNpcAttacker->IsSemiHuman();

	if (bDivideTotalDamage)
	{
		if (pNpcAttacker && !bIsSemiHuman)
		{
			if (!descDamage.pFXHit)
			{
				descDamage.fDamageTotal = (float)pNpcAttacker->attribute[NPC_ATR_STRENGTH];
			}
		}

		int nDamageTotal = (int)descDamage.fDamageTotal;
		ApplyDamages(descDamage.enuModeDamage, (int *)descDamage.aryDamage, nDamageTotal);
		descDamage.fDamageTotal = (float)nDamageTotal;
	}

	printf("bDivideTotalDamage\n");
	printf("aryDamage Edge: %d\n", descDamage.aryDamage[oEDamageIndex_Edge]);
	printf("fDamageTotal: %f\n", descDamage.fDamageTotal);
	printf("fDamageMultiplier: %f\n", descDamage.fDamageMultiplier);
	printf("\n");

	if (pNpcAttacker && bIsSemiHuman)
	{
		for (int nIndex = 0; nIndex < oEDamageIndex_MAX; nIndex++)
		{
			descDamage.aryDamage[nIndex] += pNpcAttacker->damage[nIndex];
		}
	}

	printf("pNpcAttacker && bIsSemiHuman\n");
	printf("aryDamage Edge: %d\n", descDamage.aryDamage[oEDamageIndex_Edge]);
	printf("fDamageTotal: %f\n", descDamage.fDamageTotal);
	printf("fDamageMultiplier: %f\n", descDamage.fDamageMultiplier);
	printf("\n");

	if (pNpcAttacker)
	{
		if (!descDamage.pFXHit && bIsSemiHuman)
		{
			bool bBlunt = ((descDamage.enuModeDamage & oEDamageType_Blunt) == oEDamageType_Blunt);
			bool bEdge = ((descDamage.enuModeDamage & oEDamageType_Edge) == oEDamageType_Edge);
			bool bPoint = ((descDamage.enuModeDamage & oEDamageType_Point) == oEDamageType_Point);

			if (!(bBlunt || bEdge || bPoint) && pNpcAttacker->IsMonster())
			{
				descDamage.enuModeDamage |= oEDamageType_Edge;
				bEdge = TRUE;
			}

			float fDivisor = 0.0f;
			float fStrength = (float)pNpcAttacker->attribute[NPC_ATR_STRENGTH];

			if (bBlunt) fDivisor = 1.0f;
			if (bEdge) fDivisor += 1.0f;

			if (fDivisor != 0.0f)
			{
				fStrength /= fDivisor;

				if (bBlunt) descDamage.aryDamage[oEDamageIndex_Blunt] += (unsigned int)fStrength;
				if (bEdge) descDamage.aryDamage[oEDamageIndex_Edge] += (unsigned int)fStrength;
			}
		}
	}

	printf("bBlunt || bEdge || bPoint\n");
	printf("aryDamage Edge: %d\n", descDamage.aryDamage[oEDamageIndex_Edge]);
	printf("fDamageTotal: %f\n", descDamage.fDamageTotal);
	printf("fDamageMultiplier: %f\n", descDamage.fDamageMultiplier);
	printf("\n");

	int nDamageEffective = 0;
	int nDamageApplied = 0;
	int nDamageCurrent = 0;
	int nProtectionTotal = 0;
	int nProtectionCurrent = 0;
	bool immortalByProtection = FALSE;
	bool canBeImmortalByProtection = TRUE;

	for (int nDamageIndex = 0; nDamageIndex < oEDamageIndex_MAX; nDamageIndex++)
	{
		if ((descDamage.enuModeDamage & (1 << nDamageIndex)) == (1 << nDamageIndex))
		{
			nDamageCurrent = descDamage.aryDamage[nDamageIndex];
			nProtectionCurrent = this->protection[nDamageIndex];

			if (nProtectionCurrent > 0)	canBeImmortalByProtection = FALSE;
			if (canBeImmortalByProtection && nProtectionCurrent < 0) immortalByProtection = TRUE;

			nProtectionTotal += nProtectionCurrent;
			nDamageApplied = (nDamageCurrent - nProtectionCurrent);
			if (nDamageApplied < 0) nDamageApplied = 0;
			nDamageEffective += nDamageApplied;
			descDamage.aryDamageEffective[nDamageIndex] = nDamageApplied;
		}
		else
		{
			descDamage.aryDamageEffective[nDamageIndex] = 0;
		}
	}

	printf("int nDamageIndex = 0; nDamageIndex < oEDamageIndex_MAX; nDamageIndex++\n");
	printf("aryDamage Edge: %d\n", descDamage.aryDamage[oEDamageIndex_Edge]);
	printf("aryDamageEffective Edge: %d\n", descDamage.aryDamageEffective[oEDamageIndex_Edge]);
	printf("fDamageTotal: %f\n", descDamage.fDamageTotal);
	printf("fDamageMultiplier: %f\n", descDamage.fDamageMultiplier);
	printf("\n");

	descDamage.fDamageTotal = (float)nDamageEffective;

	if (this->HasFlag(NPC_FLAG_IMMORTAL) || immortalByProtection) descDamage.fDamageTotal = 0.0f;

	descDamage.fDamageEffective = descDamage.fDamageTotal;

	if (descDamage.fDamageEffective < 0.0f) descDamage.fDamageEffective = 0.0f;

	descDamage.fDamageReal = descDamage.fDamageEffective;
	int nDamage = (int)descDamage.fDamageEffective;

	printf("NPC_FLAG_IMMORTAL\n");
	printf("this->HasFlag(NPC_FLAG_IMMORTAL) || immortalByProtection: %d\n", this->HasFlag(NPC_FLAG_IMMORTAL) || immortalByProtection);
	printf("aryDamage Edge: %d\n", descDamage.aryDamage[oEDamageIndex_Edge]);
	printf("aryDamageEffective Edge: %d\n", descDamage.aryDamageEffective[oEDamageIndex_Edge]);
	printf("fDamageTotal: %f\n", descDamage.fDamageTotal);
	printf("fDamageMultiplier: %f\n", descDamage.fDamageMultiplier);
	printf("\n");

	// zERR_FAULT("X: Dealing " + zSTRING(nDamage) + " damage!");

	bool bBarrier = this->HasFlag(descDamage.enuModeDamage, oEDamageType_Barrier);
	bool bDeathByBarrier = FALSE;
	const bool bForceBarrierDeath = TRUE; // for some extra spicy barrier behaviour

	if (bBarrier)
	{
		if (this->anictrl)
		{
			bool bInWater = this->anictrl->GetWaterLevel() > 1;
			bDeathByBarrier = bInWater;
		}

		if (bDeathByBarrier || bForceBarrierDeath) nDamage = this->attribute[NPC_ATR_HITPOINTS];
	}

	if (this->IsSelfPlayer() && nDamage <= 0)
	{
		nDamage = NPC_MINIMAL_DAMAGE;
	}

	if (!this->HasFlag(NPC_FLAG_IMMORTAL) && !immortalByProtection)
	{
		this->ChangeAttribute(NPC_ATR_HITPOINTS, -nDamage);
	}

	this->hpHeal = this->attribute[NPC_ATR_REGENERATEHP] * 1000.0f;
	this->manaHeal = this->attribute[NPC_ATR_REGENERATEMANA] * 1000.0f;

	printf("\n");
}

void hVisualFX::SetCollisionEnabled(bool en)
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
	InjectHook(0x0047668E, &hSpell::Invest); // oCMag_Book::Spell_Invest()

	// Hooks for hSpell::StopTargetEffects
	InjectHook(0x00484A77, oCSpell_Setup_Hook, PATCH_JUMP); // oCSpell::Setup()
	InjectHook(0x00485417, oCSpell_Cast_Hook, PATCH_JUMP); // oCSpell::Cast()
	InjectHook(0x00485626, oCSpell_Stop_Hook, PATCH_JUMP); // oCSpell::Stop()
	InjectHook(0x004857D7, oCSpell_Kill_Hook, PATCH_JUMP); // oCSpell::Kill()

	// oCVisualFX::SetCollisionEnabled()
	Patch(0x00830374, &hVisualFX::SetCollisionEnabled); // oCVisualFX::`vftable'
	Patch(0x0083060C, &hVisualFX::SetCollisionEnabled); // oCVisFX_MultiTarget::`vftable'

	// Remove unneeded (?) collsion enabling in oCVisualFX::InitEffect()
	PatchJump(0x00494B56, 0x00494BAC); // oCVisualFX::InitEffect()
}

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
		// very hacky - there are other lighting differences, too (see Old Mine Entrance)
		// perhaps it's better to understand how the lighting differs from G1 to G2 ...
		InjectHook(0x005D57DA, zCRenderLightContainer_CollectLights_StatLights_Hook); // zCRenderLightContainer::CollectLights_StatLights()

		// Damage calculation
		InjectHook(0x00666513, &hNpc::OnDamage_Hit); // oCNpc::OnDamage()

		// An attempt at reintroducing the hardcoded spells from Gothic 1 (Telekinesis, Control)
		PatchSpells();
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
