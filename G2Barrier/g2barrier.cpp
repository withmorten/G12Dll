#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "..\G12Dll\MemoryMgr.h"

#define G12DLL_NAME "G2Barrier"

#include "..\G12Dll\G12.h"

#include "..\G12Dll\G2.hpp"
#include "..\G12Dll\G2.h"
#include "g2barrier.hpp"

#define WORLD_NAME "WORLD"

static char worldName[MAX_PATH] = { '\0' };

#define POINTS_WORLD 38

static zVEC2 newPointList[POINTS_WORLD];

float hMagFrontier::GetDistance(zVEC3 &pos, float &dist, zVEC3 &nearestPoint)
{
	float real_dist;
	zVEC2 pos2d, a, b, nearest;
	int showLineOfDeath = ogame->world->masterFrameCtr && oCNpc::godmode;
	dist = 10000000.0f;

	pos2d.n[0] = pos.n[0];
	pos2d.n[1] = pos.n[2];

	if (pos2d.n[0] == 0.0f && pos2d.n[1] == 0.0f)
	{
		return dist;
	}

	for (int x = 0; x < POINTS_WORLD - 1; x++)
	{
		a.n[0] = newPointList[x].n[0];
		a.n[1] = newPointList[x].n[1];

		b.n[0] = newPointList[x + 1].n[0];
		b.n[1] = newPointList[x + 1].n[1];

		GetNearestPointFromLineSegment2D(a, b, pos2d, nearest);

		if (showLineOfDeath)
		{
			DrawLineSegment2D(a, b, pos.n[1], (x & 1) ? GFX_BLUE : GFX_LBLUE);
		}

		real_dist = sqrtf((nearest.n[1] - pos2d.n[1]) * (nearest.n[1] - pos2d.n[1]) +
							(nearest.n[0] - pos2d.n[0]) * (nearest.n[0] - pos2d.n[0]));

		if (real_dist < dist)
		{
			dist = real_dist;
		}
	}

	return dist;
}

void hMagFrontier::DoCheck(void)
{
	this->oCMagFrontier::DoCheck();

	if (ogame && this->npc == oCNpc::player
		&& !zCCamera::activeCam->connectedVob->homeWorld->csPlayer->GetPlayingGlobalCutscene())
	{
		oCWorld *world = ogame->GetGameWorld();

		if (world)
		{
			if (!_strnicmp(world->worldName._Ptr, worldName, world->worldName._Len))
			{
				if (this->npc)
				{
					zVEC3 playerProjPos, startPoint;
					playerProjPos.n[0] = this->npc->trafoObjToWorld.v[0].n[3];
					playerProjPos.n[1] = this->npc->trafoObjToWorld.v[1].n[3];
					playerProjPos.n[2] = this->npc->trafoObjToWorld.v[2].n[3];

					float distToBarriere = 60000.0f;
					this->GetDistance(playerProjPos, distToBarriere, startPoint);

					if (distToBarriere > 1200.0f || distToBarriere < 0.0f)
					{
						this->DisposeShootFX();
						this->DisposeWarningFX();
					}
					else
					{
						int level = (int)((1200.0f - distToBarriere) / 55.0f);

						if (!level)
						{
							level = 1;
						}

						this->DoWarningFX(level);

						if (oCNpc::godmode || (distToBarriere > 650.0f || distToBarriere < 0.0f))
						{
							this->DisposeShootFX();
						}
						else
						{
							this->DoShootFX(startPoint);
						}
					}
				}
			}
		}
	}
}

