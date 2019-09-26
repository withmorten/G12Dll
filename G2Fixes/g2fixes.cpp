#include "..\G12Dll\G12Core.h"

#define G12DLL_NAME "G2Fixes"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G2.hpp"
#include "..\G12Dll\G2.h"
#include "g2fixes.hpp"

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
				vob->AddRef();
			}
		}
	}
}

bool hCNpc::IsSkeleton()
{
	return this->guildTrue == NPC_GIL_SKELETON || this->guildTrue == NPC_GIL_SUMMONED_SKELETON || this->guildTrue == NPC_GIL_SKELETON_MAGE;
}

static int DAM_CRITICAL_MULTIPLIER = -1;
static int NPC_MINIMAL_DAMAGE = -1;

void hCNpc::OnDamage_Hit(oSDamageDescriptor &descDamage)
{
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
		else NPC_MINIMAL_DAMAGE = 1; // default value
	}

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
			int nChance = pNpcAttacker->GetTalentValue(talentNr);

			int nPercentage = zRand(100) + 1;

			if (nPercentage <= nChance)
			{
				descDamage.fDamageMultiplier *= DAM_CRITICAL_MULTIPLIER;
			}
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

	int nDamageTotal = 0;

	for (int nIndex = 0; nIndex < oEDamageIndex_MAX; nIndex++)
	{
		if (this->HasFlag(descDamage.enuModeDamage, 1 << nIndex))
		{
			nDamageTotal += descDamage.aryDamage[nIndex];
		}
	}

	bool bDivideTotalDamage = !nDamageTotal;
	bool bIsSemiHuman = pNpcAttacker && !pNpcAttacker->IsHalfMonster();

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

	if (pNpcAttacker && bIsSemiHuman)
	{
		for (int nIndex = 0; nIndex < oEDamageIndex_MAX; nIndex++)
		{
			descDamage.aryDamage[nIndex] += pNpcAttacker->damage[nIndex];
		}
	}

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
			nDamageApplied = nDamageCurrent - nProtectionCurrent;
			if (nDamageApplied < 0) nDamageApplied = 0;
			nDamageEffective += nDamageApplied;
			descDamage.aryDamageEffective[nDamageIndex] = nDamageApplied;
		}
		else
		{
			descDamage.aryDamageEffective[nDamageIndex] = 0;
		}
	}

	descDamage.fDamageTotal = (float)nDamageEffective;

	if (this->HasFlag(NPC_FLAG_IMMORTAL) || immortalByProtection) descDamage.fDamageTotal = 0.0f;

	descDamage.fDamageEffective = descDamage.fDamageTotal;

	if (descDamage.fDamageEffective < 0.0f) descDamage.fDamageEffective = 0.0f;

	descDamage.fDamageReal = descDamage.fDamageEffective;
	int nDamage = (int)descDamage.fDamageReal;

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

		// if (bDeathByBarrier || bForceBarrierDeath) nDamage = this->attribute[NPC_ATR_HITPOINTS];
		if (bForceBarrierDeath) nDamage = this->attribute[NPC_ATR_HITPOINTS] - 1;
		if (bDeathByBarrier) nDamage = this->attribute[NPC_ATR_HITPOINTS];
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
}

void hCSkyControler_Outdoor::ReadFogColorsFromINI()
{
	zVEC3 defaultCol0(116.0f, 89.0f, 75.0f);
	zVEC3 defaultCol1(80.0f, 90.0f, 80.0f);
	zVEC3 defaultCol2(120.0f, 140.0f, 180.0f);
	zVEC3 defaultCol3(120.0f, 140.0f, 180.0f);

	this->zCSkyControler_Outdoor::ReadFogColorsFromINI();

	// Maybe actually read this from ini, but then this whole thing would be unnecessary
	this->fogColorDayVariations[0] = defaultCol0;
	this->fogColorDayVariations[1] = defaultCol1;
	this->fogColorDayVariations[2] = defaultCol2;
	this->fogColorDayVariations[3] = defaultCol3;
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

void PatchGothic1(void)
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
	InjectHook(0x005E6443, &hCSkyControler_Outdoor::ReadFogColorsFromINI); // zCSkyControler_Outdoor::zCSkyControler_Outdoor()

	// No snow in NCI and NCO
	PatchJump(0x00640811, 0x00640861); // oCZoneMusic::ProcessZoneList()

	// Fix dark trees in WALD sectors
	// very hacky - there are other lighting differences, too (see Old Mine Entrance, Abandoned Mine Entrance ...)
	// perhaps it's better to understand how the lighting differs from G1 to G2 ...
	InjectHook(0x005D57DA, zCRenderLightContainer_CollectLights_StatLights_Hook); // zCRenderLightContainer::CollectLights_StatLights()

	// Damage calculation
	InjectHook(0x00666513, &hCNpc::OnDamage_Hit); // oCNpc::OnDamage()

	// TODO Fire in OnDamage_Anim (or not? honestly not one of the more desirable features from G1 ...)
	// TODO Why is the water less transparent?
	// TODO Weird sounds when swinging sword near water
	// TODO override GetTalentValue ... Npc_SetTalentSkill doesn't set talent value, which is unnice to say the least since no NPC actually has "value" ...
}

