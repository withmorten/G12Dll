#ifdef PlaySound
#undef PlaySound
#endif

#define zRND_ALPHA_FUNC_ADD 3

typedef int zTSoundHandle;
typedef int zTVobCharClass;
typedef int zTVobSleepingMode;
typedef int zTSndManMedium;

typedef int oTSndMaterial;

struct oTDirectionInfo;
struct oTVobList;
struct oTVobListNpcs;
struct oTVobListItems;

struct zTNode;
struct zTRndSimpleVertex;
struct zTRnd_Stats;
struct zTRndSurfaceDesc;
struct zTRnd_DeviceInfo;
struct zTRnd_VidModeInfo;
struct zTMaterial;

class zCLightMap;
class zCMaterial;
class zCTexture;
class zCBspSector;
class zCSoundFX;
class zCDecal;
class zCViewBase;
class zCWorld;
class zCBspLeaf;
class zCAIBase;
class zCRigidBody;
class zCEventManager;
class zCCollisionObjectDef;
class zCCollisionObject;
class zCOutdoorRainFX;
class zCCSManager;
class zCWaypoint;
class zCCSCutsceneContext;
class zCSession;
class zCWayNet;
class zCViewProgressBar;
class zCWorldPerFrameCallback;
class zCZone;
class zCCBspTree;
class zCBspNode;
class zCBspBase;
class zCVobLight;
class zCCSPlayer;
class zCBBox3DSorterBase;
class zCPolyStrip;
class zCParser;
class zCClassDef;
class zCArchiver;
class zCVob;
class zCCamera;
class zCPolygon;
class zCVertex;
class zCEventMessage;
class zCRoute;
class zCView;
class zCPlayerGroup;
class zCObject;
class zCAICamera;
class zCParticleFX;
class zCModel;
class zCModelAni;
class zCRnd_D3D;
class zCTextureConvert;
class zCRenderLight;
class zCRndAlphaSortObject;
class zCVertexBuffer;

class oCNpc;
class oCNpcTalent;
class oCVisualFX;
class oCAIVobMove;
class oCRtnEntry;
class oCItem;
class oCMag_Book;
class oCSpell;
class oCAskBox;
class oCInfo;
class oCNews;
class oCMission;
class oCMobInter;
class oCAIHuman;
class oCAniCtrl_Human;
class oCNpcMessage;
class oCVob;
class oCSavegameManager;
class oCViewStatusBar;
class oCGameInfo;
class oCWorldTimer;
class oCGuilds;
class oCInfoManager;
class oCNewsManager;
class oCSVMManger;
class oCTradeManager;
class oCPortalRoomManager;
class oCSpawnManager;

class zVEC2
{
public:
	float n[2];
};

class zVEC3
{
public:
	zVEC3 &operator +=(zVEC3 &v) { XCALL(0x004B60A0); }
	zVEC3 &operator -=(zVEC3 &v) { XCALL(0x00554A00); }
	friend zVEC3 operator -(zVEC3 &v);
	friend zVEC3 operator +(zVEC3 &v, float f);
	friend zVEC3 operator +(zVEC3 &a, zVEC3 &b);
	friend zVEC3 operator -(zVEC3 &v, float f);
	friend zVEC3 operator -(zVEC3 &a, zVEC3 &b);
	friend zVEC3 operator *(zVEC3 &v, float f);
	friend float operator *(zVEC3 &a, zVEC3 &b);
	friend zVEC3 operator ^(zVEC3 &a, zVEC3 &b);

	float LengthApprox() { XCALL(0x00490E10); }
	zVEC3 &Normalize() { XCALL(0x00490EA0); }
	zVEC3 &NormalizeApprox() { XCALL(0x0054E750); }

public:
	float n[3];
};

class zVEC4
{
public:
	float n[4];
};

class zMAT3
{
public:
	zVEC3 v[3];
};

class zMAT4
{
public:
	static zMAT4 &s_identity;

	zVEC4 v[4];

public:
	zVEC3 GetTranslation() { XCALL(0x00408EE0); }
	void GetTranslation(zVEC3 &t) { XCALL(0x00487C70); }
	zMAT4 InverseLinTrafo() { XCALL(0x00515340); }
};

template<class T> class zCArray
{
public:
	T *array;
	int numAlloc;
	int numInArray;

public:
	void RemoveIndex(int index);
};

template<class T> class zCArraySort : public zCArray<T>
{
public:
	int (*Compare) (T *ele1, T *ele2);
};

template<class T> class zCList
{
public:
	T *data;
	zCList *next;
};

template<class T> class zCListSort
{
public:
	int (*Compare) (T *ele1, T *ele2);
	T *data;
	zCListSort *next;
};

template<class T> class zCTree
{
public:
	zCTree *parent;
	zCTree *firstChild;
	zCTree *next;
	zCTree *prev;
	T *data;
};

template<class T, int SIZE> class zCMatrixStack
{
public:
	int pos;
	T stack[SIZE];
};

class zSTRING // : public std::string
{
public:
	UCHAR allocator;
	CHAR *_Ptr;
	INT _Len;
	INT _Res;

public:
	virtual ~zSTRING() { XCALL(0x00401160); };

	zSTRING() { XCALL(0x00402AF0); }
	zSTRING(char *pstring) { XCALL(0x004010C0); }
	void Clear() { XCALL(0x0059D010); }
	int Search(int startIndex, char *substr, unsigned int num) { XCALL(0x0046C920); }
	int Contains(char *substr) { return Search(0, substr, 1) != -1; }
};

class zCOLOR
{
public:
	union
	{
		struct
		{
			BYTE b;
			BYTE g;
			BYTE r;
			BYTE alpha;
		};

		DWORD dword;
	};

public:
	zCOLOR() { };
	zCOLOR(BYTE rr, BYTE gg, BYTE bb, BYTE aa) { r = rr; g = gg; b = bb; alpha = aa; };
	zCOLOR(DWORD color) { dword = color; }
};

struct zTPlane
{
	float distance;
	zVEC3 normal;
};

struct zTBBox3D
{
	zVEC3 mins;
	zVEC3 maxs;
};

struct zTBSphere3D
{
	zVEC3 center;
	float radius;
};

struct zTViewPortData
{
	int xmin;
	int ymin;
	int xdim;
	int ydim;
	float xminFloat;
	float yminFloat;
	float xmaxFloat;
	float ymaxFloat;
	float xdimFloat;
	float ydimFloat;
	float xdimFloatMinus1;
	float ydimFloatMinus1;
	float xcenter;
	float ycenter;
};

struct zTCamVertSimple
{
	float x;
	float y;
	float z;
	zVEC2 texuv;
	zCOLOR color;
};

struct zTRenderContext
{
	int clipFlags;
	zCVob *vob;
	zCWorld *world;
	zCCamera *cam;
	float distVobToCam;
	int visualCamAlign;
	int m_AniMode;
	float m_aniModeStrength;
	zCOLOR hintLightingHighlightColor;

