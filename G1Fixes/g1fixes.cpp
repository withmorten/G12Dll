#include "..\G12Dll\G12Core.h"

#define G12DLL_NAME "G1Fixes"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G1.hpp"
#include "..\G12Dll\G1.h"
#include "g1fixes.hpp"

const float SCANNER_DIM_Y = 100.0f;
const float SCANNER_DIM_X = 15.0f;
const float AUTODISPOSE_TIME_LIGHTNING = 0.1f;
// const float AUTODISPOSE_TIME_LIGHTNING = 1.0f;
const float AUTODISPOSE_TIME_LIGHTNING_INV = 1.0f / AUTODISPOSE_TIME_LIGHTNING;

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
		zVEC3 up = right ^ ((zCCamera::activeCam->connectedVob->GetPositionWorld() - newPos).Normalize());
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

bool hCVisFX_Lightning::CheckDeletion()
{
	printf("hCVisFX_Lightning::CheckDeletion()\n");

	if (this->shouldDelete)
	{
		this->ai->delete_it = TRUE;

		printf("hCVisFX_Lightning::CheckDeletion(): should delete\n");

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
		printf("hCVisFX_Lightning::CheckDeletion(): isDeleted\n");

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
	printf("hCVisFX_Lightning::UpdateBurnVobs()\n");

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

			printf("upKey: %d\n", upKey);

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

	printf("numBurnVobs: %d\n", this->burnVobs.numInArray);
	printf("numBurnNodes: %d\n", this->burnNodes.numInArray);

	this->trajectory.SetByList(this->burnVobs);
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

void hCVisFX_Lightning::Open()
{
	printf("hCVisFX_Lightning::Open()\n");

	this->oCVisualFX::Open();

	if (this->light)
	{
		this->lightRange = this->light->lightData.range;

		this->light->SetRange(0.0f, TRUE);
	}
}

void hCVisFX_Lightning::Init(zCArray<zCVob *> &trajectoryVobs)
{
	printf("hCVisFX_Lightning::Init()\n");

	if (this->origin)
	{
		this->SetOrigin(NULL);

		for (int nr = 1; nr < this->vobList.numInArray; nr++)
		{
			if (showTarget) this->vobList[nr]->drawBBox3D = FALSE;

			if (this->vobList[nr])
			{
				this->vobList[nr]->Release();

				this->vobList[nr] = NULL;
			}
		}

		this->vobList.DeleteList();
	}

	this->oCVisualFX::Init(trajectoryVobs[0], NULL, NULL);

	this->vobList = trajectoryVobs;

	printf("hCVisFX_Lightning::Init(): %d trjVobs\n", this->vobList.numInArray);

	for (int nr = 1; nr < this->vobList.numInArray; nr++)
	{
		if (showTarget && nr == 1) this->vobList[1]->drawBBox3D = TRUE;

		this->vobList[nr]->AddRef();
	}

	zCModel *mdl = zDYNAMIC_CAST<zCModel>(this->origin->visual);

	if (mdl && !this->emTrjOriginNode_S.IsEmpty())
	{
		this->orgNode = mdl->SearchNode(this->emTrjOriginNode_S);
		this->targetNode = mdl->SearchNode(this->emTrjTargetNode_S);

		printf("hCVisFX_Lightning::Init(): Setting org and target node ...\n");
	}

	if (this->initialized && this->sfx)
	{
		this->sfxHnd = zsound->PlaySound3D(this->sfx, this, zSND_SLOT_NONE, NULL);

		printf("hCVisFX_Lightning::Init(): Already initialized but no sfx ...\n");
	}

	this->initialized = TRUE;
}

void hCVisFX_Lightning::InvestNext()
{
	printf("hCVisFX_Lightning::InvestNext()\n");

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

		this->decalVobs[i]->Release();
	}

	this->decalVobs.DeleteList(); // GEngine
	this->DeleteScanner(); // GEngine

	this->oCVisualFX::Stop(TRUE); // killAfterDone
}

void PatchGothic(void)
{
	if (G12GetPrivateProfileBool("ChainLightning", TRUE))
	{
		InjectHook(0x0049557A, &hCFXScanner::Initialized); // oCVisFX_Lightning::OnTick()
		InjectHook(0x00495585, &hCFXScanner::Disable); // oCVisFX_Lightning::OnTick()
		InjectHook(0x00495CFC, &hCFXScanner::Run); // oCVisFX_Lightning::OnTick()
		InjectHook(0x00496E80, &hCFXScanner::Run); // oCVisFX_Lightning::UpdateScanner()

		InjectHook(0x00495CE7, &hCVisFX_Lightning::CreateScanner); // oCVisFX_Lightning::OnTick()
		InjectHook(0x004953C5, &hCVisFX_Lightning::CheckDeletion); // oCVisFX_Lightning::OnTick()
		InjectHook(0x004953F8, &hCVisFX_Lightning::UpdateBurnVobs); // oCVisFX_Lightning::OnTick()

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
	if (GOTHIC108KM)
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