#define PD_TA_FRAME 1
#define PD_TA_LOOP 2
#define PD_TA_CHECK 3
#define PD_TA_DETAIL 4

#define PD_ZS_FRAME 6
#define PD_ZS_LOOP 7
#define PD_ZS_CHECK 8
#define PD_ZS_DETAIL 9

#define PD_MST_FRAME 11
#define PD_MST_LOOP 12
#define PD_MST_CHECK 13
#define PD_MST_DETAIL 14

#define PD_ORC_FRAME 16
#define PD_ORC_LOOP 17
#define PD_ORC_CHECK 18
#define PD_ORC_DETAIL 19

#define PD_MISSION 21
#define PD_CUTSCENE 22
#define PD_SPELL 23
#define PD_ITEM_MOBSI 24
#define PD_MAGIC 25

bool PrintDebugInstCh()
{
	zCPar_Symbol *sym = cur_parser->GetSymbol("SELF");

	zCVob *self;
	if (sym) self = (zCVob *)sym->GetInstanceAdr();

	zSTRING s;
	int ch;

	cur_parser->GetParameter(s);
	cur_parser->GetParameter(ch);

	switch(ch)
	{
#if 0
	case PD_MST_LOOP:
	case PD_ZS_DETAIL:
		break;
#else
	case PD_MAGIC: // dummy case
#endif
	default:
		printf("%02d: %s\n", ch, s.ToChar());
	}

	return FALSE;
}

#define zPARTICLE_MAX_GLOBAL 65536

void PatchGothic2(void)
{
	if (G12GetPrivateProfileBool("PrintDebugInstCh", FALSE))
	{
		Patch(0x006D4B1B + 1, PrintDebugInstCh); // oCGame::DefineExternals_Ulfi()
	}

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
		InjectHook(0x0073369B, &hCNpc::CreateVobList); // oCNpc::ToggleFocusVob()
		InjectHook(0x00733BE9, &hCNpc::CreateVobList); // oCNpc::CollectFocusVob()
		InjectHook(0x0075DC54, &hCNpc::CreateVobList); // oCNpc::PerceiveAll()
		InjectHook(0x0075DE95, &hCNpc::CreateVobList); // oCNpc::PerceptionCheck()
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
		InjectHook(0x004F16C6, &hCActiveSnd::AutoCalcObstruction); // zCSndSys_MSS::PlaySound3D()
		InjectHook(0x004F2080, &hCActiveSnd::AutoCalcObstruction); // zCSndSys_MSS::PlaySound3DAmbient()
		InjectHook(0x004F259B, &hCActiveSnd::AutoCalcObstruction); // zCSndSys_MSS::UpdateSound3D()
		InjectHook(0x004F3275, &hCActiveSnd::AutoCalcObstruction); // zCSndSys_MSS::UpdateSoundPropsAmbient()
	}

	if (G12GetPrivateProfileBool("FixGetDistance", TRUE))
	{
		// GetDistance* all don't check the last point in the list
		Patch(0x0047405E + 2, 0x008CDF94 + sizeof(zVEC2)); // oCMagFrontier::GetDistanceNewWorld()
		Patch(0x00474568 + 2, 0x008CDEC0 + sizeof(zVEC2)); // oCMagFrontier::GetDistanceDragonIsland()
		Patch(0x00474728 + 2, 0x008CDE6C + sizeof(zVEC2)); // oCMagFrontier::GetDistanceAddonWorld()
	}

	if (G12GetPrivateProfileBool("FixSkeletonMageGuild", TRUE))
	{
		// Fix DestroyUndead
		Patch(0x0048619B + 2, (BYTE)NPC_GIL_SKELETON_MAGE); // oCSpell::IsTargetTypeValid()

		// Fix IsSkeleton()
		Patch(0x0083D840, &hCNpc::IsSkeleton); // oCNpc::`vftable'
	}

	if (G12GetPrivateProfileBool("ClassSizeFixes", TRUE))
	{
		// Don't instantiate an oCEmitterKey object for checking ScriptClassSize
		PatchJump(0x0048B6DD, 0x0048B6EF); // oCVisualFX::InitParser()

		// Don't instantiate an oCVisualFX object ScriptClassSize
		PatchJump(0x0048B73A, 0x0048B782); // oCVisualFX::InitParser()
	}

	if (G12GetPrivateProfileBool("Gothic1Mode", FALSE))
	{
		PatchGothic1();
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
	if (GOTHIC26FIX)
	{
		G12AllocConsole();
		PatchGothic2();
	}
	else if (SPACER26MOD)
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