	unsigned char hintLightingFullbright : 1;
	unsigned char hintLightingSwell : 1;
	unsigned char hintLightingHighlight : 1;
};

struct zTRainFX
{
	zCOutdoorRainFX *outdoorRainFX;
	int camLocationHint;
	float outdoorRainFXWeight;
	float soundVolume;
	float timerInsideSectorCantSeeOutside;
	float timeStartRain;
	float timeStopRain;
	int renderLightning;
	int m_bRaining;
	int m_iRainCtr;
};

struct zTTraceRayReport
{
	int foundHit;
	zCVob *foundVob;
	zCPolygon *foundPoly;
	zVEC3 foundIntersection;
	zVEC3 foundPolyNormal;
	zCVertex *foundVertex;
};

struct zTSound3DParams
{
	float obstruction;
	float volume;
	float radius;
	int loopType;
	float coneAngleDeg;
	float reverbLevel;
	int isAmbient3D;
	float pitchOffset;
};

struct myVert
{
	int vertIndex;
	int vertNeighbours[8];
	int numNeighbours;

	int polyIndices[50];
	int numPolyIndices;

	int active;
};

struct myThunder
{
	zVEC3 originVec;
	myThunder *childs;
	int numChilds;
	float startTime[5];
	zCPolyStrip *polyStrip;
	int numSegs;
	int valid;
	float t0;
	float t1;
	int numSplits;
	int dead;
	int isChild;
	int sector;
};

struct myPoly
{
	int Alpha;
};

class zCClassDef
{
public:
	zSTRING className;
	zSTRING baseClassName;
	zSTRING scriptClassName;
	zCClassDef *baseClassDef;
	zCObject *(*createNewInstance)(void);
	zCObject *(*createNewInstanceBackup)(void);
	unsigned int classFlags;
	unsigned int classSize;
	int numLivingObjects;
	int numCtorCalled;
	zCObject **hashtable;
	zCArray<zCObject *> objectList;
	unsigned short archiveVersion;
	unsigned short archiveVersionSum;
};

class zCObject
{
public:
	int refCtr;
	unsigned short hashIndex;
	zCObject *hashNext;
	zSTRING objectName;

public:
	static int CheckInheritance(zCClassDef *baseClass, zCClassDef *subClass) { XCALL(0x00476E30); }

	virtual zCClassDef *_GetClassDef() { XCALL(0x00401EC0); }

	int Release() { XCALL(0x0040C310); }
};

class zTBoxSortHandle
{
public:
	zCBBox3DSorterBase *mySorter;
	zTBBox3D bbox3D;

	int indexBegin[3];
	int indexEnd[3];

	zCArray<zCVob *> activeList;

public:
	virtual ~zTBoxSortHandle() { XCALL(0x0063B030); }
};

class zCVisual : public zCObject
{
public:
	zCVisual *nextLODVisual;
	zCVisual *prevLODVisual;
	float lodFarDistance;
	float lodNearFadeOutDistance;
};

class zCVertex
{
public:
	zVEC3 position;

	union
	{
		struct
		{
			int transformedIndex;
			int myIndex;
		};

		unsigned int hackData;
	};
};

class zCVertFeature
{
public:
	zVEC3 vertNormal;
	zCOLOR lightStat;
	zCOLOR lightDyn;
	float texu;
	float texv;
};

#pragma pack (push, 1)
struct TFlags
{
	unsigned char portalPoly : 2;
	unsigned char occluder : 1;
	unsigned char sectorPoly : 1;
	unsigned char mustRelight : 1;
	unsigned char portalIndoorOutdoor : 1;
	unsigned char ghostOccluder : 1;
	unsigned char noDynLightNear : 1;
	unsigned short sectorIndex : 16;
};
#pragma pack (pop)

class zCPolygon
{
public:
	zCVertex **vertex;

	int lastTimeDrawn;
	zTPlane polyPlane;
	zCMaterial *material;
	zCLightMap *lightmap;

	zCVertex **clipVert;
	zCVertFeature **clipFeat;
	int numClipVert;

	zCVertFeature **feature;
	unsigned char polyNumVert;

	TFlags flags;
};

class zCOBBox3D
{
public:
	zVEC3 center;
	zVEC3 axis[3];
	zVEC3 extent;
	zCList<zCOBBox3D> childs;
};

class zCMesh : public zCVisual
{
public:
	int numPoly;
	int numVert;
	int numFeat;

	zCVertex **vertList;
	zCPolygon **polyList;
	zCVertFeature **featList;

	zCVertex *vertArray;
	zCPolygon *polyArray;
	zCVertFeature *featArray;

	zTBBox3D bbox3D;
	zCOBBox3D obbox3D;

	zCMesh *next;
	zCMesh *prev;

	int masterFrameCtr;

	zSTRING meshName;
	int hasLightmaps;
	int m_bUsesAlphaTesting;

	int numVertAlloc;
	int numPolyAlloc;

public:
	static zCMesh *Load(zSTRING &meshFileName, int a_bDontConvertToNPolys) { XCALL(0x00567600); }

	int Render(zTRenderContext &renderContext, zCOLOR *vertexColor) { XCALL(0x0056B210); }
};

class zCTexAniCtrl
{
public:
	int aniChannel;
	float actFrame;
	float aniFPS;
	int frameCtr;
	int bOneShotAni;
};

class zCMaterial : public zCObject
{
public:
	zCArray<zCPolygon *> polyList;
	int polyListTimeStamp;

	zCTexture *texture;
	zCOLOR color;
	float smoothAngle;
	int matGroup;
	zCBspSector *bspSectorFront;
	zCBspSector *bspSectorBack;
	zCTexAniCtrl texAniCtrl;
	zSTRING *detailObjectVisualName;

	float kambient;
	float kdiffuse;

	int m_bEnvironmentalMappingStrength;

	unsigned char smooth : 1;
	unsigned char dontUseLightmaps : 1;
	unsigned char texAniMap : 1;
	unsigned char lodDontCollapse : 1;
	unsigned char noCollDet : 1;
	unsigned char forceOccluder : 1;
	unsigned char m_bEnvironmentalMapping : 1;
	unsigned char polyListNeedsSort : 1;
	unsigned char matUsage : 8;
	unsigned char libFlag : 8;
	int rndAlphaBlendFunc : 8;
	unsigned char m_bIgnoreSun : 1;

	int m_enuWaveMode;
	int m_enuWaveSpeed;
	float m_fWaveMaxAmplitude;
	float m_fWaveGridSize;

	zCTexture *detailTexture;
	float detailTextureScale;
	zVEC2 texAniMapDelta;

	zVEC2 default_mapping;
	zVEC2 texScale;

public:
	void SetTexture(zSTRING &texName) { XCALL(0x005649E0); }
};

class zCEventMessage : public zCObject
{
public:
	unsigned short subType;
	int inCutscene;
};

class oCNpcMessage : public zCEventMessage
{
public:
	zSTRING targetVobName;

	int highPriority : 1;
	int deleted : 1;
	int inUse : 1;
};

