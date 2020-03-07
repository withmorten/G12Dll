#include "..\G12Dll\G12Core.h"

#define G12DLL_NAME "G1Spells"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G1.hpp"
#include "..\G12Dll\G1.h"
#include "g1spells.hpp"

const float SCANNER_DIM_Y = 100.0f;
const float SCANNER_DIM_X = 15.0f;
const float AUTODISPOSE_TIME_LIGHTNING = 0.1f;
// const float AUTODISPOSE_TIME_LIGHTNING = 1.0f;
const float AUTODISPOSE_TIME_LIGHTNING_INV = 1.0f / AUTODISPOSE_TIME_LIGHTNING;

bool32 hCFXScanner::Initialized()
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
		zCDecal *dc = (zCDecal *)this->scannerVobs[i]->visual;

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
		zCDecal *dc = (zCDecal *)this->scannerVobs[i]->visual;

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

		zCDecal *dc = (zCDecal *)scannerVob->visual;

		if (dc) dc->SetDecalDim(SCANNER_DIM_Y, SCANNER_DIM_X);

		scannerVob->visualCamAlign = zVISUAL_CAMALIGN_NONE;
		scannerVob->UpdateVisualDependencies();
		scannerVob->SetTrafo(trafo);
	}
}

bool32 hCVisFX_Lightning::CheckDeletion()
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

bool32 hCVisFX_Lightning::UpdateBurnVobsInvestNext()
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

			zCDecal *dc = (zCDecal *)decalVob->visual;

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

		zCDecal *dc = zSTATIC_CAST<zCDecal>(this->decalVobs[i]->visual); // perhaps actually use the visual here

		if (dc)
		{
			dc->SetDecalDim(head.Length() * 0.5f, head.Length() * 0.2f);
		}

		this->decalVobs[i]->visualCamAlign = zVISUAL_CAMALIGN_NONE;
		this->decalVobs[i]->UpdateVisualDependencies();
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

				zCDecal *dc = (zCDecal *)scannerVob->visual;

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

				this->EndMovement(); // TODO possibly FALSE

				this->origin->GetEM(FALSE)->OnDamage(this, this->origin, this->damage, this->damageType, this->origin->GetPositionWorld());

				this->BeginMovement();
			}
			else if (!this->UpdateBurnVobsInvestNext())
			{
				this->EndMovement(); // TODO possibly FALSE

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

			this->EndMovement(); // TODO possibly FALSE

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

	this->EndMovement(); // TODO possibly FALSE
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
		this->SetOrigin(NULL);

		for (int nr = 1; nr < this->vobList.numInArray; nr++)
		{
			if (showTarget && nr >= 1) this->vobList[nr]->drawBBox3D = FALSE;

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

void hCVisFX_Lightning::Cast(bool32 killAfterDone)
{
	this->oCVisualFX::Stop(killAfterDone);
}

void hCVisFX_Lightning::Stop(bool32 killAfterDone)
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

void PatchGothic(void)
{
	if (G12GetPrivateProfileBool("ChainLightning", FALSE))
	{
		Patch(0x007D2378, &hCVisFX_Lightning::_OnTick); // oCVisFX_Lightning::`vftable'
		Patch(0x007D23BC, &hCVisFX_Lightning::_Open); // oCVisFX_Lightning::`vftable'
		Patch(0x007D23DC, &hCVisFX_Lightning::_Init); // oCVisFX_Lightning::`vftable'
		Patch(0x007D23E8, &hCVisFX_Lightning::_InvestNext); // oCVisFX_Lightning::`vftable'
		Patch(0x007D23F4, &hCVisFX_Lightning::_Cast); // oCVisFX_Lightning::`vftable'
		Patch(0x007D23F8, &hCVisFX_Lightning::_Stop); // oCVisFX_Lightning::`vftable'
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