void MagicFrontierNewPointListInit(void)
{
	// newPointList from Gothic 1
	newPointList [0].n[0] =  57939.2f;
	newPointList [0].n[1] =  1280.28f;
	newPointList [1].n[0] =  55954.4f;
	newPointList [1].n[1] =  5421.51f;
	newPointList [2].n[0] =  52856.8f;
	newPointList [2].n[1] =  10047.0f;
	newPointList [3].n[0] =  49451.9f;
	newPointList [3].n[1] =  14908.2f;
	newPointList [4].n[0] =  44199.8f;
	newPointList [4].n[1] =  20513.3f;
	newPointList [5].n[0] =  37684.2f;
	newPointList [5].n[1] =  26271.2f;
	newPointList [6].n[0] =  30434.0f;
	newPointList [6].n[1] =  31462.4f;
	newPointList [7].n[0] =  25573.6f;
	newPointList [7].n[1] =  32692.7f;
	newPointList [8].n[0] =  21248.3f;
	newPointList [8].n[1] =  35176.1f;
	newPointList [9].n[0] =  19450.7f;
	newPointList [9].n[1] =  35205.0f;
	newPointList[10].n[0] =  16263.1f;
	newPointList[10].n[1] =  32799.6f;
	newPointList[11].n[0] =  10755.6f;
	newPointList[11].n[1] =  34744.4f;
	newPointList[12].n[0] =   9736.9f;
	newPointList[12].n[1] =  37990.5f;
	newPointList[13].n[0] =   8218.6f;
	newPointList[13].n[1] =  38393.1f;
	newPointList[14].n[0] =   4065.0f;
	newPointList[14].n[1] =  39018.4f;
	newPointList[15].n[0] =    839.9f;
	newPointList[15].n[1] =  39079.3f;
	newPointList[16].n[0] =  -9312.9f;
	newPointList[16].n[1] =  38694.2f;
	newPointList[17].n[0] = -19258.3f;
	newPointList[17].n[1] =  40991.4f;
	newPointList[18].n[0] = -29684.1f;
	newPointList[18].n[1] =  40535.7f;
	newPointList[19].n[0] = -39313.7f;
	newPointList[19].n[1] =  36558.8f;
	newPointList[20].n[0] = -49319.6f;
	newPointList[20].n[1] =  31970.2f;
	newPointList[21].n[0] = -54137.3f;
	newPointList[21].n[1] =  26761.7f;
	newPointList[22].n[0] = -62089.3f;
	newPointList[22].n[1] =  21598.1f;
	newPointList[23].n[0] = -66193.7f;
	newPointList[23].n[1] =  12999.2f;
	newPointList[24].n[0] = -66132.3f;
	newPointList[24].n[1] =   6204.0f;
	newPointList[25].n[0] = -63855.2f;
	newPointList[25].n[1] =  -5700.8f;
	newPointList[26].n[0] = -59385.1f;
	newPointList[26].n[1] = -10081.5f;
	newPointList[27].n[0] = -56013.8f;
	newPointList[27].n[1] = -22393.4f;
	newPointList[28].n[0] = -47250.3f;
	newPointList[28].n[1] = -28502.0f;
	newPointList[29].n[0] = -37136.5f;
	newPointList[29].n[1] = -38319.2f;
	newPointList[30].n[0] = -24664.7f;
	newPointList[30].n[1] = -46687.9f;
	newPointList[31].n[0] =  -7860.6f;
	newPointList[31].n[1] = -48966.6f;
	newPointList[32].n[0] =   4876.6f;
	newPointList[32].n[1] = -49691.0f;
	newPointList[33].n[0] =  23147.8f;
	newPointList[33].n[1] = -47875.1f;
	newPointList[34].n[0] =  48722.3f;
	newPointList[34].n[1] = -39488.8f;
	newPointList[35].n[0] =  55902.4f;
	newPointList[35].n[1] = -31909.8f;
	newPointList[36].n[0] =  61238.6f;
	newPointList[36].n[1] = -23412.8f;
	newPointList[37].n[0] =  60230.1f;
	newPointList[37].n[1] =  -6641.9f;
}

void MagicFrontierNewPointListInitGRM(void)
{
	// newPointList from GRM
	newPointList[17].n[0] = -26219.2f;
	newPointList[17].n[1] =  40844.2f;
	newPointList[18].n[0] = -34576.0f;
	newPointList[18].n[1] =  43032.3f;
	newPointList[19].n[0] = -44458.8f;
	newPointList[19].n[1] =  43099.2f;
	newPointList[20].n[0] = -49763.7f;
	newPointList[20].n[1] =  37384.8f;
}