class oCMsgWeapon : public oCNpcMessage
{
public:
	int targetMode;
	int duringRun : 1;
	int initDone : 1;
	int firstTime : 1;
	int useFist : 1;
	int showMagicCircle : 1;

	int ani;

public:
	static oCMsgWeapon *_CreateNewInstance() { XCALL(0x007636E0); }
};

class zCEventManager
{
public:
	void OnMessage(zCEventMessage *message, zCVob *sourceVob) { XCALL(0x00786380); }
};

class zCRigidBody
{
public:
	float mass;
	float massInv;
	zMAT3 iBody;
	zMAT3 iBodyInv;

	zVEC3 xPos;
	zMAT3 RDir;
	zVEC3 PLinMom;
	zVEC3 LAngMom;

	zMAT3 iInv;
	zVEC3 v;
	zVEC3 omega;

	zVEC3 force;
	zVEC3 torque;

	float gravityScale;
	zVEC3 slideDir;
	float slideAngle;

	unsigned char gravityOn : 1;
	unsigned char collisionHad : 1;
	unsigned char mode : 1;
	unsigned char justSetSliding : 4;

public:
	void SetVelocity(zVEC3 &vel) { XCALL(0x005B66D0); }
};

class zCVob : public zCObject
{
public:
	zCTree<zCVob> *globalVobTreeNode;
	int lastTimeDrawn;
	int lastTimeCollected;

	zCArray<zCBspLeaf *> vobLeafList;

	zMAT4 trafoObjToWorld;
	zTBBox3D bbox3D;
	zTBSphere3D bsphere3D;

	zCArray<zCVob *> touchVobList;

	int type;
	int groundShadowSizePacked;
	zCWorld *homeWorld;
	zCPolygon *groundPoly;
	zCAIBase *callback_ai;
	zMAT4 *trafo;

	zCVisual *visual;
	float visualAlpha;
	float m_fVobFarClipZScale;
	int m_AniMode;
	float m_aniModeStrength;
	int m_zBias;

	zCRigidBody *rigidBody;

	zCOLOR lightColorStat;
	zCOLOR lightColorDyn;
	zVEC3 lightDirectionStat;
	zSTRING *vobPresetName;

	zCEventManager *eventManager;
	float nextOnTimer;

	unsigned char showVisual : 1;
	unsigned char drawBBox3D : 1;
	unsigned char visualAlphaEnabled : 1;
	unsigned char physicsEnabled : 1;

	unsigned char staticVob : 1;
	unsigned char ignoredByTraceRay : 1;
	unsigned char collDetectionStatic : 1;
	unsigned char collDetectionDynamic : 1;

	unsigned char castDynShadow : 2;
	unsigned char lightColorStatDirty : 1;
	unsigned char lightColorDynDirty : 1;

	int isInMovementMode : 2;
	unsigned char sleepingMode : 2;

	unsigned char mbHintTrafoLocalConst : 1;
	unsigned char mbInsideEndMovementMethod : 1;
	int visualCamAlign : 2;

	unsigned char collButNoMove : 4;

	unsigned char dontWriteIntoArchive : 1;
	unsigned char bIsInWater : 1;
	unsigned char bIsAmbientVob : 1;

	zCCollisionObjectDef *m_poCollisionObjectClass;
	zCCollisionObject *m_poCollisionObject;

public:
	zCEventManager *__fastcall GetEM(int dontCreate) { XCALL(0x005FFE10); }
	void SetSleeping(int sleep) { XCALL(0x00602930); }
	float GetDistanceToVob2(zCVob &v) { XCALL(0x0061BA40); }
	void SetPositionWorld(zVEC3 &posWorld) { XCALL(0x0061BB70); }
	void SetPhysicsEnabled(int enable) { XCALL(0x0061D190); }
	void BeginMovement() { XCALL(0x0061DA80); }
	void EndMovement(int a_bHintTrafoChanged) { XCALL(0x0061E0D0); }
};

class zCCamera
{
public:
	static zCCamera *&activeCam;

	zTPlane frustumplanes[6];
	BYTE signbits[6];

	zTViewPortData vpdata;
	zCViewBase *targetView;

	zMAT4 camMatrix;
	zMAT4 camMatrixInv;

	int tremorToggle;
	float tremorScale;
	zVEC3 tremorAmplitude;
	zVEC3 tremorOrigin;
	float tremorVelo;

	zMAT4 trafoView;
	zMAT4 trafoViewInv;
	zMAT4 trafoWOrld;

	zCMatrixStack<zMAT4, 8> trafoViewStack;
	zCMatrixStack<zMAT4, 8> trafoWorldStack;
	zCMatrixStack<zMAT4, 8> trafoWorldViewStack;
	zMAT4 trafoProjection;

	zTCamVertSimple polyCamVerts[4];
	zCPolygon *poly;
	zCMesh *polyMesh;
	zCMaterial *polyMaterial;

	int screenFadeEnabled;
	zCOLOR screenFadeColor;
	zSTRING screenFadeTexture;
	float screenFadeTextureAniFps;
	int screenFadeTextureBlendFunc;
	int cinemaScopeEnabled;
	zCOLOR cinemaScopeColor;

	int projection;
	int drawMode;
	int shadeMode;
	int drawWire;

	float farClipZ;
	float nearClipZ;
	float viewDistanceX;
	float viewDistanceY;
	float viewDistanceXInv;
	float viewDistanceYInv;
	int vobFarClipZ;
	float fovH;
	float fovV;
	zCVob *connectedVob;

	float topBottomSin;
	float topBottomCos;
	float leftRightSin;
	float leftRightCos;

public:
	void SetTransform(int trafoType, zMAT4 &trafo) { XCALL(0x0054A540); }
	zMAT4 &GetTransform(int trafoType) { XCALL(0x0054A6A0); }
	void SetFarClipZ(float z) { XCALL(0x0054B200); }
	void AddTremor(zVEC3 &posWorldSpace, float radiusSquare, float timeMsec, zVEC3 &amplitude) { XCALL(0x0054B660); }
};

class zCDecal : public zCVisual
{
public:
	zCMaterial *decalMaterial;

	float xdim;
	float ydim;
	float xoffset;
	float yoffset;
	int decal2Sided;
	int ignoreDayLight;
	int m_bOnTop;

public:
	void SetDecalDim(float xd, float yd) { XCALL(0x00556970); }
	int Render(zTRenderContext &renderContext) { XCALL(0x00556990); }
};

class zCPolyStrip : public zCVisual
{
public:
	zCMaterial *material;

	zCVertex *vertList;
	zCPolygon *polyList;

	int numPoly;
	int numVert;

	zVEC3 *centerPointList;
	float *alphaList;
	float width;
	zCVob *connectedVob;
	zTBBox3D bbox3D;
	int camAlign;
	int heightCheck;
	int everyFrameUpdate;
	float heightBound;

	int firstSeg;
	int lastSeg;
	int numSeg;

