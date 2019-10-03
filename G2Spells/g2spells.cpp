#include "..\G12Dll\G12Core.h"

#define G12DLL_NAME "G2Spells"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G2.hpp"
#include "..\G12Dll\G2.h"
#include "g2spells.hpp"

const float SCANNER_DIM_Y = 100.0f;
const float SCANNER_DIM_X = 15.0f;
const float AUTODISPOSE_TIME_LIGHTNING = 0.1f;
// const float AUTODISPOSE_TIME_LIGHTNING = 1.0f;
const float AUTODISPOSE_TIME_LIGHTNING_INV = 1.0f / AUTODISPOSE_TIME_LIGHTNING;

hCFXScanner *hCFXScanner::Constructor()
{
	this->enabled = FALSE;
	this->initVob = NULL;
	this->alpha = 255;

	for (int i = 0; i < NUM_SCANNER_VOBS; i++)
	{
		this->scannerVobs[i] = NULL;
	}

	return this;
}

void hCFXScanner::Destructor()
{
	for (int i = 0; i < NUM_SCANNER_VOBS; i++)
	{
		if (this->scannerVobs[i])
		{
			this->scannerVobs[i]->RemoveVobFromWorld();

			zRELEASE(this->scannerVobs[i]);
		}
	}
}

bool hCFXScanner::Initialized()
{
	for (int i = 0; i < NUM_SCANNER_VOBS; i++)
	{
		if (!this->scannerVobs[i]) return FALSE;
	}

	return TRUE;
}

void hCFXScanner::Enable(zCVob *orgVob)
{
	this->enabled = TRUE;

	for (int i = 0; i < NUM_SCANNER_VOBS; i++)
	{
		zCDecal *dc = zSTATIC_CAST<zCDecal>(this->scannerVobs[i]->visual);

		if (dc) dc->decalMaterial->color = zCOLOR(255, 255, 255, this->alpha);

		this->scannerVobs[i]->SetSleeping(FALSE);
		this->scannerVobs[i]->showVisual = TRUE;
	}

	this->initVob = orgVob;
}

void hCFXScanner::Disable()
{
	this->enabled = FALSE;

	for (int i = 0; i < NUM_SCANNER_VOBS; i++)
	{
		zCDecal *dc = zSTATIC_CAST<zCDecal>(this->scannerVobs[i]->visual);

		if (dc) dc->decalMaterial->color.alpha = 0;

		this->scannerVobs[i]->SetSleeping(TRUE);
		this->scannerVobs[i]->showVisual = FALSE;
	}
}

void hCFXScanner::Run()
{
	for (int i = 0; i < NUM_SCANNER_VOBS; i++)
	{
		zCVob *scannerVob = this->scannerVobs[i];

		scannerVob->RotateLocalX((float)zRand(360));
		scannerVob->RotateLocalY((float)zRand(360));
		scannerVob->RotateLocalZ((float)zRand(360));

		zVEC3 head = scannerVob->trafoObjToWorld.GetAtVector() * SCANNER_DIM_Y;
		zVEC3 newPos = (head * 0.5f) + this->initVob->GetPositionWorld();
		zVEC3 right = (-head).Normalize();
		zVEC3 up = (right ^ (zCCamera::activeCam->connectedVob->GetPositionWorld() - newPos).Normalize()); // 1.01d is different
		zVEC3 at = (up ^ right).Normalize();

		zMAT4 trafo = Alg_Identity3D();
		trafo.SetTranslation(newPos);
		trafo.SetAtVector(at);
		trafo.SetRightVector(right);
		trafo.SetUpVector(up);

		zCDecal *dc = zSTATIC_CAST<zCDecal>(scannerVob->visual);

		if (dc) dc->SetDecalDim(SCANNER_DIM_Y, SCANNER_DIM_X);

		scannerVob->visualCamAlign = zVISUAL_CAMALIGN_NONE;
		scannerVob->UpdateVisualDependencies(TRUE);
		scannerVob->SetTrafo(trafo);
	}
}

zCClassDef &hCVisFX_Lightning::classDef = *(zCClassDef *)0x008CE7F8;

hCVisFX_Lightning *hCVisFX_Lightning::Constructor()
{
	// Constructor start
	this->burnVobs.numInArray = 0;
	this->burnVobs.numAlloc = 0;
	this->burnVobs.array = NULL;

	this->burnNodes.numInArray = 0;
	this->burnNodes.numAlloc = 0;
	this->burnNodes.array = NULL;

	this->decalVobs.numInArray = 0;
	this->decalVobs.numAlloc = 0;
	this->decalVobs.array = NULL;

	this->electricFX.numInArray = 0;
	this->electricFX.numAlloc = 0;
	this->electricFX.array = NULL;

	this->scanner.Constructor();
	// Constructor end

	this->targetNode = NULL;
	this->phase = 0;
	this->showScanner = FALSE;
	this->investedNext = FALSE;
	this->castOnSelf = FALSE;
	this->lightRange = 1000.0f;

	this->dontWriteIntoArchive = TRUE;

	return this;
}