void PatchMagicFrontier(void)
{
	if (G12GetPrivateProfileBool("MagicFrontierEnable", FALSE))
	{
		if (!*worldName) G12GetPrivateProfileString("WorldName", WORLD_NAME, worldName, MAX_PATH);

		MagicFrontierNewPointListInit();

		InjectHook(0x0073E71B, &hMagFrontier::DoCheck); // oCNpc::ProcessNpc()
	}

	if (G12GetPrivateProfileBool("MagicFrontierPointsWorldGRM", FALSE))
	{
		// GRMFixes has some different points at the old mine pass ... just included here for now
		MagicFrontierNewPointListInitGRM();
	}
}

#define BARRIER_COLOR 0x00FFFFFF

const int barrierEverLoomingMinOpacity = 1;
const int barrierEverLoomingMaxOpacity = 15;

static int barrierMinOpacity = 0;
static int barrierMaxOpacity = 120;

static int meshLoaded = FALSE;
static int firstRender = TRUE;

static int isBarrierRender;

static int earthQuakeInterval;
static int earthQuakeTimer = 0;

static int ignoreSkyEffectsSetting;
static int alwaysVisible;
static int tremorEnable;
static int earthQuakeEnable;

static int activeThunder_Sector1 = FALSE;
static int activeThunder_Sector2 = FALSE;
static int activeThunder_Sector3 = FALSE;
static int activeThunder_Sector4 = FALSE;

static zCSoundFX *sfx1 = NULL;
static zTSoundHandle sfxHandle1 = 0;
static zCSoundFX *sfx2 = NULL;
static zTSoundHandle sfxHandle2 = 0;
static zCSoundFX *sfx3 = NULL;
static zTSoundHandle sfxHandle3 = 0;
static zCSoundFX *sfx4 = NULL;
static zTSoundHandle sfxHandle4 = 0;

static float nextActivation = 8000.0f;

static int showThunders = FALSE;
static float fadeTime;
static float timeUpdatedFade = 0.0f;
static float timeStepToUpdateFade = 1.0f;
static float timeToStayVisible;
static float timeToStayHidden;

static float delayTimeSector1 = 4000.0f;
static float delayTimeSector2 = 200.0f;
static float delayTimeSector3 = 6000.0f;
static float delayTimeSector4 = 10000.0f;

static myThunder *myThunderList = NULL;
static int numMyThunders;

void hBarrier::AddTremor(zTRenderContext &renderContext)
{
	zVEC3 amplitude;

	amplitude.n[0] = 0.8f;
	amplitude.n[1] = 1.5f;
	amplitude.n[2] = 0.8f;

	renderContext.cam->AddTremor(renderContext.cam->connectedVob->trafoObjToWorld.GetTranslation(), 4000.0f * 4000.0f, 3000.0f, amplitude);
}

void hBarrier::AddEarthQuake()
{
	oCVisualFX *vfx = oCVisualFX::CreateAndPlay(zSTRING("FX_EarthQuake"), oCNpc::player, oCNpc::player, 0, 0, 0, FALSE);

	if (vfx)
	{
		vfx->Release();

		vfx = NULL;
	}
}