	float visLastFrac;
	float visFirstFrac;

	float alphaFadeSpeed;
	float newAlphaFadeSpeed;
	float newAlpha;
	int lastDirSeg;
	zVEC3 lastDirNormal;

	unsigned char localFOR : 1;

public:
	void SetVisibleSegments(float visibleFirst, float visibleLast) { XCALL(0x005BDB90); }
	int Render(zTRenderContext &renderContext) { XCALL(0x005BDC70); }
};

class oCBarrier
{
public:
	zCMesh *skySphereMesh;

	myPoly *myPolyList;
	myVert *myVertList;

	int numMyVerts;
	int numMyPolys;

	myThunder *myThunderList;
	int numMaxThunders;
	int numMyThunders;

	int actualIndex;
	int rootBoltIndex;

	int startPointList1[10];
	int numStartPoints1;
	int startPointList2[10];
	int numStartPoints2;
	int startPointList3[10];
	int numStartPoints3;
	int startPointList4[10];
	int numStartPoints4;

	int topestPoint;

	int bFadeInOut;
	int fadeState;

	int fadeIn;
	int fadeOut;
	int bFlashing;
	int fFlashTime;

	zCSoundFX *sfx1;
	zTSoundHandle sfxHandle1;
	zCSoundFX *sfx2;
	zTSoundHandle sfxHandle2;
	zCSoundFX *sfx3;
	zTSoundHandle sfxHandle3;
	zCSoundFX *sfx4;
	zTSoundHandle sfxHandle4;

	zCDecal *thunderStartDecal;

	int activeThunder_Sector1;
	int activeThunder_Sector2;
	int activeThunder_Sector3;
	int activeThunder_Sector4;

	zVEC2 *originalTexUVList;

public:
	void Init() { XCALL(0x006B9440); }
	void Initialise(int newNumMaxThunders) { XCALL(0x006B9BF0); }
	void AddTremor(zTRenderContext &renderContext) { XCALL(0x006B9CE0); }
	int RenderLayer(zTRenderContext &rndContext, int layerNumber, int &addNewThunder) { XCALL(0x006B9CF0); }
	int Render(zTRenderContext &rndContext, int fadeInOut, int alwaysVisible) { XCALL(0x006B9F30); }
	void RemoveThunder(myThunder *thunder) { XCALL(0x006BA9F0); }
	int AddThunderSub(myThunder *rootThunder, int startIndex, int startNexIntex, int length, int mumSplits) { XCALL(0x006BAAA0); }
	int AddThunder(int startIndex, int length, float random, int sector) { XCALL(0x006BADE0); }
	int RenderThunder(myThunder *thunder, zTRenderContext &rndContext) { XCALL(0x006BB4B0) }
};

class zCRnd_D3D
{
public:
	virtual ~zCRnd_D3D() { XCALL(0x0064A570); }
	virtual void BeginFrame() { XCALL(0x0064DD20); }
	virtual void EndFrame() { XCALL(0x0064DF20); }
	virtual void FlushPolys() { XCALL(0x0064DD10); }
	virtual void DrawPoly(zCPolygon *poly) { XCALL(0x0064B260); }
	virtual void DrawLightmapList(zCPolygon **polyList, int numPoly) { XCALL(0x0064D660); }
	virtual void DrawLine(float x1, float x2, float y1, float y2, zCOLOR col) { XCALL(0x0064D8E0); }
	virtual void DrawLineZ(float x1Scr, float y1Scr, float z1CamSpaceInv, float x2Scr, float y2Scr, float z2CamSpaceInv, zCOLOR col) { XCALL(0x0064DB00); }
	virtual void SetPixel(float x, float y, zCOLOR col) { XCALL(0x0064D720); }
	virtual void DrawPolySimple(zCTexture *texture, zTRndSimpleVertex *vertex, int numVert) { XCALL(0x0064AC30); }
	virtual void SetFog(int foggy) { XCALL(0x00651E80); }
	virtual int GetFog() { XCALL(0x00652070); }
	virtual void SetRadialFog(int foggy) { XCALL(0x00652010); }
	virtual int GetRadialFog() { XCALL(0x0064A400); }
	virtual void SetFogColor(zCOLOR &col) { XCALL(0x00652080); }
	virtual int GetFogColor() { XCALL(0x006521D0); }
	virtual void SetFogRange(float nearz, float farz, int falloff) { XCALL(0x006521E0); }
	virtual void GetFogRange(float &nearz, float &farz, int &falloff) { XCALL(0x006522D0); }
	virtual int GetPolyDrawMode() { XCALL(0x00652310); }
	virtual void SetPolyDrawMode(int &drawMode) { XCALL(0x00652300); }
	virtual int GetSurfaceLost() { XCALL(0x0064A470); }
	virtual void SetSurfaceLost(int b) { XCALL(0x0064A480); }
	virtual int GetSyncOnAmbientCol() { XCALL(0x0064A490); }
	virtual void SetSyncOnAmbientCol(int b) { XCALL(0x0064A4A0); }
	virtual void SetTextureWrapEnabled(int b) { XCALL(0x00652320); }
	virtual int GetTextureWrapEnabled() { XCALL(0x00652330); }
	virtual void SetBilerpFilterEnabled(int b) { XCALL(0x00652340); }
	virtual int GetBilerpFilterEnabled() { XCALL(0x006523D0); }
	virtual void SetDitherEnabled(int b) { XCALL(0x006523E0); }
	virtual int GetDitherEnabled() { XCALL(0x006523F0); }
	virtual int GetPolySortMode() { XCALL(0x006524D0); }
	virtual void SetPolySortMode(int &smode) { XCALL(0x00652400); }
	virtual int GetZBufferWriteEnabled() { XCALL(0x00652520); }
	virtual void SetZBufferWriteEnabled(int flag) { XCALL(0x006524E0); }
};

class zCSkyPlanet
{
public:
	zCMesh *mesh;
	zVEC4 color0;
	zVEC4 color1;
	float size;
	zVEC3 pos;
	zVEC3 rotAxis;
};

class zCSkyLayer
{
public:
	zCMesh *skyPolyMesh;
	zCPolygon *skyPoly;
	zVEC2 skyTexOffs;
	zCMesh *skyDomeMesh;
	int skyMode;
};

class zCSkyLayerData
{
public:
	int skyMode;
	zCTexture *tex;
	zSTRING texName;
	float texAlpha;
	float texScale;
	zVEC2 texSpeed;
};

class zCSkyState
{
public:
	float time;
	zVEC3 polyColor;
	zVEC3 fogColor;
	zVEC3 domeColor1;
	zVEC3 domeColor0;
	float fogDist;
	int sunOn;
	int cloudShadowOn;

	zCSkyLayerData layer[2];
};

class zCSkyControler : public zCObject
{
public:
	static int &s_skyEffectsEnabled;

	zCOLOR *polyLightCLUTPtr;
	float cloudShadowScale;
	int m_bColorChanged;
	int m_enuWeather;