ASM(hCVisFX_Lightning_Constructor_Hook)
{
	__asm
	{
		mov ecx, esi
		call hCVisFX_Lightning::Constructor
	}

	RET(0x0049F7B9);
}

void hCVisFX_Lightning::Destructor()
{
	for (int i = 0; i < this->decalVobs.numInArray; i++)
	{
		this->decalVobs[i]->RemoveVobFromWorld();

		zRELEASE(this->decalVobs[i]);
	}

	for (int i = 0; i < this->electricFX.numInArray; i++)
	{
		this->electricFX[i]->Stop(TRUE);

		zRELEASE(this->electricFX[i]);
	}

	oCNpc *orgNpc = zDYNAMIC_CAST<oCNpc>(this->origin);

	for (int i = 0; i < this->burnVobs.numInArray; i++)
	{
		oCNpc *burnNpc = zDYNAMIC_CAST<oCNpc>(this->burnVobs[i]);

		if (orgNpc && burnNpc) burnNpc->AssessStopMagic_S(orgNpc, this->spellType);
	}

	for (int i = 1; i < this->vobList.numInArray; i++)
	{
		if (showTarget && i >= 1) this->vobList[i]->drawBBox3D = FALSE;

		zRELEASE(this->vobList[i]);
	}

	this->vobList.DeleteList();

	// Destructor start
	delete[] this->burnVobs.array;
	this->burnVobs.array = NULL;

	delete[] this->burnNodes.array;
	this->burnNodes.array = NULL;

	delete[] this->decalVobs.array;
	this->decalVobs.array = NULL;

	delete[] this->electricFX.array;
	this->electricFX.array = NULL;

	this->scanner.Destructor();
	// Destructor end
}

ASM(hCVisFX_Lightning_Destructor_Hook)
{
	__asm
	{
		mov ecx, esi
		call hCVisFX_Lightning::Destructor
	}

	RET(0x0049ECC5);
}

bool hCVisFX_Lightning::CheckDeletion()
{
	if (this->shouldDelete)
	{
		this->ai->delete_it = TRUE;

		return TRUE;
	}

	if (!this->isDeleted)
	{
		if (this->visual)
		{
			zCParticleFX *pfx = zDYNAMIC_CAST<zCParticleFX>(this->visual);

			if (this->visual->GetVisualDied() || pfx && pfx->CalcIsDead())
			{
				if (this->fxState >= zVFXSTATE_CAST) this->isDeleted = TRUE;
			}
		}
	}

	if (this->emFXLifeSpan != -1.0f)
	{
		this->lifeSpanTimer += this->frameTime;

		if (this->lifeSpanTimer >= this->emFXLifeSpan)
		{
			this->emFXLifeSpan = -1.0f;

			this->isDeleted = TRUE;
		}
	}

	if (this->isDeleted)
	{
		if (this->deleteTime == 0.0f)
		{
			oCNpc *orgNpc = zDYNAMIC_CAST<oCNpc>(this->origin);
			oCNpc *infNpc = zDYNAMIC_CAST<oCNpc>(this->inflictor);

			if (orgNpc && infNpc && this->sendAssessMagic)
			{
				orgNpc->AssessStopMagic_S(infNpc, this->spellType);
			}

			if (this->visual)
			{
				zCParticleFX *pfx = zDYNAMIC_CAST<zCParticleFX>(this->visual);

				if (pfx && pfx->emitter)
				{
					pfx->StopEmitterOutput();
					pfx->emitter->ppsIsLooping = FALSE;

					for (zCParticleEmitter *em = pfx->emitter; em; em = em->ppsCreateEmitter)
					{
						if (em->ppsIsLooping)
						{
							em->isOneShotFX = TRUE;
							em->ppsIsLooping = FALSE;
						}
					}
				}
			}
		}

		this->deleteTime += this->frameTime;

		if (this->deleteTime >= AUTODISPOSE_TIME_LIGHTNING)
		{
			this->deleteTime = AUTODISPOSE_TIME_LIGHTNING;

			for (int i = 0; i < this->childList.numInArray; i++)
			{
				this->childList[i]->Stop(TRUE);
			}

			this->shouldDelete = TRUE;
		}

		float t = 1.0f - (zSinusEase(AUTODISPOSE_TIME_LIGHTNING_INV * this->deleteTime));

		if (this->light)
		{
			this->light->SetRange(t * this->lightRange, TRUE);
		}

		zCSoundSystem::zTSound3DParams params;

		if (this->sfxHnd && zsound->GetSound3DProps(this->sfxHnd, params))
		{
			params.volume = t;

			zsound->UpdateSound3D(this->sfxHnd, &params);
		}

		if (this->visual && this->visAlpha != 0.0f)
		{
			this->visualAlphaEnabled = TRUE;
			this->visualAlpha = t;
		}
	}

	return FALSE;
}