void hBarrier::Init()
{
	this->originalTexUVList = 0;

	this->skySphereMesh = zCMesh::Load(zSTRING("Magicfrontier_Out.3ds"), TRUE);

	if (this->skySphereMesh)
	{
		int startPointsTemp[40];
		int numStartPointsTemp;
		int texUVListCtr = 0;

		zCMaterial *material = this->skySphereMesh->polyList[0]->material;

		material->SetTexture(zSTRING("BARRIERE.TGA"));
		material->rndAlphaBlendFunc = zRND_ALPHA_FUNC_ADD;

		int numTexUVs = this->skySphereMesh->numPoly * 3;
		this->originalTexUVList = new zVEC2[numTexUVs];

		float maxSkyY = this->skySphereMesh->bbox3D.maxs.n[1];
		float minSkyY = maxSkyY * 0.925f;

		for (int i = 0; i < this->skySphereMesh->numPoly; i++)
		{
			zCPolygon *poly = this->skySphereMesh->polyList[i];

			for (int j = 0; j < poly->polyNumVert; j++)
			{
				zVEC2 *originalTexUV = new zVEC2();

				zCVertFeature *feat = poly->feature[j];
				zCVertex *vert = poly->vertex[j];

				originalTexUV->n[0] = feat->texu;
				originalTexUV->n[1] = feat->texv;

				this->originalTexUVList[texUVListCtr++] = *originalTexUV;

				float vertY = vert->position.n[1];

				int alpha;

				if (vertY > minSkyY)
				{
					alpha = (int)(255.0f * (maxSkyY - vertY) / (maxSkyY - minSkyY));
				}
				else
				{
					alpha = (int)(255.0f * (vertY / 8000.0f));
				}

				if (alpha > 255)
				{
					alpha = 255;
				}

				if (alpha < 0)
				{
					alpha = 0;
				}

				feat->lightDyn.dword = BARRIER_COLOR;
				feat->lightDyn.alpha = alpha;

				feat->lightStat.dword = BARRIER_COLOR;
				feat->lightStat.alpha = alpha;
			}
		}

		this->numMyVerts = this->skySphereMesh->numVert;
		this->numMyPolys = this->skySphereMesh->numPoly;

		this->myVertList = new myVert[this->numMyVerts];
		memset(this->myVertList, 0x00, sizeof(myVert) * this->numMyVerts);

		int goOn = TRUE;

		for (int x = 0; x < this->numMyVerts; x++)
		{
			this->myVertList[x].vertIndex = x;

			for (int z = 0; z < this->numMyPolys; z++)
			{
				int save_index = -1;

				zCPolygon *poly = this->skySphereMesh->polyList[z];

				for (int f = 0; f < poly->polyNumVert; f++)
				{
					if (this->skySphereMesh->vertList[x] == poly->vertex[f])
					{
						save_index = f;
					}
				}

				int k = 0;

				if (save_index != -1)
				{
					for (int h = 0; h < poly->polyNumVert; h++)
					{
						if (h != save_index)
						{
							for (k = 0; k < this->skySphereMesh->numVert; k++)
							{
								if (this->skySphereMesh->vertList[k] == poly->vertex[h])
								{
									for (int n = 0; n < this->myVertList[x].numNeighbours; n++)
									{
										if (this->myVertList[x].vertNeighbours[n] == k)
										{
											goOn = FALSE;
										}
									}

									int addPolyIndex = TRUE;

									for (int xx = 0; xx < this->myVertList[x].numPolyIndices; xx++)
									{
										if (z == this->myVertList[x].polyIndices[xx])
										{
											addPolyIndex = FALSE;
										}
									}

									if (addPolyIndex)
									{
										this->myVertList[x].polyIndices[this->myVertList[x].numPolyIndices] = z;
										this->myVertList[x].numPolyIndices++;
									}

									if (goOn)
									{
										this->myVertList[x].vertNeighbours[this->myVertList[x].numNeighbours] = k;
										this->myVertList[x].numNeighbours++;
									}

									goOn = TRUE;
								}
							}
						}
					}
				}
			}
		}

		this->numStartPoints1 = 0;
		this->numStartPoints2 = 0;
		this->numStartPoints3 = 0;
		this->numStartPoints4 = 0;
		this->topestPoint = 0;

		float height2 = 35000.0f;
		int count = 0;

		for (int k = 0; k < this->numMyVerts; k++)
		{
			if (this->skySphereMesh->vertList[k]->position.n[1] > height2)
			{
				startPointsTemp[count] = k;
				count++;
			}
		}

		numStartPointsTemp = count;

		for (int l = 0; l < numStartPointsTemp; l++)
		{
			zCVertex *vec = this->skySphereMesh->vertList[startPointsTemp[l]];

			if (vec->position.n[0] <= -5008.4f && vec->position.n[2] < -5597.02f)
			{
				this->startPointList1[this->numStartPoints1] = startPointsTemp[l];
				this->numStartPoints1++;
			}

			if (vec->position.n[0] > -5008.4f && vec->position.n[2] <= -5597.02f)
			{
				this->startPointList2[this->numStartPoints2] = startPointsTemp[l];
				this->numStartPoints2++;
			}

			if (vec->position.n[0] > -5008.4f && vec->position.n[2] > -5597.02f)
			{
				this->startPointList3[this->numStartPoints3] = startPointsTemp[l];
				this->numStartPoints3++;
			}

			if (vec->position.n[0] < -5008.4f && vec->position.n[2] > -5597.02f)
			{
				this->startPointList4[this->numStartPoints4] = startPointsTemp[l];
				this->numStartPoints4++;
			}

			if (vec->position.n[0] == -5008.4f && vec->position.n[2] == -5597.02f)
			{
				this->topestPoint = startPointsTemp[l];
			}
		}

		material->color.alpha = this->fadeState;

		this->Initialise(20);

		// Init extra 20 thunders for barrier
		myThunder *myThunderList = this->myThunderList;
		int numMyThunders = this->numMyThunders;

		this->myThunderList = ::myThunderList;

		this->Initialise(20);

		::myThunderList = this->myThunderList;
		::numMyThunders = this->numMyThunders;

		this->myThunderList = myThunderList;
		this->numMyThunders = numMyThunders;
	}
}