	zCOLOR backgroundColor;
	int fillBackground;
	zCTexture *backgroundTexture;
	int relightCtr;
	float lastRelightTime;
	float m_fRelightTime;
};

class zCSkyControler_Mid : public zCSkyControler
{
public:
	int underwaterFX;
	zCOLOR underwaterColor;
	float underwaterFarZ;

	float underwaterStartTime;
	float oldFovX;
	float oldFovY;

	zCVob *underwaterPFX;

	zCPolygon *scrPoly;
	zCMesh *scrPolyMesh;
	int scrPolyAlpha;
	zCOLOR scrPolyColor;
	int scrPolyAlphaFunc;
};

class zCSkyControler_Outdoor : public zCSkyControler_Mid
{
public:
	static zCClassDef &classDef;

	int initDone;
	float masterTime;
	float masterTimeLast;

	zCSkyState masterState;
	zCSkyState *state0;
	zCSkyState *state1;

	zCArray<zCSkyState *> stateList;

	zCOLOR polyLightClut[256];

	float dayCounter;

	zCArray<zVEC3> fogColorDayVariations;
	zCArray<zVEC3> fogColorDayVariations2;

	float m_fSkyScale;
	int m_bSkyScaleChanged;
	zVEC3 m_overrideColor;
	int m_bOverrideColorFlag;
	int m_bDontRain;
	int m_bLevelChanged;
	int m_bDarkSky;

	float resultFogScale;
	float heightFogMinY;
	float heightFogMaxY;
	float userFogFar;
	float resultFogNear;
	float resultFogFar;
	float resultFogSkyNear;
	float resultFogSkyFar;
	zCOLOR resultFogColor;
	zCOLOR resultFogColorOverride;
	float userFarZScalability;

	zCSkyState *skyLaterState[2];
	zCSkyLayer skyLayer[2];
	zCSkyLayer skyLayerRainClouds;
	zCTexture *skyCloudLayerTex;

	zCSkyPlanet planets[2];
	int m_bSunVisible;
	float m_fFadeScale;

	zCVob *vobSkyPfx;
	float skyPFXTimer;

	int m_bIsMainControler;
	zVEC3 m_bWindVec;

	zTRainFX rainFX;

public:
	void ReadFogColorsFromINI() { XCALL(0x005E7EA0); }
	void RenderSkyPre() { XCALL(0x005EA850); }
};

class oCSkyControler_Barrier : public zCSkyControler_Outdoor
{
public:
	oCBarrier *barrier;
	int bFadeInOut;

public:
	void RenderSkyPre() { XCALL(0x006BB8D0); }
};

class zCBspTree
{
public:
	zCBspNode *actNodePtr;
	zCBspLeaf *actLeafPtr;

	zCBspBase *bspRoot;
	zCMesh *mesh;
	zCPolygon **treePolyList;
	zCBspNode *nodeList;
	zCBspLeaf *leafList;
	int numNodes;
	int numLeafs;
	int numPolys;

	zCArray<zCVob *> renderVobList;
	zCArray<zCVobLight *> renderLightList;
	zCArray<zCBspSector *> sectorList;
	zCArray<zCPolygon *> portalList;

	int bspTreeMode;
	int worldRenderMode;

	float vobFarClipZ;
	zTPlane vobFarPlane;
	int vobFarPlaneSignbits;
	int drawVobBBox3D;
	int leafsRendered;
	int vobsRendered;
	int m_bRenderedFirstTime;
	int masterFrameCtr;
	zCPolygon **actPolyPtr;
	int compiled;
};

class zCVobBBox3DSorter
{
public:
	zCArray<zTBoxSortHandle *> handles;
	zCArraySort<zTNode *> nodeList[3];
	int sorted;

public:
	virtual ~zCVobBBox3DSorter(){ XCALL(0x006288D0); }
};

class zCWorld : public zCObject
{
public:
	zCTree<zCVob> globalVobTree;

	zTTraceRayReport traceRayReport;

	zCSession *ownerSession;
	zCCSPlayer *csPlayer;

	zSTRING m_strlevelName;
	int compiled;
	int compiledEditorMode;
	int traceRayIgnoreVobFlag;
	int m_bIsInventoryWorld;
	int worldRenderMode;
	zCWayNet *wayNet;
	int masterFrameCtr;
	float vobFarClipZ;
	float vobFarClipZScalability;

	zCArray<zCVob *> traceRayVobList;
	zCArray<zCVob *> traceRayTempIgnoreVobList;

	int renderingFirstTime;
	int showWaynet;
	int showTraceRayLines;

	zCViewProgressBar *progressBar;
	int unarchiveFileLen;
	int unarchiveStartPosVobtree;
	int numVobsInWorld;

	zCArray<zCWorldPerFrameCallback *> perFrameCallbackList;

	zCSkyControler *skyControlerIndoor;
	zCSkyControler *skyControlerOutdoor;
	zCSkyControler *activeSkyControler;

	zCArray<zCZone *> zoneGlobalList;
	zCArraySort<zCZone *> zoneActiveList;
	zCArraySort<zCZone *> zoneLastClassList;

	zCVobBBox3DSorter zoneBoxSorter;
	zTBoxSortHandle zoneActiveHandle;

	int addZonesToWorld;
	int showZonesDebugInfo;

	zCCBspTree *cbspTree;
	zCBspTree bspTree;

	zCArray<zCVob *> activeVobList;
	zCArray<zCVob *> walkList;

	zCArray<zCVob *> vobHashTable[2048];

public:
	int __fastcall TraceRayNearestHit(zVEC3 &rayOrigin, zVEC3 &ray, zCVob *ignoreVob, int traceFlags) { XCALL(0x00621FA0); }
};

class zCCSPlayer
{
public:
	zCCSCutsceneContext *GetPlayingGlobalCutscene() { XCALL(0x00420770); }
};