void hCVisFX_Lightning::UpdateBurnVobs()
{
	for (int i = 0; i < this->burnVobs.numInArray; i++)
	{
		if (!this->vobList.IsInList(this->burnVobs[i]))
		{
			oCNpc *orgNpc = zDYNAMIC_CAST<oCNpc>(this->origin);
			oCNpc *targetNpc = zDYNAMIC_CAST<oCNpc>(this->burnVobs[i]);

			if (orgNpc && targetNpc)
			{
				targetNpc->AssessStopMagic_S(orgNpc, this->spellType);
			}

			this->burnVobs.RemoveOrderIndex(i);
			this->burnNodes.RemoveOrderIndex(i);
		}
	}

	for (int i = 0; i < this->vobList.numInArray; i++)
	{
		if (!this->burnVobs.IsInList(this->vobList[i]))
		{
			int upKey = this->trajectory.VobCross(this->vobList[i]);

			if (upKey != -1)
			{
				this->burnVobs.InsertAtPos(this->vobList[i], upKey);

				zCModelNodeInst *ins = NULL;
				zCModel *mdl = zDYNAMIC_CAST<zCModel>(this->vobList[i]->visual);

				if (mdl && !this->emTrjTargetNode_S.IsEmpty())
				{
					ins = mdl->SearchNode(this->emTrjTargetNode_S);
				}

				this->burnNodes.InsertEnd(ins);
			}
		}
	}

	this->trajectory.SetByList(this->burnVobs);
}

bool hCVisFX_Lightning::UpdateBurnVobsInvestNext()
{
	zCVob *nearestVob = NULL;

	if (!this->burnVobs.numInArray)
	{
		this->burnVobs.InsertEnd(this->origin);
		this->burnNodes.InsertEnd(this->orgNode);

		if (this->vobList.numInArray < 2) return FALSE;

		nearestVob = this->vobList[1];
	}
	else
	{
		float minDist = FLT_MAX;
		zCVob *orgVob = this->burnVobs[this->burnVobs.numInArray - 1];

		for (int i = 1; i < this->vobList.numInArray; i++)
		{
			zCVob *vob = this->vobList[i];

			if (this->burnVobs.IsInList(vob)) continue;

			zVEC3 orgPos = orgVob->GetPositionWorld();
			zVEC3 targetPos = vob->GetPositionWorld();

			float dist = (targetPos - orgPos).Length2();

			if (minDist <= dist) continue;

			if (this->origin->homeWorld->TraceRayFirstHit(orgPos, targetPos - orgPos, &this->vobList, zTRACERAY_STAT_POLY)) continue;

			nearestVob = vob;
			minDist = dist;
		}
	}

	if (!nearestVob) return FALSE;

	this->burnVobs.InsertEnd(nearestVob);

	zCModel *mdl = zDYNAMIC_CAST<zCModel>(nearestVob->visual);

	if (mdl && !this->emTrjTargetNode_S.IsEmpty()) this->burnNodes.InsertEnd(mdl->SearchNode(this->emTrjTargetNode_S));
	else this->burnNodes.InsertEnd(NULL);

	this->trajectory.SetByList(this->burnVobs);

	return TRUE;
}