int hBarrier::Render(zTRenderContext &rndContext, int fadeInOut, int alwaysVisible)
{
	if (this->skySphereMesh)
	{
		zCMaterial *material = this->skySphereMesh->polyList[0]->material;

		nextActivation -= ztimer.frameTimeFloat;

		if (nextActivation < 0)
		{
			nextActivation = timeToStayHidden + ((_rand() / RAND_MAX) * 5.0f * 60.0f * 1000.0f);
			this->bFadeInOut = TRUE;
		}

		if (this->bFadeInOut)
		{
			if (this->fadeIn)
			{
				if (!alwaysVisible || firstRender)
				{
					material->color.alpha = this->fadeState;
				}

				if (ztimer.totalTimeFloat - timeUpdatedFade > timeStepToUpdateFade)
				{
					this->fadeState++;

					timeUpdatedFade = ztimer.totalTimeFloat;
				}

				if (this->fadeState > barrierMaxOpacity)
				{
					if (firstRender)
					{
						firstRender = FALSE;
					}

					this->fadeState = barrierMaxOpacity;
					this->fadeIn = FALSE;
					showThunders = TRUE;
					fadeTime = ztimer.totalTimeFloat;
				}
			}
			else
			{
				if (ztimer.totalTimeFloat - fadeTime > timeToStayVisible)
				{
					this->fadeOut = TRUE;
					showThunders = FALSE;
					fadeTime = ztimer.totalTimeFloat;
				}

				if (this->fadeOut)
				{
					if (!alwaysVisible)
					{
						material->color.alpha = this->fadeState;
					}

					if (ztimer.totalTimeFloat - timeUpdatedFade > timeStepToUpdateFade)
					{
						this->fadeState--;

						timeUpdatedFade = ztimer.totalTimeFloat;
					}

					if (this->fadeState <= barrierMinOpacity)
					{
						// ever looming barrier
						if (barrierMinOpacity >= barrierEverLoomingMinOpacity)
						{
							// randomise it a bit
							barrierMinOpacity = barrierEverLoomingMinOpacity + _rand() % ((barrierEverLoomingMaxOpacity + 1) - barrierEverLoomingMinOpacity);
							this->fadeState = barrierMinOpacity;
						}
						else
						{
							this->fadeState = barrierMinOpacity;
						}

						this->fadeIn = TRUE;
						this->fadeOut = FALSE;
						this->bFadeInOut = FALSE;

						if (earthQuakeEnable)
						{
							if (earthQuakeTimer)
							{
								earthQuakeTimer--;
							}
							else
							{
								this->AddEarthQuake();
								earthQuakeTimer = earthQuakeInterval;
							}
						}
					}
				}
			}
		}

		int zBufferWriteEnabled;
		float zFarClip;

		if (this->fadeState > 0 || alwaysVisible)
		{
			zBufferWriteEnabled = zrenderer->GetZBufferWriteEnabled();
			zFarClip = zCCamera::activeCam->farClipZ;
			zrenderer->SetZBufferWriteEnabled(FALSE);
			zCCamera::activeCam->SetFarClipZ(2000000.0f);
			rndContext.cam->SetTransform(1, rndContext.cam->connectedVob->trafoObjToWorld.InverseLinTrafo());

			int addNewThunder = TRUE;

			this->RenderLayer(rndContext, 0, addNewThunder);
			this->RenderLayer(rndContext, 1, addNewThunder);

			zrenderer->FlushPolys();
		}

		if (showThunders)
		{
			zSTRING mfxBarriereAmbient("MFX_Barriere_Ambient");

			zTSound3DParams sound3DParams;
			sound3DParams.obstruction = 0.0f;
			sound3DParams.volume = 1.0f;
			sound3DParams.radius = -1.0f;
			sound3DParams.loopType = 0;
			sound3DParams.coneAngleDeg = 0.0f;
			sound3DParams.reverbLevel = 1.0f;
			sound3DParams.isAmbient3D = FALSE;
			sound3DParams.pitchOffset = -999999.0f;

			if (!::activeThunder_Sector1 && !zsound->IsSoundActive(::sfxHandle1) &&
				(ztimer.totalTimeFloat - delayTimeSector1) > 8000.f)
			{
				this->AddThunder(this->startPointList1[(this->numStartPoints1 - 1) & _rand()], 11, 0, 1);
				::activeThunder_Sector1 = TRUE;
				delayTimeSector1 = ztimer.totalTimeFloat;

				if (!::sfx1)
				{
					::sfx1 = zsound->LoadSoundFXScript(mfxBarriereAmbient);
				}

				if (tremorEnable)
				{
					this->AddTremor(rndContext);
				}

				::sfxHandle1 = zsound->PlaySound3D(::sfx1, zCCamera::activeCam->connectedVob, 0, &sound3DParams);
			}

			if (!::activeThunder_Sector2 && !zsound->IsSoundActive(::sfxHandle2) &&
				(ztimer.totalTimeFloat - delayTimeSector2) > 6000.f)
			{
				this->AddThunder(this->startPointList2[(this->numStartPoints2 - 1) & _rand()], 11, 0, 2);
				::activeThunder_Sector2 = TRUE;
				delayTimeSector2 = ztimer.totalTimeFloat;

				if (!::sfx2)
				{
					::sfx2 = zsound->LoadSoundFXScript(mfxBarriereAmbient);
				}

				if (tremorEnable)
				{
					this->AddTremor(rndContext);
				}

				::sfxHandle2 = zsound->PlaySound3D(::sfx2, zCCamera::activeCam->connectedVob, 0, &sound3DParams);
			}

			if (!::activeThunder_Sector3 && !zsound->IsSoundActive(::sfxHandle3) &&
				(ztimer.totalTimeFloat - delayTimeSector3) > 14000.f)
			{
				this->AddThunder(this->startPointList3[(this->numStartPoints3 - 1) & _rand()], 11, 0, 3);
				::activeThunder_Sector3 = TRUE;
				delayTimeSector3 = ztimer.totalTimeFloat;

				if (!::sfx3)
				{
					::sfx3 = zsound->LoadSoundFXScript(mfxBarriereAmbient);
				}

				if (tremorEnable)
				{
					this->AddTremor(rndContext);
				}

				::sfxHandle3 = zsound->PlaySound3D(::sfx3, zCCamera::activeCam->connectedVob, 0, &sound3DParams);
			}

			if (!::activeThunder_Sector4 && !zsound->IsSoundActive(::sfxHandle4) &&
				(ztimer.totalTimeFloat - delayTimeSector4) > 2000.f)
			{
				this->AddThunder(this->startPointList4[(this->numStartPoints4 - 1) & _rand()], 11, 0, 4);
				::activeThunder_Sector4 = TRUE;
				delayTimeSector4 = ztimer.totalTimeFloat;

				if (!::sfx4)
				{
					::sfx4 = zsound->LoadSoundFXScript(mfxBarriereAmbient);
				}

				if (tremorEnable)
				{
					this->AddTremor(rndContext);
				}

				::sfxHandle4 = zsound->PlaySound3D(::sfx4, zCCamera::activeCam->connectedVob, 0, &sound3DParams);
			}

			for (int x = 0; x < this->numMyThunders; x++)
			{
				this->RenderThunder(&this->myThunderList[x], rndContext);
			}

			zrenderer->FlushPolys();

			if (::sfx1)
			{
				zsound->UpdateSound3D(::sfxHandle1, NULL);
			}

			if (::sfx2)
			{
				zsound->UpdateSound3D(::sfxHandle2, NULL);
			}

			if (::sfx3)
			{
				zsound->UpdateSound3D(::sfxHandle3, NULL);
			}

			if (::sfx4)
			{
				zsound->UpdateSound3D(::sfxHandle4, NULL);
			}
		}

		if (this->fadeState > 0 || alwaysVisible)
		{
			zrenderer->SetZBufferWriteEnabled(zBufferWriteEnabled);
			zCCamera::activeCam->SetFarClipZ(zFarClip);
		}
	}

	return this->bFadeInOut;
}