class zCSndSys_MSS
{
public:
	virtual ~zCSndSys_MSS() { XCALL(0x004EB570); }
	virtual zCSoundFX *LoadSoundFX(zSTRING &fileName) { XCALL(0x004ED960); }
	virtual zCSoundFX *LoadSoundFXScript(zSTRING &scriptIdentifier) { XCALL(0x004EE120); }
	virtual zCParser *GetSFXParser() { XCALL(0x004F4270); }
	virtual float GetPlayingTimeMSEC(zSTRING &fileName) { XCALL(0x004F40E0); }
	virtual void SetSound3DDefaultRadius(float defRad) { XCALL(0x004EB3A0); }
	virtual float GetSound3DDefaultRadius() { XCALL(0x004EB3B0); }
	virtual void SetMasterVolume(float vol) { XCALL(0x004ED8E0); }
	virtual float GetMasterVolume() { XCALL(0x004ED730); }
	virtual zTSoundHandle PlaySound(zCSoundFX *sfx, int slot) { XCALL(0x004EF7B0); }
	virtual zTSoundHandle PlaySound(zCSoundFX *sfx, int slot, int freq, float vol, float pan) { XCALL(0x004F0B70); }
	virtual zTSoundHandle PlaySound3D(zCSoundFX *sfx, zCVob *sourceVob, int vobSlot, zTSound3DParams *sound3DParams) { XCALL(0x004F10F0); }
	virtual zTSoundHandle PlaySound3D(zSTRING &soundName, zCVob *sourceVob, int vobSlot, zTSound3DParams *sound3DParams) { XCALL(0x004F1060); }
	virtual void StopSound(zTSoundHandle &) { XCALL(0x004F2300); }
	virtual void StopAllSounds() { XCALL(0x004F23C0); }
	virtual int GetSound3DProps(zTSoundHandle &sfxHandle, zTSound3DParams &sound3DParams) { XCALL(0x004F3780); }
	virtual int UpdateSound3D(zTSoundHandle &sfxHandle, zTSound3DParams *sound3DParams) { XCALL(0x004F2410); }
	virtual void GetSoundProps(zTSoundHandle &sfxHandle, int &freq, float &vol, float &pan) { XCALL(0x004F3580); }
	virtual void UpdateSoundProps(zTSoundHandle &sfxHandle, int freq, float vol, float pan) { XCALL(0x004F3970); }
	virtual int IsSoundActive(zTSoundHandle &sfxHandle) { XCALL(0x004F3FD0); }
};

class zCOption
{
public:
	int ReadBool(zSTRING &secName, char *name, int value) { XCALL(0x00462160); }
};

class zCInputCallback
{
public:
	virtual int HandleEvent(int key) { XCALL(0x0043D4E0); }
};

class zCBspBase
{
public:
	void __fastcall CollectVobsInBBox3D(zCArray<zCVob *> &resVobList, zTBBox3D &inbbox3D) { XCALL(0x00531110); }
};

struct oTRobustTrace
{
	unsigned char stand : 1;
	unsigned char dirChoosed : 1;
	unsigned char exactPosition : 1;
	unsigned char targetReached : 1;
	unsigned char standIfTargetReached : 1;
	unsigned char waiting : 1;
	unsigned char isObstVobSmall : 1;
	unsigned char targetVisible : 1;
	unsigned char useChasmChecks : 1;

	zVEC3 targetPos;
	zCVob *targetVob;
	zCVob *obstVob;
	float targetDist;
	float lastTargetDist;
	float maxTargetDist;
	float dirTurn;
	float timer;
	zVEC3 dirFirst;
	float dirLastAngle;

	zCArray<oTDirectionInfo *> lastDirections;

	int frameCtr;

	zVEC3 targetPostArray[5];
	int targetPosCounter;
	int targetPosIndex;
	float checkVisibilityTime;
	float positionUpdateTime;
	float failurePossibility;
};

struct TNpcAIState
{
	int index;
	int loop;
	int end;
	int timeBehaviour;
	float restTime;
	int phase;
	int valid;
	zSTRING name;
	float stateTime;
	int prgIndex;
	int isRtnState;
};

struct TNpcPerc
{
	int percID;
	int percFunc;
};

struct TNpcSlot
{
	zSTRING name;
	int inInventory;
	int tmpLevel;
	zSTRING itemName;

	oCVob *vob;

	int wasVobTreeWhenInserted : 1;
};

class oCVob : public zCVob { };

class oCItem : public oCVob { };

class oCWorld : public zCWorld
{
public:
	zSTRING worldFilename;
	zSTRING worldName;

	oTVobList *voblist;
	oTVobListNpcs *voblist_npcs;
	oTVobListItems *voblist_items;
};

class zCSession : public zCInputCallback
{
public:
	zCCSManager *csMan;
	zCWorld *world;
	zCCamera *camera;
	zCAICamera *aiCam;
	zCVob *camVob;
	zCView *viewport;
};

struct TObjectRoutine
{
	zSTRING objName;
	int stateNum;
	int hour1;
	int min1;
	int type;
};

class oCGame : public zCSession
{
public:
	float cliprange;
	float fogrange;

	int inScriptStartup;
	int inLoadSaveGame;
	int inLevelChange;

	zCView *array_view[6];
	int array_view_visible[6];
	int array_view_enabled[6];

	oCSavegameManager *savegameManager;
	zCView *game_text;
	zCView *load_screen;
	zCView *save_screen;
	zCView *pause_screen;
	oCViewStatusBar *hpBar;
	oCViewStatusBar *swimBar;
	oCViewStatusBar *manaBar;
	oCViewStatusBar *focusBar;
	int showPlayerStatus;

	int game_drawall;
	int game_frameinfo;
	int game_showaniinfo;
	int game_showwaynet;
	int game_testmode;
	int game_editwaynet;
	int game_showtime;
	int game_showranges;
	int drawWayBoxes;
	int scriptStartup;
	int showFreePoints;
	oCNpc *showRoutineNpc;

	int loadNextLevel;
	zSTRING loadNextLevelName;
	zSTRING loadNextLevelStart;

	zVEC3 startpos;
	oCGameInfo *gameInfo;
	zCVobLight *pl_light;
	float pl_lightval;

	oCWorldTimer *wldTimer;
	float timeStep;
	int singleStep;

	oCGuilds *guilds;
	oCInfoManager *infoman;
	oCNewsManager *newsman;
	oCSVMManger *svmman;
	oCTradeManager *trademan;
	oCPortalRoomManager *portalman;
	oCSpawnManager *spawnman;

	float music_delay;
	oCNpc *watchnpc;

	int m_bWorldEntered;
	float m_fEnterWorldTimer;

	int initial_hour;
	int initial_minute;

	zCArray<zCVob *> debugInstances;
	int debugChannels;
	int debugAllInstances;

	int oldRoutineDay;
	zCListSort<TObjectRoutine> objRoutineList;
	zCListSort<TObjectRoutine> *currentObjectRoutine;
	zCViewProgressBar *progressBar;
	zCArray<zCVisual *> visualList;

public:
	void Render() { XCALL(0x006C86A0); }
	oCWorld *GetGameWorld() { XCALL(0x006C2C30); }
};

class oCMagFrontier
{
public:
	oCVisualFX *warningFX;
	oCVisualFX *shootFX;
	oCNpc *npc;

	unsigned char isWarning : 1;
	unsigned char isShooting : 1;

public:
	void DoCheck() { XCALL(0x00473740); }
	void DoWarningFX(int level) { XCALL(0x00474CB0); }
	void DisposeWarningFX() { XCALL(0x00474E70); }
	void DoShootFX(zVEC3 &startPoint) { XCALL(0x00474EB0); }
	void DisposeShootFX() { XCALL(0x004751A0); }
};

class oCNpcTimedOverlay
{
public:
	zSTRING mdsOverlayName;
	float timer;
};

class oCNpc_States
{
public:
	zSTRING name;

	oCNpc *npc;
	TNpcAIState curState;
	TNpcAIState nextState;
	int lastAIState;

