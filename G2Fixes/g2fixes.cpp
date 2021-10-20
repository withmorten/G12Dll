#include "..\G12Dll\G12Core.h"

#define G12DLL_NAME "G2Fixes"

#define IGNORE_CUSTOM_SYSTEMPACK_FIXES // these are already in a custom SytemPack .PATCH file

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

	bool32 delete_vob;

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

bool32 hCNpc::IsSkeleton()
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

	bool32 bDivideTotalDamage = !nDamageTotal;
	bool32 bIsSemiHuman = pNpcAttacker && !pNpcAttacker->IsHalfMonster();

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
			bool32 bBlunt = ((descDamage.enuModeDamage & oEDamageType_Blunt) == oEDamageType_Blunt);
			bool32 bEdge = ((descDamage.enuModeDamage & oEDamageType_Edge) == oEDamageType_Edge);
			bool32 bPoint = ((descDamage.enuModeDamage & oEDamageType_Point) == oEDamageType_Point);

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
	bool32 immortalByProtection = FALSE;
	bool32 canBeImmortalByProtection = TRUE;

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

	bool32 bBarrier = this->HasFlag(descDamage.enuModeDamage, oEDamageType_Barrier);
	bool32 bDeathByBarrier = FALSE;
	const bool32 bForceBarrierDeath = TRUE; // for some extra spicy barrier behaviour

	if (bBarrier)
	{
		if (this->anictrl)
		{
			bool32 bInWater = this->anictrl->GetWaterLevel() > 1;
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

	this->fogColorDayVariations2[0] = defaultCol0;
	this->fogColorDayVariations2[1] = defaultCol1;
	this->fogColorDayVariations2[2] = defaultCol2;
	this->fogColorDayVariations2[3] = defaultCol3;
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

const char *AppName = "Gothic - 2.6 (fix)";
const char *WorldZen = "world.zen";
const char *ItMiNugget = "ITMINUGGET";
const char *Erz = "Erz: ";
const char *NotEnoughOre = "Nicht genug Erz um den Gegenstand zu kaufen.";
const char *NoSound = "NEWGAME";
const char *SectorName = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

hCSoundManager::hTScriptSoundData *scriptSoundData = NULL;

void PatchGothic1(void)
{
	// Fix App Title
	Patch(0x0089D9AC, AppName); // APP_NAME

	// New game starts WORLD.ZEN
	Patch(0x00429A23 + 1, WorldZen); // CGameManager::Menu()
	Patch(0x00429A52 + 1, WorldZen); // CGameManager::Menu()

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

	// Fix Evening preset
	Patch(0x005E3BAF + 4, 180.0f); // zCSkyState::PresetEvening()
	Patch(0x005E3BBF + 4, 75.0f); // zCSkyState::PresetEvening()
	Patch(0x005E3BCE + 4, 60.0f); // zCSkyState::PresetEvening()

	// No snow in NCI and NCO
	PatchJump(0x00640811, 0x00640861); // oCZoneMusic::ProcessZoneList()
	PatchJump(0x00640CA3, 0x00640CF3); // oCZoneMusic::ProcessZoneList()

	// Fix dark trees in WALD sectors
	// very hacky - there are other lighting differences, too (see Old Mine Entrance, Abandoned Mine Entrance, Sleeper Entrance ...)
	// perhaps it's better to understand how the lighting differs from G1 to G2 ...
	InjectHook(0x005D57DA, zCRenderLightContainer_CollectLights_StatLights_Hook); // zCRenderLightContainer::CollectLights_StatLights()

	// Damage calculation
	InjectHook(0x00666513, &hCNpc::OnDamage_Hit); // oCNpc::OnDamage()

	// Disable some Gothic 2 specific cheats
	// HESSE / GARRETT
	Nop(0x0043350D, 6); // HandleResultString()
	Nop(0x00433518, 6); // HandleResultString()

	// LARES
	Nop(0x00433523); // HandleResultString()
	Patch(0x00433523 + 1, (BYTE)0xE9); // HandleResultString()

	// THEQUEENISDEAD
	Nop(0x004345DA); // HandleResultString()
	Patch(0x004345DA + 1, (BYTE)0xE9); // HandleResultString()

	// No CheckMeleeWeaponHitsLevel
	Nop(0x0075004B, 6); // oCNpc::EV_AttackForward()
	Nop(0x00750BF2, 6); // oCNpc::EV_AttackLeft()
	Nop(0x007516CA, 6); // oCNpc::EV_AttackRight()
	Nop(0x007519ED, 6); // oCNpc::EV_AttackRun()

	// Movers collide with static vobs
	Patch(0x00611221, (BYTE)0xEB); // zCMover::CanThisCollideWith

	// Fix scriptSoundData pfxName (use zCSndFrame here)
	scriptSoundData = new hCSoundManager::hTScriptSoundData();

	Patch(0x005ED7B8 + 1, scriptSoundData); // zCSoundManager::zCSoundEventData::GetData()
	Patch(0x005ED7D4 + 1, scriptSoundData + offsetof(hCSoundManager::hTScriptSoundData, pfxName)); // zCSoundManager::zCSoundEventData::GetData()

	Patch(0x005ED865 + 1, scriptSoundData); // zCSoundManager::zCSoundEventData::GetData()
	Patch(0x005ED875 + 1, scriptSoundData + offsetof(hCSoundManager::hTScriptSoundData, pfxName)); // zCSoundManager::zCSoundEventData::GetData()

	Patch(0x005EDFBB + 1, scriptSoundData); // zCSoundManager::StartSlideSound()
	Patch(0x005EDFCB + 1, scriptSoundData + offsetof(hCSoundManager::hTScriptSoundData, pfxName)); // zCSoundManager::StartSlideSound()

	Patch(0x005EE220 + 1, scriptSoundData); // zCSoundManager::StartDestructionSound()
	Patch(0x005EE230 + 1, scriptSoundData + offsetof(hCSoundManager::hTScriptSoundData, pfxName)); // zCSoundManager::StartDestructionSound()

	// Same time for PrintTimedCXY() as in G1 SystemPack
	Patch(0x006FD1FE + 1, 1500.0f); // oCGame::HandleEvent()

	// Fix less transparent water
	Patch(0x005D84A7, (BYTE)0xEB);

	// TODO wtf is up with walking in the SleeperEntrance? On angled floor it's veeeery slow - also lighting there is fucked
	// TODO Fire in OnDamage_Anim (or not? honestly not one of the more desirable features from G1 ...)
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

bool32 PrintDebugInstCh()
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

zCArchiver *arc;
zCTrigger *trigger;

void zCTrigger_Unarchive(void)
{
	bool32 isEnabled = TRUE;
	arc->ReadBool("isEnabled", isEnabled);
	trigger->flags.isEnabled = isEnabled;
}

ASM(zCTrigger_Unarchive_Hook)
{
	__asm
	{
		mov arc, esi
		mov trigger, edi
		call zCTrigger_Unarchive
	}

	RET(0x00610D02);
}

int Game_SpeciesConsole_TempFightRange;
float Game_SpeciesConsole_JumpupHeight;
int Game_SpeciesConsole_BloodMaxDistance;
int Game_SpeciesConsole_BloodFlow;
int Game_SpeciesConsole_BloodAmount;

#define zCON_TYPE_FLOAT 1

#define zPARTICLE_MAX_GLOBAL 65536

void PatchGothic2(void)
{
	if (G12GetPrivateProfileBool("PrintDebugInstCh", FALSE))
	{
		Patch(0x006D4B1B + 1, PrintDebugInstCh); // oCGame::DefineExternals_Ulfi()
	}

	if (G12GetPrivateProfileBool("FixTriggerIsEnabled", FALSE))
	{
		// Fix zCTrigger::Unarchive - isEnabled doesn't get read properly
		InjectHook(0x00610CFD, zCTrigger_Unarchive_Hook, PATCH_JUMP); // zCTrigger::Unarchive()
	}

	if (G12GetPrivateProfileBool("FixSpeciesConsoleVars", TRUE))
	{
		// These values are shorts in the exe, but get treated as ints by the functions that receive these pointers ... so replace them with ints
		Nop(0x006D3821); // Game_OpenSpeciesConsole()
		Patch(0x006D3821 + 2, &Game_SpeciesConsole_TempFightRange); // Game_OpenSpeciesConsole()
		Patch(0x006D384C + 1, &Game_SpeciesConsole_TempFightRange); // Game_OpenSpeciesConsole()

		// replace this one with a float, so get rid of all the float2int, int2float conversions
		Nop(0x006D2A3C); // Game_SpeciesChanged()
		Patch(0x006D2A3C + 1, (BYTE)0x8B); // Game_SpeciesChanged()
		Patch(0x006D2A3C + 3, &Game_SpeciesConsole_JumpupHeight); // Game_SpeciesChanged()
		Nop(0x006D2A43, 4); // Game_OpenSpeciesConsole()
		Nop(0x006D2A48, 4); // Game_OpenSpeciesConsole()
		Nop(0x006D2A4E, 3); // Game_OpenSpeciesConsole()
		Nop(0x006D309C, 5); // Game_OpenSpeciesConsole()
		Patch(0x006D30A1 + 0, (BYTE)0xD9); // Game_OpenSpeciesConsole()
		Patch(0x006D30A1 + 1, (BYTE)0x1D); // Game_OpenSpeciesConsole()
		Patch(0x006D30A1 + 2, &Game_SpeciesConsole_JumpupHeight); // Game_OpenSpeciesConsole()
		Patch(0x006D30E0 + 1, &Game_SpeciesConsole_JumpupHeight); // Game_OpenSpeciesConsole()
		Patch(0x006D30E5 + 1, (BYTE)zCON_TYPE_FLOAT); // Game_OpenSpeciesConsole()

		Nop(0x006D2A8D, 2); // Game_SpeciesChanged()
		Patch(0x006D2A8D + 2, (BYTE)0xA1); // Game_SpeciesChanged()
		Patch(0x006D2A8D + 3, &Game_SpeciesConsole_BloodMaxDistance); // Game_SpeciesChanged()
		Nop(0x006D31D4); // Game_OpenSpeciesConsole()
		Patch(0x006D31D4 + 2, &Game_SpeciesConsole_BloodMaxDistance); // Game_OpenSpeciesConsole()
		Patch(0x006D320F + 1, &Game_SpeciesConsole_BloodMaxDistance); // Game_OpenSpeciesConsole()

		Nop(0x006D2AE9); // Game_SpeciesChanged()
		Patch(0x006D2AE9 + 3, &Game_SpeciesConsole_BloodFlow); // Game_SpeciesChanged()
		Nop(0x006D3272); // Game_OpenSpeciesConsole()
		Patch(0x006D3272 + 2, &Game_SpeciesConsole_BloodFlow); // Game_OpenSpeciesConsole()
		Patch(0x006D32A3 + 1, &Game_SpeciesConsole_BloodFlow); // Game_OpenSpeciesConsole()

		Nop(0x006D2B31, 2); // Game_SpeciesChanged()
		Patch(0x006D2B31 + 2, (BYTE)0xA1); // Game_SpeciesChanged()
		Patch(0x006D2B31 + 3, &Game_SpeciesConsole_BloodAmount); // Game_SpeciesChanged()
		Nop(0x006D32F9); // Game_OpenSpeciesConsole()
		Patch(0x006D32F9 + 2, &Game_SpeciesConsole_BloodAmount); // Game_OpenSpeciesConsole()
		Patch(0x006D333E + 1, &Game_SpeciesConsole_BloodAmount); // Game_OpenSpeciesConsole()
	}

	if (G12GetPrivateProfileBool("NoHardcodedThreatMusic", FALSE))
	{
		// No hardcoded threat music in forest
		Patch(0x00696DBD + 1, SectorName); // oCAIHuman::GetEnemyThreat()
	}

	// Some SystemPack fixes not in Gothic 2
#ifndef IGNORE_CUSTOM_SYSTEMPACK_FIXES
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
#endif

	// if (G12GetPrivateProfileBool("ThrowFlag", TRUE))
	{
		// Nop(0x0070E9A8, 6); // oCNpcInventory::HandleEvent()
		// ...
	}

	if (G12GetPrivateProfileBool("HideFocus", TRUE))
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

#ifndef IGNORE_CUSTOM_SYSTEMPACK_FIXES
	if (G12GetPrivateProfileBool("FixGetDistance", TRUE))
	{
		// GetDistance* all don't check the last point in the list
		Patch(0x0047405E + 2, 0x008CDF94 + sizeof(zVEC2)); // oCMagFrontier::GetDistanceNewWorld()
		Patch(0x00474568 + 2, 0x008CDEC0 + sizeof(zVEC2)); // oCMagFrontier::GetDistanceDragonIsland()
		Patch(0x00474728 + 2, 0x008CDE6C + sizeof(zVEC2)); // oCMagFrontier::GetDistanceAddonWorld()
	}
#endif

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
	else
	{
		// disable use of scriptSoundData - program reads over the actual size of the struct
		Patch(0x005ED7B6, (BYTE)0xEB); // zCSoundManager::zCSoundEventData::GetData()
		Patch(0x005ED85B, (BYTE)0xEB); // zCSoundManager::zCSoundEventData::GetData()
		Patch(0x005EDFB1, (BYTE)0xEB); // zCSoundManager::StartSlideSound()
		Patch(0x005EE216, (BYTE)0xEB); // zCSoundManager::StartDestructionSound()

		// no SPL_CHARGEFIRESTORM (SPL_PYROKINESIS, G1 engine leftover) check
		Patch(0x00735231 + 2, (BYTE)SPL_SUCKENERGY); // oCNpc::IsConditionValid()
		PatchJump(0x00735240, 0x00735265); // oCNpc::IsConditionValid()

		// and inlined calls
		Patch(0x00732B0C + 2, (BYTE)SPL_SUCKENERGY); // oCNpc::GetCSStateFlags()
		PatchJump(0x00732B1B, 0x00732B3A); // oCNpc::GetCSStateFlags()

		Patch(0x00734C78 + 1, (BYTE)SPL_SUCKENERGY); // oCNpc::SetEnemy()
		PatchJump(0x00734C85, 0x00734E18); // oCNpc::SetEnemy()

		Patch(0x00734DF5 + 1, (BYTE)SPL_SUCKENERGY); // oCNpc::SetEnemy()
		PatchJump(0x00734E02, 0x00734E0F); // oCNpc::SetEnemy()

		Patch(0x00734EC5 + 1, (BYTE)SPL_SUCKENERGY); // oCNpc::GetNextEnemy()
		PatchJump(0x00734ED2, 0x00734EDF); // oCNpc::GetNextEnemy()

		Patch(0x007350C0 + 1, (BYTE)SPL_SUCKENERGY); // oCNpc::GetNextEnemy()
		PatchJump(0x007350CD, 0x007350DA); // oCNpc::GetNextEnemy()

		Patch(0x00740D71 + 2, (BYTE)SPL_SUCKENERGY); // oCNpc::FindNpcEx()
		PatchJump(0x00740D80, 0x00740DA5); // oCNpc::FindNpcEx()

		Patch(0x00740FCF + 1, (BYTE)SPL_SUCKENERGY); // oCNpc::FindNpcExAtt()
		PatchJump(0x00740FE0, 0x00740FF1); // oCNpc::FindNpcExAtt()

		Patch(0x00741265 + 1, (BYTE)SPL_SUCKENERGY); // oCNpc::GetComrades()
		PatchJump(0x00741272, 0x0074127F); // oCNpc::GetComrades()

		Patch(0x0074B575 + 1, (BYTE)SPL_SUCKENERGY); // oCNpc::OnMessage()
		PatchJump(0x0074B582, 0x0074B58F); // oCNpc::OnMessage()

		Patch(0x0074BA32 + 1, (BYTE)SPL_SUCKENERGY); // oCNpc::OnMessage()
		PatchJump(0x0074BA43, 0x0074BA54); // oCNpc::OnMessage()
	}
}

void Init(void)
{
	if (GOTHIC26FIX)
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