int hBarrier::AddThunderSub(myThunder *rootThunder, int startIndex, int startNexIntex, int length, int mumSplits)
{
	int rV = this->oCBarrier::AddThunderSub(rootThunder, startIndex, startNexIntex, length, mumSplits);

	if (isBarrierRender)
	{
		myThunder *thunder = &rootThunder->childs[rootThunder->numChilds];

		thunder->polyStrip->camAlign = 1;
		thunder->polyStrip->alphaFadeSpeed = 0.5f;
		thunder->polyStrip->width = 3000.0f;
	}

	return rV;
}

int hBarrier::AddThunder(int startIndex, int length, float random, int sector)
{
	int thunderIndex = this->oCBarrier::AddThunder(startIndex, length, random, sector);

	myThunder *thunder = &this->myThunderList[thunderIndex];

	thunder->polyStrip->camAlign = 1;
	thunder->polyStrip->alphaFadeSpeed = 0.5f;
	thunder->polyStrip->width = 3000.0f;

	return thunderIndex;
}

int hBarrier::RenderThunder(myThunder *thunder, zTRenderContext &rndContext)
{
	if (thunder->valid)
	{
		thunder->t1 = ztimer.frameTimeFloat / 200.0f + thunder->t1;

		float f = 0.9f;

		if (thunder->t0 >= f)
		{
			thunder->t0 = f;
		}

		if (thunder->t1 >= f)
		{
			thunder->t1 = f;
		}

		if (thunder->polyStrip->numSeg <= 2)
		{
			thunder->t0 = f;
		}

		if (thunder->t0 >= f
			&& thunder->t1 >= f
			&& thunder->numChilds <= 0)
		{
			if (!thunder->isChild)
			{
				if (thunder->sector == 1)
				{
					::activeThunder_Sector1 = FALSE;
				}
				if (thunder->sector == 2)
				{
					::activeThunder_Sector2 = FALSE;
				}
				if (thunder->sector == 3)
				{
					::activeThunder_Sector3 = FALSE;
				}
				if (thunder->sector == 4)
				{
					::activeThunder_Sector4 = FALSE;
				}

				this->numMyThunders--;
			}

			this->RemoveThunder(thunder);
			thunder->valid = FALSE;

			return FALSE;
		}

		if (thunder->t0 != thunder->t1)
		{
			thunder->polyStrip->SetVisibleSegments(thunder->t0, thunder->t1);
			thunder->polyStrip->Render(rndContext);
		}

		for (int x = 0; x < thunder->numChilds; x++)
		{
			if (thunder->t1 >= thunder->startTime[x])
			{
				if (!this->RenderThunder(&thunder->childs[x], rndContext))
				{
					thunder->numChilds--;
				}
			}
		}

		if (!thunder->isChild)
		{
			zMAT4 mat, matTemp, matTemp2;

			matTemp = zMAT4::s_identity;
			mat = zMAT4::s_identity;
			mat = rndContext.cam->GetTransform(1);

			mat.v[0].n[2] *= -1.0f;
			mat.v[1].n[2] *= -1.0f;
			mat.v[2].n[2] *= -1.0f;

			mat.v[0].n[3] = thunder->originVec.n[0];
			mat.v[1].n[3] = thunder->originVec.n[1];
			mat.v[2].n[3] = thunder->originVec.n[2];

			matTemp2 = rndContext.cam->GetTransform(0);

			rndContext.cam->SetTransform(0, mat);

			this->thunderStartDecal->SetDecalDim(5000.0f, 5000.0f);
			this->thunderStartDecal->Render(rndContext);
			this->thunderStartDecal->SetDecalDim(4000.0f, 4000.0f);

			rndContext.cam->SetTransform(0, matTemp2);
		}

		return TRUE;
	}

	return FALSE;
}