void hCVisFX_Lightning::Draw()
{
	if (this->burnVobs.numInArray - 1 <= this->decalVobs.numInArray)
	{
		if (this->decalVobs.numInArray > 0 && this->decalVobs.numInArray >= this->burnVobs.numInArray)
		{
			int numDecRemove = this->decalVobs.numInArray - this->burnVobs.numInArray;

			for (int j = 0; j < numDecRemove; j++)
			{
				zCVob *delVob = this->decalVobs[this->decalVobs.numInArray - 1];

				this->decalVobs.RemoveIndex(this->decalVobs.numInArray - 1);

				delVob->RemoveVobFromWorld();

				zRELEASE(delVob);
			}
		}
	}
	else
	{
		int numDecals = this->burnVobs.numInArray - this->decalVobs.numInArray - 1;

		for (int k = 0; k < numDecals; k++)
		{
			zCVob *decalVob = zCVob::_CreateNewInstance();

			decalVob->SetVobName(this->fxName + " decal Vob");
			decalVob->SetSleeping(FALSE);
			decalVob->SetPhysicsEnabled(FALSE);
			decalVob->SetAI(NULL);
			decalVob->dontWriteIntoArchive = TRUE;
			decalVob->SetVisual(this->visName_S);
			decalVob->visualCamAlign = zVISUAL_CAMALIGN_NONE;

			zCDecal *dc = zSTATIC_CAST<zCDecal>(decalVob->visual);

			if (dc)
			{
				dc->SetDecalDim(this->visSize[0], this->visSize[1]);
				dc->decal2Sided = TRUE;
				dc->decalMaterial->rndAlphaBlendFunc = zrenderer->AlphaBlendFuncStringToType(this->visAlphaBlendFunc_S);
				dc->decalMaterial->color = zCOLOR(255, 255, 255, (byte)(this->visAlpha * 255.0f));
				dc->decalMaterial->texAniCtrl.aniFPS = this->visTexAniFPS / 1000.0f;
				dc->decalMaterial->texAniCtrl.bOneShotAni = !this->visTexAniIsLooping;
			}

			this->origin->homeWorld->AddVob(decalVob);

			this->decalVobs.InsertEnd(decalVob);
		}
	}

	for (int i = 0; i < this->decalVobs.numInArray; i++)
	{
		zVEC3 lastPos, thisPos;

		if (this->burnNodes[i + 1]) thisPos = this->burnVobs[i + 1]->GetTrafoModelNodeToWorld(this->burnNodes[i]).GetTranslation();
		else thisPos = this->burnVobs[i + 1]->GetPositionWorld();

		if (this->burnNodes[i]) lastPos = this->burnVobs[i]->GetTrafoModelNodeToWorld(this->burnNodes[i]).GetTranslation();
		else lastPos = this->burnVobs[i]->GetPositionWorld();

		zVEC3 head, newPos, right, at, up;
		zMAT4 trafo;

		head = thisPos - lastPos;
		newPos = lastPos + (head * 0.5f);

		right = (-head).Normalize();

		up = (right ^ (zCCamera::activeCam->connectedVob->GetPositionWorld() - newPos)).Normalize(); // 1.01d is different
		at = (up ^ right).Normalize();

		trafo = Alg_Identity3D();
		trafo.SetTranslation(newPos);
		trafo.SetAtVector(at);
		trafo.SetRightVector(right);
		trafo.SetUpVector(up);

		zCDecal *dc = zSTATIC_CAST<zCDecal>(this->decalVobs[i]->visual);

		if (dc)
		{
			dc->SetDecalDim(head.Length() * 0.5f, head.Length() * 0.2f);
		}

		this->decalVobs[i]->visualCamAlign = zVISUAL_CAMALIGN_NONE;
		this->decalVobs[i]->UpdateVisualDependencies(TRUE); // TODO possibly false
		this->decalVobs[i]->SetTrafo(trafo);
	}
}

void hCVisFX_Lightning::CreateScanner(zCVob *orgVob)
{
	if (this->vobList[0] != orgVob)
	{
		if (!this->scanner.Initialized())
		{
			for (int i = 0; i < NUM_SCANNER_VOBS; i++)
			{
				zCVob *scannerVob = zCVob::_CreateNewInstance();

				scannerVob->SetVobName(this->fxName + " scan Vob");
				// scannerVob->SetSleepingMode(zVOB_AWAKE); // also happens in hCFXScanner::Enable()
				scannerVob->SetPhysicsEnabled(FALSE);
				scannerVob->SetCollDet(FALSE);
				scannerVob->dontWriteIntoArchive = TRUE;
				scannerVob->SetAI(NULL);
				scannerVob->SetVisual(this->visName_S);
				scannerVob->visualCamAlign = zVISUAL_CAMALIGN_NONE;

				zCDecal *dc = zSTATIC_CAST<zCDecal>(scannerVob->visual);

				if (dc)
				{
					dc->SetDecalDim(this->visSize[0], this->visSize[1]);
					dc->decal2Sided = TRUE;
					dc->decalMaterial->rndAlphaBlendFunc = zrenderer->AlphaBlendFuncStringToType(this->visAlphaBlendFunc_S);
					// dc->decalMaterial->color = zCOLOR(255, 255, 255, (byte)(this->visAlpha * 255.0f)); // also happens in hCFXScanner::Enable()
					dc->decalMaterial->texAniCtrl.aniFPS = this->visTexAniFPS / 1000.0f;
					dc->decalMaterial->texAniCtrl.bOneShotAni = !this->visTexAniIsLooping;
				}

				scannerVob->SetTrafo(orgVob->trafoObjToWorld);

				orgVob->homeWorld->AddVob(scannerVob);
				// scannerVob->showVisual = TRUE; // also happens in hCFXScanner::Enable()

				this->scanner.scannerVobs[i] = scannerVob;
			}

			this->scanner.SetAlpha((byte)(this->visAlpha * 255.0f));
		}

		this->scanner.Enable(orgVob);
	}
}