	int hasRoutine;
	int rtnChanged;
	oCRtnEntry *rtnBefore;
	oCRtnEntry *rtnNow;
	zCRoute *rtnRoute;
	int rtnOverlay;
	int rtnOverlayCount;
	int walkmode_routine;
	int weaponmode_routine;
	int startNewRoutine;

	int aiStateDriven;
	zVEC3 aiStatePosition;

	oCNpc *parOther;
	oCNpc *parVictim;
	oCItem *parItem;

	int rtnChangeCount;

public:
	virtual void Archive(zCArchiver &arc) { XCALL(0x0076EE70); }

	int StartAIState(zSTRING &name, int endOldState, int timeBehaviour, float timed, int isRtnState) { XCALL(0x0076C700); }
	void ClearAIState() { XCALL(0x0076D6E0); }
	void EndCurrentState() { XCALL(0x0076D880); }
};

class oCItemContainer : public zCInputCallback
{
public:
	zCListSort<oCItem> *contents;
	oCNpc *npc;
	zSTRING titleText;
	int invMode;
	int selectedItem;
	int offset;
	int maxSlotsCol;
	int maxSlotsColScr;
	int maxSlotsRow;
	int maxSlotsRowScr;
	int maxSlots;
	int marginTop;
	int marginLeft;
	int frame;
	int right;
	int ownList;
	int prepared;
	int passive;
	int TransferCount;

	zCView *viewTitle;
	zCView *viewBack;
	zCView *viewItem;
	zCView *viewItemActive;
	zCView *viewItemHightlighted;
	zCView *viewItemActiveHighlighted;
	zCView *viewItemInfo;
	zCView *viewItemInfoItem;
	zCView *textView;
	zCView *viewArrowAtTop;
	zCView *viewArrowAtBottom;

	zCWorld *rndWorld;
	int posx;
	int posy;
	int m_bManipulateItemsDisabled;
	int m_bCanTransferMoreThanOneItem;
};

class oCNpcInventory : public oCItemContainer
{
public:
	oCNpc *owner;
	int packAbility;

	zCListSort<oCItem> inventory;

	zSTRING packString;
	int maxSlots;

public:
	int IsEmpty(int bIgnoreArmor, int bIgnoreActive) { XCALL(0x0070D1A0); }
};

class oCNewsMemory
{
public:
	zCList<oCNews> iknow;

public:
	virtual void Archive(zCArchiver &arc) { XCALL(0x0072A140); }
};

struct oSDamageDescriptor
{
	unsigned int dwFieldsValid;

	zCVob *pVobAttacker;
	oCNpc *pNpcAttacker;
	zCVob *pVobHit;
	oCVisualFX *pFXHit;
	oCItem *pItemWeapon;
	unsigned int nSpellId;
	unsigned int nSpellCat;
	unsigned int nSpellLevel;
	int enuModeDamage;
	int enuModeWeapon;
	unsigned int aryDamage[8];
	float fDamageTotal;
	float fDamageMultiplier;
	zVEC3 vecLocationHit;
	zVEC3 vecDirectionFly;
	zSTRING strVisualFX;
	float fTimeDuration;
	float fTimeInterval;
	float fDamagePerInterval;
	int bDamageDontKill;

	unsigned int bOnce : 1;
	unsigned int bFinished : 1;
	unsigned int bIsDead : 1;
	unsigned int bIsUnconscious : 1;
	unsigned int lReserved : 28;

	float fAzimuth;
	float fElevation;
	float fTimeCurrent;
	float fDamageReal;
	float fDamageEffective;
	unsigned int aryDamageEffective[8];
	zCVob *pVobParticleFX;
	zCParticleFX *pParticleFX;
	oCVisualFX *pVisualFX;
};

class oCNpc : public oCVob
{
public:
	static zCClassDef &classDef;
	static oCNpc *&player;
	static int &godmode;

	int idx;
	zSTRING name[5];
	zSTRING slot;
	zSTRING effect;

	int npcType;
	int variousFlags;
	int attribute[8];
	int hitChance[5];
	int protection[8];
	int damage[8];
	int damagetype;
	int guild;
	int level;
	int mission[5];
	int fighttactic;
	int fmode;
	int voice;
	int voicePitch;
	int mass;
	int daily_routine;
	int startAIState;
	zSTRING spawnPoint;
	int spawnDelay;
	int senses;
	int senses_range;
	int aiscriptvars[100];
	zSTRING wpname;
	int experience_points;
	int experience_points_next_level;
	int learn_points;
	int bodyStateInterruptableOverride;
	int noFocus;
	int parserEnd;

	int bloodEnabled;
	int bloodDistance;
	int bloodAmount;
	int bloodFlow;
	zSTRING bloodEmitter;
	zSTRING bloodTexture;
	int didHit;
	int didParade;
	int didShoot;
	int hasLockedEnemy;
	int isDefending;
	int wasAming;
	int lastAction;
	oCNpc *enemy;

	float speedTurn;
	int foundFleePoint;
	int reachedFleePoint;
	zVEC3 vecFlee;
	zVEC3 posFlee;
	zCWaypoint *waypointFlee;
	oTRobustTrace rbt;

	zCList<oCNpcTimedOverlay> timedOverlays;
	zCArray<oCNpcTalent *> talents;

	int spellMana;

	oCMagFrontier magFrontier;
	oCNpc_States state;
	oCNpcInventory inventory2;
	oCItemContainer *trader;
	oCNpc *tradeNpc;

	float rangeToPlayer;

	zCArray<zTSoundHandle> listOfVoiceHandles;
	int voiceIndex;
	zCArray<oCVisualFX *> effectList;

	int showaidebug : 1;
	int showNews : 1;
	int csAllowedAsRole : 1;

	int isSummoned : 1;
	int respawnOn : 1;
	int movlock : 1;
	int drunk : 1;
	int mad : 1;
	int overlay_wounded : 1;
	int inOnDamage : 1;
	int autoremoveweapon : 1;
	int openinventory : 1;
	int askroutine : 1;
	int spawnInRange : 1;

	int body_TexVarNr : 16;
	int body_TexColorNr : 16;
	int head_TexVarNr : 16;
	int teeth_TexVarNr : 16;
	int guildTrue : 8;
	int drunk_heal : 8;
	int mad_heal : 8;
	int spells : 8;
	int bodyState : 19;
	int m_bAniMessageRunning : 1;

	int instanz;

	zSTRING mds_name;
	zSTRING body_visualName;
	zSTRING head_visualName;

	zVEC3 model_scale;
	float model_fatness;

	int namenr;
	float hpHeal;
	float manaHeal;
	float swimtime;
	float divetime;
	float divectr;
	zCVob *fireVob;
	int fireDamage;
	float fireDamageTimer;

	int attitude;
	int tmp_attitude;
	float attTimer;
	int knowsPlayer;

	TNpcPerc percList[33];
	int percActive;
	float percActiveTime;
	float percActiveDelta;

	int overrideFallDownHeight;
	float fallDownHeight;
	int fallDownDamage;