void hSkyControler_Barrier::RenderSkyPre(void)
{
	this->zCSkyControler_Outdoor::RenderSkyPre();

	hBarrier *barrier = (hBarrier *)this->barrier;

	if (!meshLoaded)
	{
		meshLoaded = TRUE;

		barrier->bFadeInOut = TRUE;

		barrier->Init();
	}

	zTRenderContext rndContext;

	if (zCObject::CheckInheritance(&zCSkyControler_Outdoor::classDef, this->_GetClassDef()))
	{
		if (zCSkyControler::s_skyEffectsEnabled
			&& this->rainFX.outdoorRainFXWeight > 0.5f
			&& this->rainFX.renderLightning && this->m_enuWeather)
		{
			memset(&rndContext, 0x00, sizeof(zTRenderContext));
			rndContext.clipFlags = -1;
			rndContext.cam = zCCamera::activeCam;
			rndContext.world = zCCamera::activeCam->connectedVob->homeWorld;
			rndContext.vob = zCCamera::activeCam->connectedVob;

			isBarrierRender = FALSE;

			barrier->oCBarrier::Render(rndContext, 0, 0);
		}
	}

	if ((zCSkyControler::s_skyEffectsEnabled || ignoreSkyEffectsSetting)
		&& !_stricmp(ogame->GetGameWorld()->worldName._Ptr, worldName))
	{
		memset(&rndContext, 0x00, sizeof(zTRenderContext));
		rndContext.clipFlags = -1;
		rndContext.cam = zCCamera::activeCam;
		rndContext.world = zCCamera::activeCam->connectedVob->homeWorld;
		rndContext.vob = zCCamera::activeCam->connectedVob;

		isBarrierRender = TRUE;

		// Instead of rewriting AddThunder, AddThunderSub and RemoveThunder just swap whichever thunder is supposed to be worked on
		myThunder *myThunderList = barrier->myThunderList;
		int numMyThunders = barrier->numMyThunders;

		barrier->myThunderList = ::myThunderList;
		barrier->numMyThunders = ::numMyThunders;

		barrier->Render(rndContext, this->bFadeInOut, alwaysVisible);

		::myThunderList = barrier->myThunderList;
		::numMyThunders = barrier->numMyThunders;

		barrier->myThunderList = myThunderList;
		barrier->numMyThunders = numMyThunders;
	}
}