void hCVisFX_Lightning::OnTick()
{
	if (!this->initialized) return;

	this->showVisual = FALSE;

	this->frameTime = ztimer.frameTimeFloat / 1000.0f;

	if (this->CheckDeletion() || !this->origin || (this->vobList.numInArray <= 1)) return;

	this->BeginMovement();
	this->UpdateBurnVobs();

	if (this->investedNext)
	{
		this->investedNext = FALSE;

		if (this->phase == 1)
		{
			if (this->castOnSelf)
			{
				this->burnVobs.RemoveIndex(this->burnVobs.numInArray - 1);
				this->burnNodes.RemoveIndex(this->burnNodes.numInArray - 1);

				this->trajectory.SetByList(this->burnVobs);

				this->castOnSelf = FALSE;
			}

			this->DeleteScanner();

			if (this->vobList.numInArray < 2)
			{
				oCVisualFX *newFX = this->CreateAndCastFX("FX_ELECTRIC", this->origin, this->origin);

				if (newFX) this->electricFX.InsertEnd(newFX);

				this->EndMovement(TRUE); // TODO possibly FALSE

				this->origin->GetEM(FALSE)->OnDamage(this, this->origin, this->damage, this->damageType, this->origin->GetPositionWorld());

				this->BeginMovement();
			}
			else if (!this->UpdateBurnVobsInvestNext())
			{
				this->EndMovement(TRUE); // TODO possibly FALSE

				oCVisualFX *newFX = this->CreateAndCastFX("FX_ELECTRIC", this->origin, this->origin);

				if (newFX) this->electricFX.InsertEnd(newFX);

				this->burnVobs.InsertEnd(this->vobList[0]);
				this->burnNodes.InsertEnd(this->targetNode);

				this->trajectory.SetByList(this->burnVobs);

				this->origin->GetEM(FALSE)->OnDamage(this, this->origin, this->damage, this->damageType, this->origin->GetPositionWorld());

				this->BeginMovement();

				this->castOnSelf = TRUE;
				this->showScanner = TRUE;
			}

			this->EndMovement(TRUE); // TODO possibly FALSE

			zCVob *vob = this->burnVobs[this->burnVobs.numInArray - (this->castOnSelf ? 2 : 1)];

			vob->GetEM(FALSE)->OnDamage(this, this->origin, this->damage, this->damageType, vob->GetPositionWorld());

			oCVisualFX *newFX = this->CreateAndCastFX("FX_ELECTRIC", vob, this->origin);

			if (newFX) this->electricFX.InsertEnd(newFX);

			oCNpc *npc = zDYNAMIC_CAST<oCNpc>(vob);
			oCNpc *orgNpc = zDYNAMIC_CAST<oCNpc>(this->origin);

			if (npc && orgNpc) npc->AssessMagic_S(orgNpc, this->spellType);

			this->BeginMovement();
		}
	}

	if (this->phase == 2)
	{
		if (this->castOnSelf)
		{
			oCNpc *npc = zDYNAMIC_CAST<oCNpc>(this->burnVobs[this->burnVobs.numInArray - 1]);

			this->burnVobs.RemoveIndex(this->burnVobs.numInArray - 1);
			this->burnNodes.RemoveIndex(this->burnNodes.numInArray - 1);

			oCNpc *orgNpc = zDYNAMIC_CAST<oCNpc>(this->origin);

			if (npc && orgNpc) npc->AssessStopMagic_S(orgNpc, this->spellType);

			this->trajectory.SetByList(this->burnVobs);

			this->castOnSelf = FALSE;
		}

		if (this->showScanner)
		{
			this->showScanner = FALSE;

			this->CreateScanner(this->burnVobs[this->burnVobs.numInArray - (this->castOnSelf ? 2 : 1)]);
		}

		this->UpdateScanner();
	}

	this->Draw();

	this->EndMovement(TRUE); // TODO possibly FALSE
}

void hCVisFX_Lightning::Open()
{
	this->oCVisualFX::Open();

	if (this->light)
	{
		this->lightRange = this->light->lightData.range;

		this->light->SetRange(0.0f, TRUE);
	}
}

void hCVisFX_Lightning::Init(zCArray<zCVob *> &trajectoryVobs)
{
	if (this->origin)
	{
		this->SetOrigin(NULL, TRUE);

		for (int nr = 1; nr < this->vobList.numInArray; nr++)
		{
			if (showTarget) this->vobList[nr]->drawBBox3D = FALSE;

			zRELEASE(this->vobList[nr]);
		}

		this->vobList.DeleteList();
	}

	this->oCVisualFX::Init(trajectoryVobs[0], NULL, NULL);

	this->vobList = trajectoryVobs;

	for (int nr = 1; nr < this->vobList.numInArray; nr++)
	{
		if (showTarget && nr >= 1) this->vobList[1]->drawBBox3D = TRUE;

		this->vobList[nr]->AddRef();
	}

	zCModel *mdl = zDYNAMIC_CAST<zCModel>(this->origin->visual);

	if (mdl && !this->emTrjOriginNode_S.IsEmpty())
	{
		this->orgNode = mdl->SearchNode(this->emTrjOriginNode_S);
		this->targetNode = mdl->SearchNode(this->emTrjTargetNode_S);
	}

	if (this->initialized && this->sfx)
	{
		this->sfxHnd = zsound->PlaySound3D(this->sfx, this, zSND_SLOT_NONE, NULL);
	}

	this->initialized = TRUE;
}