	oCMag_Book *mag_book;
	zCList<oCSpell> activeSpells;
	int lastHitSpellID;
	int lastHitSpellCat;

	zCArray<zSTRING> activeOverlays;

	oCAskBox *askbox;
	int askYes;
	int askNo;
	float canTalk;
	oCNpc *talkOther;

	oCInfo *info;
	oCNews *news;
	oCMission *curMission;

	oCNewsMemory knownNews;

	zCVob *carry_vob;
	oCMobInter *interactMob;
	oCItem *interactItem;
	int interactItemCurrentState;
	int interactItemTargetState;

	int script_aiprio;
	int old_script_state;
	oCAIHuman *human_ai;
	oCAniCtrl_Human *anictrl;
	zCRoute *route;
	float damageMul;

	oCNpcMessage *csg;
	oCNpcMessage *lastLookMsg;
	oCNpcMessage *lastPointMsg;

	zCArray<zCVob *> vobList;
	float vobcheck_time;
	float pickvobdelay;
	zCVob *focus_vob;

	zCArray<TNpcSlot *> invSlot;
	zCArray<TNpcSlot *> tmpSlotList;

	float fadeAwayTime;
	float respawnTime;

	float selfDist;
	int fightRangeBase;
	int fightRangeFist;
	int fightRangeG;

	float fight_waitTime;
	int fight_waitForAniEnd;
	float fight_lastStrafeFrame;

	int soundType;
	zCVob *soundVob;
	zVEC3 soundPosition;

	zCPlayerGroup *playerGroup;

public:
	void OnDamage_Events(oSDamageDescriptor &descDamage) { XCALL(0x0067ABE0); }
	void StandUp(int walkingallowed, int startAniTransition) { XCALL(0x00682B40); }
	zCModel *GetModel() { XCALL(0x00738720); }
	oCSpell *IsSpellActive(int nr) { XCALL(0x0073D340); }
	void InsertActiveSpell(oCSpell *spell) { XCALL(0x0073D070); }
	int CheckForOwner(zCVob *v) { XCALL(0x007405F0); }
	void SetAsPlayer() { XCALL(0x007426A0); }
	void ClearVobList() { XCALL(0x0075D7F0); }
	void CreateVobList(float max_dist) { XCALL(0x0075DA40); }
	int HasBodyStateModifier(int nr) { XCALL(0x0075EBF0); }
	void SetBodyStateModifier(int nr) { XCALL(0x0075EC10); }
	int ModifyBodyState(int add, int remove) { XCALL(0x0075EF50); }
};

class zCModel : public zCObject
{
public:
	zCModelAni *GetAniFromAniID(int aniID) { XCALL(0x00472F50); }
	int IsAniActive(zSTRING &aniName);
	int IsAniActive(zCModelAni *modelAni) { XCALL(0x00472F90); }
	int GetAniIDFromAniName(zSTRING &aniName) { XCALL(0x00612070); }
};

class oCAniCtrl_Human : public zCObject
{
public:
	void SearchStandAni(int forceStartAni) { XCALL(0x006A4D20); }
};

class oCSpell : public zCObject
{
public:
	int keyNo;

	oCVisualFX *effect;
	oCVisualFX *controlWarnFX;
	zCVob *spellCaster;
	oCNpc *spellCasterNpc;
	zCVob *spellTarget;
	oCNpc *spellTargetNpc;
	oCNpc *saveNpc;
	float manaTimer;
	int manaInvested;
	int spellLevel;
	int spellStatus;
	int spellID;
	int spellInfo;
	int spellEnabled;
	int spellInitDone;

	int timerEffect;
	int canBeDeleted;

	float up;
	float hoverY;
	float hoverOld;
	float hoverDir;

	int spellEnergy;

	float manaInvestTime;
	int damagePerLevel;
	int damageType;
	int spellType;
	int canTurnDuringInvest;
	int canChangeTargetDuringInvest;
	int isMultiEffect;
	int targetCollectAlgo;
	int targetCollectType;
	int targetCollectRange;
	int targetCollectAzi;
	int targetCollectElev;

public:
	oCVisualFX *CreateEffect() { XCALL(0x004842E0); }
	void StopTargetEffects(zCVob *vob) { XCALL(0x00485C40); }
	int IsValidTarget(zCVob *v) { XCALL(0x004861D0); }
	int IsInvestSpell() { XCALL(0x00486630); }
	void DoLogicInvestEffect() { XCALL(0x00486950); }
	int CastSpecificSpell() { XCALL(0x00486960); }
	void EndTimedEffect() { XCALL(0x00486E10); }
	void DoTimedEffect() { XCALL(0x00487280); }
};

class oCMOB : public oCVob
{
public:
	int IsMovable() { XCALL(0x0071BF20); }
};

class oCMobInter : public oCMOB
{
public:
	static zCClassDef &classDef;

public:
	int IsOccupied() { XCALL(0x00718CC0); }
};

class oCVisualFX : public zCObject
{
public:
	static oCVisualFX *CreateAndPlay(zSTRING &id, zCVob *org, zCVob *target, int level, float damage, int damageType, int bIsProjectile) { XCALL(0x0048E760); }
	static oCVisualFX *CreateAndPlay(zSTRING &id, zVEC3 &orgPos, zCVob *target, int level, float damage, int damageType, int bIsProjectile) { XCALL(0x0048EA80); }

	void SetSpellTargetTypes(int types) { XCALL(0x0048A1C0); }
	void Init(zCVob *orgVob, zCVob *trgtVob, zCVob *inflictorVob) { XCALL(0x00491F20); }
	void Cast(int killAfterDone) { XCALL(0x00493160); }
	void Stop(int killAfterDone) { XCALL(0x00493BE0); }
};

class zCTimer
{
public:
	float factorMotion;
	float frameTimeFloat;
	float totalTimeFloat;
	float frameTimeFloatSecs;
	float totalTimeFloatSecs;
	unsigned int lastTimer;
	unsigned int frameTime;
	unsigned int totalTime;
	unsigned int minFrameTime;
	unsigned int forcedMaxFrameTime;
};

class zCInput_Win32
{
public:
	float GetState(unsigned short logicalID) { XCALL(0x00830B68); }
};

class zCActiveSnd
{
public:
	void AutoCalcObstruction(int immediate) { XCALL(0x004F9830); }
};

class zCRenderLight
{
public:
	int lightType;
	zVEC3 colorDiffuse;
	zVEC3 position;
	zVEC3 direction;
	float range;

	float rangeInv;
	zVEC3 positionLS;
	zVEC3 directionLS;
	float dir_approxFalloff;
};

class zCBspSector
{
public:
	zSTRING sectorName;
	// ...
};

struct zSParticle
{
	zSParticle *next;
	zVEC3 position;
	zVEC3 positionWS;
	zVEC3 vel;
	float lifeSpan;
	float alpha;
	float alphaVel;
	zVEC2 size;
	zVEC2 sizeVel;
	zVEC3 color;
	zVEC3 colorVel;
	zCPolyStrip *polyStrip;
};