void PatchBarrier(void)
{
	if (G12GetPrivateProfileBool("BarrierEnable", FALSE))
	{
		G12GetPrivateProfileString("WorldName", WORLD_NAME, worldName, sizeof(worldName));

		alwaysVisible = G12GetPrivateProfileBool("BarrierAlwaysOn", FALSE);
		ignoreSkyEffectsSetting = G12GetPrivateProfileBool("BarrierIgnoreSkyEffectsSetting", FALSE);

		timeToStayVisible = (float)(G12GetPrivateProfileInt("BarrierTimeOn", 25) * 1000);

		timeToStayHidden = (float)(G12GetPrivateProfileInt("BarrierTimeOff", 1200) * 1000);

		// looks bad when walking, should probably not be enabled at all, creates a small tremor each barrier thunder
		tremorEnable = G12GetPrivateProfileBool("BarrierTremorEnable", FALSE);

		// creates an earthQuake (eines dieser Beben) each interval times the barrier vanishes
		earthQuakeEnable = G12GetPrivateProfileBool("BarrierEarthQuakeEnable", FALSE);
		earthQuakeInterval = G12GetPrivateProfileInt("BarrierEarthQuakeInterval", 20);

		// ever looming barrier (idea from "F a w k e s" here https://forum.worldofplayers.de/forum/threads/1541239-G1-Barrier-ever-looming-threat)
		if (G12GetPrivateProfileBool("BarrierEverLooming", FALSE))
		{
			alwaysVisible = FALSE; // set this to false for no clashing
			barrierMinOpacity = barrierEverLoomingMinOpacity;
		}

		// Use our own RenderSkyPre()
		Patch(0x0083C178, &hSkyControler_Barrier::RenderSkyPre); // oCSkyControler_Barrier::`vftable'

		// Use a slightly moddified AddThunderSub()
		InjectHook(0x006BB376, &hBarrier::AddThunderSub); // oCBarrier::AddThunder()
	}
}

void PatchGothic2(void)
{
	PatchBarrier();
	PatchMagicFrontier();
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