void hCVisFX_Lightning::InvestNext()
{
	if (this->initialized)
	{
		this->oCVisualFX::InvestNext();

		this->phase++;

		if (this->phase == 3) this->phase = 1;

		if (this->light) this->light->SetRange(this->lightRange, TRUE);

		this->investedNext = TRUE;
		this->showScanner = TRUE;
	}
}

void hCVisFX_Lightning::Cast(bool killAfterDone)
{
	this->oCVisualFX::Stop(killAfterDone);
}

void hCVisFX_Lightning::Stop(bool killAfterDone)
{
	oCNpc *orgNpc = zDYNAMIC_CAST<oCNpc>(this->origin);

	for (int i = 0; i < this->burnVobs.numInArray; i++)
	{
		oCNpc *burnNpc = zDYNAMIC_CAST<oCNpc>(this->burnVobs[i]);

		if (orgNpc && burnNpc) burnNpc->AssessStopMagic_S(orgNpc, this->spellType);
	}

	// GEngine
	for (int i = 0; i < this->decalVobs.numInArray; i++)
	{
		this->decalVobs[i]->RemoveVobFromWorld();

		zRELEASE(this->decalVobs[i]);
	}

	this->decalVobs.DeleteList(); // GEngine
	this->DeleteScanner(); // GEngine

	this->oCVisualFX::Stop(TRUE); // killAfterDone
}

oCVisualFX *hCSpell::CreateEffect()
{
	if (this->spellID == SPL_CHAINLIGHTNING) this->effect = hCVisFX_Lightning::_CreateNewInstance();
	else this->effect = oCVisualFX::_CreateNewInstance();

	this->effect->SetSpellTargetTypes(this->targetCollectType);
	this->effect->SetDamage((float)this->damagePerLevel);
	this->effect->SetDamageType(this->damageType);
	this->effect->SetSpellType(this->spellID);
	this->effect->SetSpellCat(this->spellType);
	this->effect->SetByScript("SPELLFX_" + this->GetSpellInstanceName(this->spellID));

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

const char *oCVisFX_Lightning = "oCVisFX_Lightning";

void PatchChainLightning(void)
{
	// New plan - just replace the unused MultiTarget class

	// Fix class name and sizeof in oCVisFX_MultiTarget::classDef constructor
	Patch(0x0049F62A + 1, oCVisFX_Lightning); // sub_49F580()
	Patch(0x0049F635 + 1, oCVisFX_Lightning); // sub_49F580()
	Patch(0x0049F64E + 1, sizeof(hCVisFX_Lightning)); // sub_49F580()

	// Fix stuff in MultiTarget CreateNewInstance
	Patch(0x0049F768 + 1, sizeof(hCVisFX_Lightning));
	InjectHook(0x0049F797, hCVisFX_Lightning_Constructor_Hook, PATCH_JUMP); // oCVisFX_MultiTarget::_CreateNewInstance()

	// Fix stuff in MultiTarget destructor
	InjectHook(0x0049EBF5, hCVisFX_Lightning_Destructor_Hook, PATCH_JUMP); // oCVisFX_MultiTarget::~oCVisFX_MultiTarget()
	PatchJump(0x0049ECC7, 0x0049ECD9); // oCVisFX_MultiTarget::~oCVisFX_MultiTarget()

	// Overwrite some virtuals for hCVisFX_Lightning
	Patch(0x00830560, &hCVisFX_Lightning::_OnTick); // oCVisFX_MultiTarget::`vftable'
	Patch(0x008305A4, &hCVisFX_Lightning::_Open); // oCVisFX_MultiTarget::`vftable'
	Patch(0x008305C4, &hCVisFX_Lightning::_Init_3); // oCVisFX_MultiTarget::`vftable'
	Patch(0x008305D0, &hCVisFX_Lightning::_InvestNext); // oCVisFX_MultiTarget::`vftable'
	Patch(0x008305DC, &hCVisFX_Lightning::_Cast); // oCVisFX_MultiTarget::`vftable'
	Patch(0x008305E0, &hCVisFX_Lightning::_Stop); // oCVisFX_MultiTarget::`vftable'

	// And set the remaining ones to oCVisualFX functions
	Patch(0x00830538, 0x00499B40); // oCVisFX_MultiTarget::`vftable', oCVisualFX::Archive()
	Patch(0x0083053C, 0x00499B50); // oCVisFX_MultiTarget::`vftable', oCVisualFX::Unarchive()
	Patch(0x008305E8, 0x0048A050); // oCVisFX_MultiTarget::`vftable', oCVisualFX::Play()
	Patch(0x008305F0, 0x004942F0); // oCVisFX_MultiTarget::`vftable', oCVisualFX::IsFinished()
	Patch(0x008305F8, 0x0048D4B0); // oCVisFX_MultiTarget::`vftable', oCVisualFX::SetByScript()
	Patch(0x00830600, 0x00491C20); // oCVisFX_MultiTarget::`vftable', oCVisualFX::Reset()
	Patch(0x00830620, 0x0048A120); // oCVisFX_MultiTarget::`vftable', oCVisualFX::SetDamage()
	Patch(0x00830624, 0x0048A130); // oCVisFX_MultiTarget::`vftable', oCVisualFX::SetDamageType()
	Patch(0x0083066C, 0x007D0F3E); // oCVisFX_MultiTarget::`vftable', __purecall
	Patch(0x00830670, 0x007D0F3E); // oCVisFX_MultiTarget::`vftable', __purecall

	// oCSpell::CreateEffect()
	// For some reason the game compiler did something really weird here and just kinda expects certain registers are intact after returning
	// Modern VS creates the function differently, so we have to push and pop the register before and after the function call via asm
	InjectHook(0x00484729, oCSpell_InitByScript_Hook, PATCH_JUMP); // oCSpell::InitByScript()
	InjectHook(0x0048710F, oCSpell_EndTimedEffect_Hook, PATCH_JUMP); // oCSpell::EndTimedEffect()
}

void hCNpc::OnDamage_Events(oSDamageDescriptor &descDamage)
{
	this->oCNpc::OnDamage_Events(descDamage);

	hCSpell *spell = NULL;
	oCNpc *npcTarget = NULL;
	oCNpc *npcAttacker = descDamage.pNpcAttacker;

	if (npcAttacker)
	{
		spell = zSTATIC_CAST<hCSpell>(npcAttacker->IsSpellActive(SPL_CONTROL));
		npcTarget = spell ? spell->spellTargetNpc : NULL;
	}

	if (spell && npcTarget && this->IsDead())
	{
		spell->EndTimedEffect();
	}

	if (this->HasBodyStateModifier(BS_MOD_CONTROLLING))
	{
		spell = zSTATIC_CAST<hCSpell>(npcAttacker->IsSpellActive(SPL_CONTROL));

		if (spell)
		{
			spell->EndTimedEffect();
		}
	}
}

bool bTelekinesisManaTaken = FALSE;

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

		// Hacks
		if (!bTelekinesisManaTaken)
		{
			bTelekinesisManaTaken = TRUE;

			this->spellCasterNpc->ChangeAttribute(this->spellEnergy, -1);
		}

		zVEC3 move, add, right, tmp, pos, curPos;
		float fac;
		int inMove;

		float speed = 75.0f / 1000.0f;

		move[0] = 0.0f;
		move[1] = 0.0f;
		move[2] = 0.0f;

		add = this->spellTarget->GetPositionWorld() - this->spellCasterNpc->GetPositionWorld();
		add[1] = 0.0f;
		add.NormalizeApprox();

		right[0] = 0.0f;
		right[1] = 1.0f;
		right[2] = 0.0f;
		right = right ^ add;

		if (zinput->GetState(GAME_UP))
		{
			if ((this->spellTarget->GetPositionWorld()
				- this->spellCasterNpc->GetPositionWorld()).LengthApprox() > 200.0f)
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
			tmp[0] = 0.0f;
			tmp[1] = 1.0f;
			tmp[2] = 0.0f;

			move += tmp;
			up += fac;
		}

		pos = this->spellTarget->GetPositionWorld();
		pos += move * fac;

		if (this->up >= 150.0f)
		{
			this->hoverY += this->hoverDir * ztimer.frameTimeFloat * speed / 4.0f;

			if (this->hoverY <= -5.0f || this->hoverY >= 5.0f)
			{
				this->hoverDir = -this->hoverDir;
			}

			move[1] += this->hoverY - this->hoverOld;
			this->hoverOld = this->hoverY;
		}

		curPos = pos;
		curPos += move;

		inMove = (this->spellCasterNpc->isInMovementMode != 0);

		if (inMove)
		{
			this->spellCasterNpc->EndMovement(TRUE); // TODO possibly FALSE
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
}

bool hCSpell::IsInvestSpell()
{
	switch (this->spellID)
	{
	case SPL_PYROKINESIS:
	case SPL_CHAINLIGHTNING:
	case SPL_HEAL:
	// case SPL_TELEKINESIS:
	case SPL_TELEKINESIS2:
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
			zRELEASE(this->saveNpc);

			this->saveNpc = this->spellCasterNpc;
			this->saveNpc->AddRef();

			this->spellCasterNpc->ModifyBodyState(BS_MOD_CONTROLLING, 0);

			this->spellTargetNpc->anictrl->SearchStandAni(0);
			this->spellTargetNpc->SetAsPlayer();
			this->spellTargetNpc->SetBodyStateModifier(BS_MOD_CONTROLLED);

			oCMsgWeapon *msgWeapon = oCMsgWeapon::_CreateNewInstance();
			msgWeapon->subType = oCMsgWeapon::EV_REMOVEWEAPON;

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

				zRELEASE(this->controlWarnFX);

				this->effect = this->CreateEffect();
				this->effect->Init(this->saveNpc, NULL, NULL);
				this->effect->SetSpellTargetTypes(this->targetCollectType);
				this->effect->Cast(TRUE);

				zRELEASE(this->effect);
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
					this->controlWarnFX = oCVisualFX::CreateAndPlay(zSTRING("CONTROL_LEAVERANGEFX"), this->spellTargetNpc->GetPositionWorld(), NULL, 1, 0, 0, FALSE);
				}
			}
			else if (this->controlWarnFX)
			{
				this->controlWarnFX->Stop(TRUE);

				zRELEASE(this->controlWarnFX);
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
			if (!zDYNAMIC_CAST<oCItem>(v) && !zDYNAMIC_CAST<oCMOB>(v) ||
				(zDYNAMIC_CAST<oCMOB>(v) && !(zSTATIC_CAST<oCMOB>(v)->IsMoveable()))) // do we need to allow MOBs to be moved?
			{
				this->spellStatus = SPL_STATUS_DONTINVEST;

				return FALSE;
			}
		}
		if (this->spellID == SPL_TELEKINESIS2) // TODO: Check for oCNpc, what is IsMovable()
		{
			if (!zDYNAMIC_CAST<oCNpc>(v))
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
			// Hacks
			bTelekinesisManaTaken = FALSE;

			zVEC3 positionWorld = vob->GetPositionWorld();
			zVEC3 groundVec(0.0f, -(positionWorld[1] - vob->bbox3D.mins[1]) - 1.0f, 0.0f);

			if (!vob->homeWorld->TraceRayNearestHit(positionWorld, groundVec, vob, zTRACERAY_VOB_IGNORE_NO_CD_DYN | zTRACERAY_VOB_IGNORE_CHARACTER))
			{
				zVEC3 velocity(0.0f, -100.0f, 0.0f);

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

	this->collDetectionStatic = en && this->emActionCollStat != TACTION_COLL_NONE;
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

void hCSpell::InitValues(int _spellID)
{
	this->spellID = _spellID;
	this->spellCaster = NULL;
	this->spellCasterNpc = NULL;
	this->spellTarget = NULL;
	this->spellTargetNpc = NULL;
	this->saveNpc = NULL;
	this->effect = NULL;

	this->manaTimer = 0.0f;
	this->manaInvested = 0;
	this->spellLevel = 0;
	this->manaInvestTime = 2000.0f;
	this->damageType = oEDamageType_Edge;
	this->damagePerLevel = 0;
	this->spellStatus = SPL_STATUS_CANINVEST;
	this->spellEnabled = FALSE;
	this->spellInitDone = FALSE;
	this->spellEnergy = NPC_ATR_MANA;
	this->controlWarnFX = NULL;

	this->canBeDeleted = TRUE;
	this->timerEffect = FALSE;

	this->up = 0.0f;
	this->hoverY = 0.0f;
	this->hoverOld = 0.0f;
	this->hoverDir = 1.0f;
	this->canChangeTargetDuringInvest = TRUE;
	this->canTurnDuringInvest = TRUE;
	this->isMultiEffect = FALSE;
	this->targetCollectAlgo = TARGET_COLLECT_FOCUS;
	this->targetCollectType = TARGET_FLAG_ALL;
	this->targetCollectRange = 10000;
	this->targetCollectAzi = 180;
	this->targetCollectElev = 90;

	this->InitByScript(this->GetSpellInstanceName(_spellID));

	this->keyNo = 32565;
}

void PatchSpells(void)
{
	// Patch necessary functions for Telekinesis and Control
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

	// Remove unneeded (?) collsion enabling in oCVisualFX::InitEffect()
	PatchJump(0x00494B56, 0x00494BAC); // oCVisualFX::InitEffect()

	// Init effect level to 0 ...
	InjectHook(0x00489EFD, &hCVisualFX::InitValues); // oCVisualFX::oCVisualFX()

	// InitValues for spell ...
	InjectHook(0x00483D71, &hCSpell::InitValues); // oCSpell::oCSpell()
	InjectHook(0x00483F26, &hCSpell::InitValues); // oCSpell::oCSpell()
	InjectHook(0x0048768A, &hCSpell::InitValues); // oCSpell::_CreateNewInstance()

	// InitInvestFX level check
	InjectHook(0x00492A97, &hCVisualFX::InitInvestFX); // oCVisualFX::InvestNext()
	InjectHook(0x00492F1A, &hCVisualFX::InitInvestFX); // oCVisualFX::SetLevel()

	// Restore oCVisFX_Lightning
	if (G12GetPrivateProfileBool("ChainLightning", FALSE))
	{
		PatchChainLightning();
	}
}

void PatchGothic2(void)
{
	if (G12GetPrivateProfileBool("G1Spells", FALSE))
	{
		// An attempt at reintroducing the hardcoded spells from Gothic 1 (Telekinesis, Control, ChainLightning)
		// TODO SPL_TRANSFORM transforms into copy of NPC in focus ... see oCSpell::CastSpecificSpell()
		// TODO Telekinesis scroll doesn't get taken
		PatchSpells();
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
