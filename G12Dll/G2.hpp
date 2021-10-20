WRAPPER void *_malloc(size_t Size) { EAXJMP(0x007B4460); }
#define malloc(Size) _malloc(Size)

WRAPPER void *_calloc(size_t NumOfElements, size_t SizeOfElements) { EAXJMP(0x007B4465); }
#define calloc(NumOfElements, SizeOfElements) _calloc(NumOfElements, SizeOfElements)

WRAPPER void *_realloc(void *Memory, size_t NewSize) { EAXJMP(0x007B446A); }
#define realloc(Memory, NewSize) _realloc(Memory, NewSize)

WRAPPER void _free(void *Memory) { EAXJMP(0x007B446F); }
#define free(Memory) _free(Memory)

void *operator new(size_t Size) { return malloc(Size); }
void operator delete(void *Memory) { free(Memory); }
void *operator new[](size_t Size) { return malloc(Size); }
void operator delete[](void *Memory) { free(Memory); }

WRAPPER void *_memmove(void *Dst, const void *Src, size_t Size) { EAXJMP(0x007D0510); }
#define memmove(Dst, Src, Size) _memmove(Dst, Src, Size)

WRAPPER int _rand(void) { EAXJMP(0x007D2F98); }
#define rand() _rand()

#ifdef PlaySound
#undef PlaySound
#endif

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

class zVEC2;
class zVEC3;
class zVEC4;
class zMAT3;
class zMAT4;
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
class zCEarthquake;
class zCVobScreenFX;
class zCPositionKey;
class zCKBSpline;
class zCModelNodeInst;
class zFILE;
class zCMutex;
class zCCollisionReport;
class zCLensFlareFX;
class zCOBBox3D;
class zCProgMeshProto;
class zCQuadMark;
class zCBuffer;

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
class oCVisualFXAI;
class oCEmitterKey;

enum zTRnd_AlphaBlendFunc
{
	zRND_ALPHA_FUNC_MAT_DEFAULT,
	zRND_ALPHA_FUNC_NONE,
	zRND_ALPHA_FUNC_BLEND,
	zRND_ALPHA_FUNC_ADD,
	zRND_ALPHA_FUNC_SUB,
	zRND_ALPHA_FUNC_MUL,
	zRND_ALPHA_FUNC_MUL2,
	zRND_ALPHA_FUNC_TEST,
	zRND_ALPHA_FUNC_BLEND_TEST
};

enum zTVobSleepingMode
{
	zVOB_SLEEPING,
	zVOB_AWAKE,
	zVOB_AWAKE_DOAI_ONLY
};

enum { VX, VY, VZ, VW };

class zVEC2
{
public:
	float n[2];

public:
	zVEC2() { }
	zVEC2(float x, float y) { this->n[VX] = x; this->n[VY] = y; }

	float &operator[](int i) { return this->n[i]; }
	float operator[](int i) const { return this->n[i]; }
};

class zVEC3
{
public:
	float n[3];

public:
	zVEC3() { }
	zVEC3(float x, float y, float z) { this->n[VX] = x; this->n[VY] = y; this->n[VZ] = z; }

	float &operator[](int i) { return this->n[i]; }
	float operator[](int i) const { return this->n[i]; }

	zVEC3 &operator+=(zVEC3 &v) { this->n[VX] += v[VX]; this->n[VY] += v[VY]; this->n[VZ] += v[VZ]; return *this; }
	zVEC3 &operator-=(zVEC3 &v) { this->n[VX] -= v[VX]; this->n[VY] -= v[VY]; this->n[VZ] -= v[VZ]; return *this; }
	friend zVEC3 operator-(zVEC3 &v) { return zVEC3(-v[VX], -v[VY], -v[VZ]); }
	friend zVEC3 operator+(zVEC3 &v, float f) { return zVEC3(v[VX] + f, v[VY] + f, v[VZ] + f); }
	friend zVEC3 operator+(zVEC3 &a, zVEC3 &b) { return zVEC3(a[VX] + b[VX], a[VY] + b[VY], a[VZ] + b[VZ]); }
	friend zVEC3 operator-(zVEC3 &v, float f) { return zVEC3(v[VX] - f, v[VY] - f, v[VZ] - f); }
	friend zVEC3 operator-(zVEC3 &a, zVEC3 &b) { return zVEC3(a[VX] - b[VX], a[VY] - b[VY], a[VZ] - b[VZ]); }
	friend zVEC3 operator*(zVEC3 &v, float f) { return zVEC3(v[VX] * f, v[VY] * f, v[VZ] * f); }
	friend float operator*(zVEC3 &a, zVEC3 &b) { return a[VX] * b[VX] + a[VY] * b[VY] + a[VZ] * b[VZ]; }
	friend zVEC3 operator*(zMAT4 &m, zVEC3 &v);
	friend zVEC3 operator^(zVEC3 &a, zVEC3 &b) { return zVEC3(a[VY] * b[VZ] - a[VZ] * b[VY], a[VZ] * b[VX] - a[VX] * b[VZ], a[VX] * b[VY] - a[VY] * b[VX]); }

	float Length() { return sqrtf(this->Length2()); }
	float Length2() { return this->n[VX] * this->n[VX] + this->n[VY] * this->n[VY] + this->n[VZ] * this->n[VZ]; }
	float LengthApprox() { XCALL(0x00490E10); }
	zVEC3 &Normalize() { XCALL(0x00490EA0); }
	zVEC3 &NormalizeApprox() { XCALL(0x0054E750); }
};

class zVEC4
{
public:
	float n[4];

public:
	zVEC4() { }
	zVEC4(float x, float y, float z, float w) { this->n[VX] = x; this->n[VY] = y; this->n[VZ] = z; this->n[VW] = w; }

	float &operator[](int i) { return this->n[i]; }
	float operator[](int i) const { return this->n[i]; }
};

class zMAT3
{
public:
	zVEC3 v[3];

public:
	zMAT3() { }
	zMAT3(const zVEC3 &v0, const zVEC3 &v1, const zVEC3 &v2) { this->v[0] = v0; this->v[1] = v1; this->v[2] = v2; }

	zVEC3 &operator[](int i) { return this->v[i]; };
	zVEC3 operator[](int i) const { return this->v[i]; };
};

class zMAT4
{
public:
	static zMAT4 &s_identity;

	zVEC4 v[4];

public:
	zMAT4() { }
	zMAT4(const zVEC4 &v0, const zVEC4 &v1, const zVEC4 &v2, const zVEC4 &v3) { this->v[0] = v0; this->v[1] = v1; this->v[2] = v2; this->v[3] = v3; }

	zVEC4 &operator[](int i) { return this->v[i]; };
	zVEC4 operator[](int i) const { return this->v[i]; };

	zVEC3 GetTranslation() { return zVEC3(this->v[0][3], this->v[1][3], this->v[2][3]); }
	zMAT4 &SetTranslation(const zVEC3 &t) { this->v[0][3] = t[VX]; this->v[1][3] = t[VY]; this->v[2][3] = t[VZ]; return *this; }
	zVEC3 GetAtVector() { return zVEC3(this->v[0][2], this->v[1][2], this->v[2][2]); }
	void SetAtVector(const zVEC3 &a) { this->v[0][2] = a[VX]; this->v[1][2] = a[VY]; this->v[2][2] = a[VZ]; }
	void SetUpVector(const zVEC3 &a) { this->v[0][1] = a[VX]; this->v[1][1] = a[VY]; this->v[2][1] = a[VZ]; }
	void SetRightVector(const zVEC3 &a) { this->v[0][0] = a[VX]; this->v[1][0] = a[VY]; this->v[2][0] = a[VZ]; }
	zMAT4 InverseLinTrafo() { XCALL(0x00515340); }
};

#define zARRAY_START_ALLOC 16

template<class T> class zCArray
{
public:
	T *array;
	int numAlloc;
	int numInArray;

public:
	zCArray() { this->numInArray = 0; this->numAlloc = 0; this->array = NULL; }

	zCArray(int startSize)
	{
		this->numInArray = 0;
		this->numAlloc = 0;
		this->array = NULL;

		if (startSize > 0) array = new T[startSize];
	}

	zCArray(const zCArray<T> &array2)
	{
		this->numInArray = 0;
		this->numAlloc = 0;
		this->array = NULL;

		AllocAbs(array2.numInArray);

		this->numInArray = array2.numInArray;

		for (int i = 0; i < this->numInArray; i++) this->array[i] = array2[i];
	}

	~zCArray() { delete[] this->array; this->array = NULL; }

	const T &operator[](int nr) const { return this->array[nr]; }
	T &operator[](int nr) { return this->array[nr]; }

	zCArray<T> &operator=(const zCArray<T> &array2)
	{
		this->EmptyList();
		this->AllocAbs(array2.numInArray);
		this->numInArray = array2.numInArray;

		for (int i = 0; i < this->numInArray; i++) this->array[i] = array2[i];

		return *this;
	}

	void AllocDelta(int numDelta)
	{
		if (numDelta <= 0) return;
		T *newArryay = new T[this->numAlloc + numDelta]();
		for (int i = 0; i < this->numInArray; i++) newArryay[i] = this->array[i];
		delete[] this->array;
		this->array = newArryay;
		this->numAlloc += numDelta;
	}

	void AllocAbs(int size)
	{
		if (this->numAlloc >= size) return;

		this->AllocDelta(size - this->numAlloc);
	}

	void InsertEnd(const T &ins)
	{
		if (this->numAlloc < this->numInArray + 1)
		{
			if (this->numAlloc < zARRAY_START_ALLOC) this->AllocDelta(zARRAY_START_ALLOC);
			else this->AllocDelta(this->numAlloc / 2);
		}

		this->array[this->numInArray++] = ins;
	}

	void InsertFront(const T &ins) { this->InsertAtPos(ins, 0); }

	void Insert(const T &ins) { this->InsertEnd(ins); }

	void InsertAtPos(const T &ins, int pos)
	{
		if (this->numAlloc < this->numInArray + 1)
		{
			if (this->numAlloc < zARRAY_START_ALLOC) this->AllocDelta(zARRAY_START_ALLOC);
			else this->AllocDelta(this->numAlloc / 2);
		}

		memmove(&this->array[pos + 1], &this->array[pos], sizeof(T) * this->numInArray - pos);
		this->array[pos] = ins;
		this->numInArray++;
	}

	void RemoveIndex(int index)
	{
		if (this->numInArray > 0)
		{
			this->numInArray--;

			if (index != this->numInArray)
			{
				this->array[index] = this->array[this->numInArray];
			}
		}
	}

	void RemoveOrder(const T &rem)
	{
		int index = this->Search(rem);

		if (index == -1) return;

		this->RemoveOrderIndex(index);
	}

	void RemoveOrderIndex(int index)
	{
		if (index >= this->numInArray) return;

		if (index != this->numInArray - 1)
		{
			for (int j = index; j < this->numInArray - 1; j++) this->array[j] = this->array[j + 1];
		}

		this->numInArray--;
	}

	void DeleteList()
	{
		delete[] this->array;

		this->array = NULL;
		this->numAlloc = 0;
		this->numInArray = 0;
	}

	void EmptyList() { this->numInArray = 0; }

	int Search(const T &data)
	{
		for (int i = 0; i < this->numInArray; i++) if (this->array[i] == data) return i;

		return -1;
	}

	bool32 IsInList(const T &data)
	{
		for (int i = 0; i < this->numInArray; i++) if (this->array[i] == data) return TRUE;

		return FALSE;
	}
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
	UCHAR _Al;
	CHAR *_Ptr;
	INT _Len;
	INT _Res;

public:
	virtual ~zSTRING() { XCALL(0x00401160); };

	zSTRING() { XCALL(0x00402AF0); }
	zSTRING(const zSTRING &xStr) { XCALL(0x00416500); }
	zSTRING(const zSTRING *pstr) { XCALL(0x007928D0); }
	zSTRING(char *pstring) { XCALL(0x004010C0); }
	zSTRING(char ch) { XCALL(0x0051AC80); }
	zSTRING(int xWert) { XCALL(0x00435870); }
	zSTRING(unsigned int xWert) { XCALL(0x00461E90); }
	zSTRING(const float xWert, int digits = 20) { XCALL(0x00435970); }
	zSTRING(const double xWert, int digits = 20) { XCALL(0x00454680); }

	byte IsEmpty() const { return this->_Len == 0; }
	int Length() const { return this->_Len; }
	char *ToChar() const { return this->_Ptr; }
	void Clear() { XCALL(0x0059D010); }

	int Search(int startIndex, const char *substr, unsigned int num = 1) { XCALL(0x0046C920); }
	int Search(int startIndex, const zSTRING &substr, unsigned int num = 1) { return this->Search(startIndex, substr.ToChar(), num); }
	int Search(const zSTRING &substr, unsigned int num = 1) { return this->Search(0, substr.ToChar(), num); }
	int Search(const char *substr, unsigned int num = 1) { return this->Search(0, substr, num); }

	bool32 Contains(char *substr) { return this->Search(0, substr, 1) != -1; }
	bool32 Contains(const zSTRING &substr) { return Search(0, substr.ToChar(), 1) != -1; }

	friend zSTRING operator+(const zSTRING &xStr1, const zSTRING &xStr2) { XCALL(0x004045C0); }
	friend zSTRING operator+(const zSTRING &xStr1, const char *pstring) { XCALL(0x00404880); }
	friend zSTRING operator+(const zSTRING &xStr1, const char ch) { XCALL(0x00445DD0); }
	friend zSTRING operator+(const char *pstring, const zSTRING &xStr2) { XCALL(0x00404710); }
	friend zSTRING operator+(const char ch, const zSTRING &xStr2) { XCALL(0x0044A190); }

	friend bool32 operator==(const zSTRING &xStr1, const zSTRING &xStr2) { XCALL(0x00674220); }
	friend bool32 operator==(const zSTRING &xStr1, const char *xStr2) { XCALL(0x006CFF50); }
	friend bool32 operator==(const char *xStr1, const zSTRING &xStr2) { return xStr2 == xStr1; }

	friend bool32 operator!=(const zSTRING &xStr1, const zSTRING &xStr2) { return !(xStr1 == xStr1); }
	friend bool32 operator!=(const zSTRING &xStr1, const char *xStr2) { return !(xStr1 == xStr2); }
	friend bool32 operator!=(const char *xStr1, const zSTRING &xStr2) { return !(xStr1 == xStr2); }
};

class zCOLOR
{
public:
	union
	{
		struct
		{
			byte b;
			byte g;
			byte r;
			byte alpha;
		};

		dword dword;
	};

public:
	zCOLOR() { };
	zCOLOR(byte rr, byte gg, byte bb, byte aa) { this->r = rr; this->g = gg; this->b = bb; this->alpha = aa; };
	zCOLOR(unsigned int color) { this->dword = color; }
};

template<class KEY, class ELEMENT> class zTRayTurboValMap
{
public:
	struct zSNode
	{
		KEY m_Key;
		ELEMENT m_Element;
		unsigned int m_u32Hash;
		zSNode *m_pNext;
	};

public:
	zCArray<zSNode *> m_arrNodes;
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

enum zTAnimationMode { };

enum zTVisualCamAlign
{
	zVISUAL_CAMALIGN_NONE,
	zVISUAL_CAMALIGN_YAW,
	zVISUAL_CAMALIGN_FULL,
	zVISUAL_CAMALIGN_COUNT
};

struct zTRenderContext
{
	int clipFlags;
	zCVob *vob;
	zCWorld *world;
	zCCamera *cam;
	float distVobToCam;
	zTVisualCamAlign visualCamAlign;
	zTAnimationMode m_AniMode;
	float m_aniModeStrength;
	zCOLOR hintLightingHighlightColor;

	byte hintLightingFullbright : 1;
	byte hintLightingSwell : 1;
	byte hintLightingHighlight : 1;
};

struct zTTraceRayReport
{
	bool32 foundHit;
	zCVob *foundVob;
	zCPolygon *foundPoly;
	zVEC3 foundIntersection;
	zVEC3 foundPolyNormal;
	zCVertex *foundVertex;
};

struct myVert
{
	int vertIndex;
	int vertNeighbours[8];
	int numNeighbours;

	int polyIndices[50];
	int numPolyIndices;

	bool32 active;
};

struct myThunder
{
	zVEC3 originVec;
	myThunder *childs;
	int numChilds;
	float startTime[5];
	zCPolyStrip *polyStrip;
	int numSegs;
	bool32 valid;
	float t0;
	float t1;
	int numSplits;
	bool32 dead;
	bool32 isChild;
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
	word archiveVersion;
	word archiveVersionSum;

public:
	static void ObjectCreated(zCObject *object, zCClassDef *objClassDef) { XCALL(0x005AAEB0); }
	static void ObjectDeleted(zCObject *object, zCClassDef *objClassDef) { XCALL(0x005AAFD0); }
};

class zCObject
{
public:
	int refCtr;
	word hashIndex;
	zCObject *hashNext;
	zSTRING objectName;

public:
	static bool32 CheckInheritance(zCClassDef *baseClass, zCClassDef *subClass) { XCALL(0x00476E30); }

	zCObject() { XCALL(0x00401D60); }

	virtual zCClassDef *_GetClassDef() { XCALL(0x00401EC0); }
	virtual void Archive(zCArchiver &arc) { }
	virtual void Unarchive(zCArchiver &arc) { }
	virtual ~zCObject() { XCALL(0x005A8C50); }

	zCObject *AddRef() { this->refCtr++; return this; }
	int Release() { XCALL(0x0040C310); }
};

#define zRELEASE(obj) { if (obj) { obj->Release(); obj = NULL; } }

class zCAIBase : public zCObject { };

class oCVisualFXAI : public zCAIBase
{
public:
	zCVob *vob;
	bool32 delete_it;
};

class zCBBox3DSorterBase
{
public:
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
};

class zCVisual : public zCObject
{
public:
	zCVisual *nextLODVisual;
	zCVisual *prevLODVisual;
	float lodFarDistance;
	float lodNearFadeOutDistance;

public:
	virtual bool32 Render(zTRenderContext &renderContext) = 0;
	virtual bool32 IsBBox3DLocal() = 0;
	virtual zTBBox3D GetBBox3D() = 0;
	virtual zCOBBox3D *GetOBBox3D() = 0;
	virtual zSTRING GetVisualName() = 0;
	virtual bool32 GetVisualDied() = 0;
};

class zCVisualAnimate : public zCVisual { };

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

class zCPolygon
{
public:
#pragma pack (push, 1)
	struct TFlags
	{
		byte portalPoly : 2;
		byte occluder : 1;
		byte sectorPoly : 1;
		byte mustRelight : 1;
		byte portalIndoorOutdoor : 1;
		byte ghostOccluder : 1;
		byte noDynLightNear : 1;
		word sectorIndex : 16;
	};
#pragma pack (pop)

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
	byte polyNumVert;

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
	bool32 hasLightmaps;
	bool32 m_bUsesAlphaTesting;

	int numVertAlloc;
	int numPolyAlloc;

public:
	static zCMesh *Load(zSTRING &meshFileName, bool32 a_bDontConvertToNPolys) { XCALL(0x00567600); }

	virtual bool32 Render(zTRenderContext &renderContext, zCOLOR *vertexColor) { XCALL(0x0056B210); }
};

class zCTexAniCtrl
{
public:
	int aniChannel;
	float actFrame;
	float aniFPS;
	int frameCtr;
	bool32 bOneShotAni;
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

	bool32 m_bEnvironmentalMappingStrength;

	byte smooth : 1;
	byte dontUseLightmaps : 1;
	byte texAniMap : 1;
	byte lodDontCollapse : 1;
	byte noCollDet : 1;
	byte forceOccluder : 1;
	byte m_bEnvironmentalMapping : 1;
	byte polyListNeedsSort : 1;
	byte matUsage : 8;
	byte libFlag : 8;
	zTRnd_AlphaBlendFunc rndAlphaBlendFunc : 8;
	byte m_bIgnoreSun : 1;

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
	word subType;
	bool32 inCutscene;
};

class oCNpcMessage : public zCEventMessage
{
public:
	zSTRING targetVobName;

	bool32 highPriority : 1;
	bool32 deleted : 1;
	bool32 inUse : 1;
};

class oCMsgWeapon : public oCNpcMessage
{
public:
	enum TWeaponSubType
	{
		EV_DRAWWEAPON,
		EV_DRAWWEAPON1,
		EV_DRAWWEAPON2,
		EV_REMOVEWEAPON,
		EV_REMOVEWEAPON1,
		EV_REMOVEWEAPON2,
		EV_CHOOSEWEAPON,
		EV_FORCEREMOVEWEAPON,
		EV_ATTACK,
		EV_EQUIPBESTWEAPON,
		EV_EQUIPBESTARMOR,
		EV_UNEQUIPWEAPONS,
		EV_UNEQUIPARMOR,
		EV_EQUIPARMOR,
		EV_WEAPON_MAX
	};

public:
	int targetMode;
	bool32 duringRun : 1;
	bool32 initDone : 1;
	bool32 firstTime : 1;
	bool32 useFist : 1;
	bool32 showMagicCircle : 1;

	int ani;

public:
	static oCMsgWeapon *_CreateNewInstance() { XCALL(0x007636E0); }
};

class zCEventManager : public zCObject
{
public:
	virtual void OnTrigger(zCVob *otherVob, zCVob *vobInstigator) { XCALL(0x007879F0); }
	virtual void OnUntrigger(zCVob *otherVob, zCVob *vobInstigator) { XCALL(0x00787AB0); }
	virtual void OnTouch(zCVob *otherVob) { XCALL(0x00787860); }
	virtual void OnUntouch(zCVob *otherVob) { XCALL(0x00787920); }
	virtual void OnTouchLevel() { XCALL(0x007879E0); }
	virtual void OnDamage(zCVob *otherVob, zCVob *inflictorVob, float damage, int damageType, zVEC3 &hitLocation) { XCALL(0x00787B70); }
	virtual void OnMessage(zCEventMessage *eventMessage, zCVob *sourceVob) { XCALL(0x00786380); }
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

	byte gravityOn : 1;
	byte collisionHad : 1;
	byte mode : 1;
	byte justSetSliding : 4;

public:
	void SetVelocity(zVEC3 &vel) { XCALL(0x005B66D0); }
};

class zCCollisionObjectDef
{
public:
	zCCollisionObject *(*m_createNewInstance)(void);
	bool32 m_bIsVolatile;
	int m_iIndex;
};

class zCCollObjectBoxPassThrough
{
public:
	static zCCollisionObjectDef &s_oCollObjClass;
};

class zCCollObjectProjectile
{
public:
	static zCCollisionObjectDef &s_oCollObjClass;
};

class zCVob : public zCObject
{
public:
	enum zTDynShadowType { };

	enum zTMovementMode { };

	enum zTVobCharClass
	{
		zVOB_CHAR_CLASS_NONE,
		zVOB_CHAR_CLASS_PC,
		zVOB_CHAR_CLASS_NPC
	};

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
	unsigned int groundShadowSizePacked;
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

	byte showVisual : 1;
	byte drawBBox3D : 1;
	byte visualAlphaEnabled : 1;
	byte physicsEnabled : 1;
	byte staticVob : 1;
	byte ignoredByTraceRay : 1;
	byte collDetectionStatic : 1;
	byte collDetectionDynamic : 1;
	byte castDynShadow : 2; // zTDynShadowType
	byte lightColorStatDirty : 1;
	byte lightColorDynDirty : 1;

	zTMovementMode isInMovementMode : 2;

	byte sleepingMode : 2; // zTVobSleepingMode
	byte mbHintTrafoLocalConst : 1;
	byte mbInsideEndMovementMethod : 1;

	zTVisualCamAlign visualCamAlign : 2;

	byte collButNoMove : 4;
	byte dontWriteIntoArchive : 1;
	byte bIsInWater : 1;
	byte bIsAmbientVob : 1;

	zCCollisionObjectDef *m_poCollisionObjectClass;
	zCCollisionObject *m_poCollisionObject;

public:
	static zCVob *_CreateNewInstance() { XCALL(0x005FD940); }

	zCVob() { XCALL(0x005FE1E0); }

	virtual zCClassDef *_GetClassDef() { XCALL(0x0043CCB0); }
	virtual void Archive(zCArchiver &arc) { XCALL(0x005FF140); }
	virtual void Unarchive(zCArchiver &arc) { XCALL(0x005FFC70); }
	virtual ~zCVob() { XCALL(0x005FE470); }

	virtual void OnTrigger(zCVob *otherVob, zCVob *vobInstigator) { }
	virtual void OnUntrigger(zCVob *otherVob, zCVob *vobInstigator) { }
	virtual void OnTouch(zCVob *otherVob) { }
	virtual void OnUntouch(zCVob *otherVob) { }
	virtual void OnTouchLevel() { }
	virtual void OnDamage(zCVob *otherVob, zCVob *inflictorVob, float damage, int damageType, zVEC3 &hitLocation) { }
	virtual void OnMessage(zCEventMessage *eventMessage, zCVob *sourceVob) { }
	virtual void OnTick() { }
	virtual void OnTImer() { }
	virtual void PostLoad() { }
	virtual zTVobCharClass GetCharacterClass() { return zVOB_CHAR_CLASS_NONE; }
	virtual void SetSleepingMode(zTVobSleepingMode smode) { XCALL(0x00602960); }
	virtual void EndMovement(bool32 a_bHintTrafoChanged) { XCALL(0x0061E0D0); }
	virtual bool32 CanThisCollideWith(zCVob *vob) { return TRUE; }
	virtual bool32 __fastcall Render(zTRenderContext &renderContext) { XCALL(0x006015D0); }
	virtual void SetVisual(zCVisual *v) { XCALL(0x006024F0); }
	virtual void SetVisual(zSTRING &visualFileName) { XCALL(0x00602680); }
	virtual bool32 GetScriptInstance(zSTRING *&scriptInstanceName, int &scriptInstanceIndex) { scriptInstanceName = NULL; scriptInstanceIndex = 0; return FALSE; }
	virtual bool32 SetByScriptInstance(zSTRING *scriptInstanceName, const int scriptInstanceIndex) { return FALSE; }
	virtual int GetCSStateFlags() { return 0; }
	virtual bool32 TraceRay(zVEC3 &rayOrigin, zVEC3 &ray, int traceFlags, zTTraceRayReport &report) { XCALL(0x005FFE40); }
	virtual zSTRING *GetTriggerTarget(int i) { return NULL; }
	virtual void ThisVobAddedToWorld(zCWorld *homeWorld) { XCALL(0x00601C80); }
	virtual void ThisVobRemovedFromWorld(zCWorld *homeWorld) { XCALL(0x00601CA0); }

	zCEventManager *__fastcall GetEM(bool32 dontCreate) { XCALL(0x005FFE10); }
	void SetSleeping(bool32 sleep) { if (sleep) this->SetSleepingMode(zVOB_SLEEPING); else this->SetSleepingMode(zVOB_AWAKE); }
	float GetDistanceToVob2(zCVob &v) { XCALL(0x0061BA40); }
	void SetPositionWorld(zVEC3 &posWorld) { XCALL(0x0061BB70); }
	void SetPhysicsEnabled(bool32 enable) { XCALL(0x0061D190); }
	void BeginMovement() { XCALL(0x0061DA80); }
	void SetCollisionClass(zCCollisionObjectDef *collClass) { XCALL(0x0061E610); }
	void RemoveVobFromWorld() { XCALL(0x00601C40); }
	void RotateLocalX(float angle) { XCALL(0x0061B6B0); }
	void RotateLocalY(float angle) { XCALL(0x0061B720); }
	void RotateLocalZ(float angle) { XCALL(0x0061B790); }
	void UpdateVisualDependencies(bool32 a_bHintTrafoChanged) { XCALL(0x00602B20); }
	void SetTrafo(zMAT4 &intrafo) { XCALL(0x0061BBD0); }
	void SetVobName(const zSTRING &n) { XCALL(0x005FFDD0); }
	void __fastcall SetCollDetStat(bool32 b) { XCALL(0x0061CE50); }
	void __fastcall SetCollDetDyn(bool32 b) { XCALL(0x0061CF40); }
	void SetCollDet(bool32 b) { SetCollDetStat(b); SetCollDetDyn(b); }
	void SetAI(zCAIBase *cbai) { XCALL(0x005FE8F0); }
	zVEC3 GetPositionWorld() { return trafoObjToWorld.GetTranslation(); }
	zMAT4 GetTrafoModelNodeToWorld(zCModelNodeInst *modelNode) { XCALL(0x00604A50); }
};

class zCVobLightData
{
public:
	zCArray<float> rangeAniScaleList;
	zCArray<zCOLOR> colorAniList;

	int lensFlareFXNo;
	zCLensFlareFX *lensFlareFX;

	zCOLOR lightColor;
	float range;
	float rangeInv;
	float rangeBackup;

	float rangeAniActFrame;
	float rangeAniFPS;

	float colorAniActFrame;
	float colorAniFPS;

	float spotConeAngleDeg;

	byte isStatic : 1;
	byte rangeAniSmooth : 1;
	byte rangeAniLoop : 1;
	byte colorAniSmooth : 1;
	byte colorAniLoop : 1;
	byte isTurnedOn : 1;
	byte lightQuality : 4;
	byte lightType : 4;
	byte m_bCanMove : 1;
};

class zCVobLight : public zCVob
{
public:
	zCVobLightData lightData;
	zTRayTurboValMap<zCPolygon *, int> affectedPolyMap;
	zSTRING lightPresetInUse;

public:
	void SetRange(float r, bool32 doBackup) { XCALL(0x00608320); }
};

class zCCamera
{
public:
	struct zTCamVertSimple
	{
		float x;
		float y;
		float z;
		zVEC2 texuv;
		zCOLOR color;
	};

public:
	static zCCamera *&activeCam;

	zTPlane frustumplanes[6];
	BYTE signbits[6];

	zTViewPortData vpData;
	zCViewBase *targetView;

	zMAT4 camMatrix;
	zMAT4 camMatrixInv;

	bool32 tremorToggle;
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

	bool32 screenFadeEnabled;
	zCOLOR screenFadeColor;
	zSTRING screenFadeTexture;
	float screenFadeTextureAniFps;
	zTRnd_AlphaBlendFunc screenFadeTextureBlendFunc;
	bool32 cinemaScopeEnabled;
	zCOLOR cinemaScopeColor;

	int projection;
	int drawMode;
	int shadeMode;
	bool32 drawWire;

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
	bool32 decal2Sided;
	bool32 ignoreDayLight;
	bool32 m_bOnTop;

public:
	virtual bool32 Render(zTRenderContext &renderContext) { XCALL(0x00556990); }

	void SetDecalDim(float xd, float yd) { this->xdim = xd; this->ydim = yd; }
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
	bool32 heightCheck;
	bool32 everyFrameUpdate;
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

	byte localFOR : 1;

public:
	virtual bool32 Render(zTRenderContext &renderContext) { XCALL(0x005BDC70); }

	void SetVisibleSegments(float visibleFirst, float visibleLast) { XCALL(0x005BDB90); }
};

typedef int zTSoundHandle;

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

	bool32 bFadeInOut;
	int fadeState;

	bool32 fadeIn;
	bool32 fadeOut;
	bool32 bFlashing;
	float fFlashTime;

	zCSoundFX *sfx1;
	zTSoundHandle sfxHandle1;
	zCSoundFX *sfx2;
	zTSoundHandle sfxHandle2;
	zCSoundFX *sfx3;
	zTSoundHandle sfxHandle3;
	zCSoundFX *sfx4;
	zTSoundHandle sfxHandle4;

	zCDecal *thunderStartDecal;

	bool32 activeThunder_Sector1;
	bool32 activeThunder_Sector2;
	bool32 activeThunder_Sector3;
	bool32 activeThunder_Sector4;

	zVEC2 *originalTexUVList;

public:
	void Init() { XCALL(0x006B9440); }
	void Initialise(int newNumMaxThunders) { XCALL(0x006B9BF0); }
	void AddTremor(zTRenderContext &renderContext) { XCALL(0x006B9CE0); }
	void RenderLayer(zTRenderContext &rndContext, int layerNumber, int &addNewThunder) { XCALL(0x006B9CF0); }
	bool32 Render(zTRenderContext &rndContext, bool32 fadeInOut, bool32 alwaysVisible) { XCALL(0x006B9F30); }
	void RemoveThunder(myThunder *thunder) { XCALL(0x006BA9F0); }
	int AddThunderSub(myThunder *rootThunder, int startIndex, int startNexIntex, int length, int mumSplits) { XCALL(0x006BAAA0); }
	int AddThunder(int startIndex, int length, float random, int sector) { XCALL(0x006BADE0); }
	bool32 RenderThunder(myThunder *thunder, zTRenderContext &rndContext) { XCALL(0x006BB4B0) }
};

class zCRenderer
{
public:
	virtual ~zCRenderer() { }
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void FlushPolys() = 0;
	virtual void DrawPoly(zCPolygon *poly) = 0;
	virtual void DrawLightmapList(zCPolygon **polyList, int numPoly) = 0;
	virtual void DrawLine(float x1, float x2, float y1, float y2, zCOLOR col) = 0;
	virtual void DrawLineZ(float x1Scr, float y1Scr, float z1CamSpaceInv, float x2Scr, float y2Scr, float z2CamSpaceInv, zCOLOR col) = 0;
	virtual void SetPixel(float x, float y, zCOLOR col) = 0;
	virtual void DrawPolySimple(zCTexture *texture, zTRndSimpleVertex *vertex, int numVert) = 0;
	virtual void SetFog(bool32 foggy) = 0;
	virtual bool32 GetFog() = 0;
	virtual void SetRadialFog(bool32 foggy) = 0;
	virtual bool32 GetRadialFog() = 0;
	virtual void SetFogColor(zCOLOR &col) = 0;
	virtual zCOLOR GetFogColor() = 0;
	virtual void SetFogRange(float nearz, float farz, int falloff) = 0;
	virtual void GetFogRange(float &nearz, float &farz, int &falloff) = 0;
	virtual int GetPolyDrawMode() = 0;
	virtual void SetPolyDrawMode(int &drawMode) = 0;
	virtual bool32 GetSurfaceLost() = 0;
	virtual void SetSurfaceLost(bool32 b) = 0;
	virtual bool32 GetSyncOnAmbientCol() = 0;
	virtual void SetSyncOnAmbientCol(bool32 b) = 0;
	virtual void SetTextureWrapEnabled(bool32 b) = 0;
	virtual bool32 GetTextureWrapEnabled() = 0;
	virtual void SetBilerpFilterEnabled(bool32 b) = 0;
	virtual bool32 GetBilerpFilterEnabled() = 0;
	virtual void SetDitherEnabled(bool32 b) = 0;
	virtual bool32 GetDitherEnabled() = 0;
	virtual int GetPolySortMode() = 0;
	virtual void SetPolySortMode(int &smode) = 0;
	virtual bool32 GetZBufferWriteEnabled() = 0;
	virtual void SetZBufferWriteEnabled(bool32 flag) = 0;

	zTRnd_AlphaBlendFunc AlphaBlendFuncStringToType(const zSTRING &s) { XCALL(0x005D38B0); }
};

class zCRnd_D3D : public zCRenderer
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
	virtual void SetFog(bool32 foggy) { XCALL(0x00651E80); }
	virtual bool32 GetFog() { XCALL(0x00652070); }
	virtual void SetRadialFog(bool32 foggy) { XCALL(0x00652010); }
	virtual bool32 GetRadialFog() { XCALL(0x0064A400); }
	virtual void SetFogColor(zCOLOR &col) { XCALL(0x00652080); }
	virtual zCOLOR GetFogColor() { XCALL(0x006521D0); }
	virtual void SetFogRange(float nearz, float farz, int falloff) { XCALL(0x006521E0); }
	virtual void GetFogRange(float &nearz, float &farz, int &falloff) { XCALL(0x006522D0); }
	virtual int GetPolyDrawMode() { XCALL(0x00652310); }
	virtual void SetPolyDrawMode(int &drawMode) { XCALL(0x00652300); }
	virtual bool32 GetSurfaceLost() { XCALL(0x0064A470); }
	virtual void SetSurfaceLost(bool32 b) { XCALL(0x0064A480); }
	virtual bool32 GetSyncOnAmbientCol() { XCALL(0x0064A490); }
	virtual void SetSyncOnAmbientCol(bool32 b) { XCALL(0x0064A4A0); }
	virtual void SetTextureWrapEnabled(bool32 b) { XCALL(0x00652320); }
	virtual bool32 GetTextureWrapEnabled() { XCALL(0x00652330); }
	virtual void SetBilerpFilterEnabled(bool32 b) { XCALL(0x00652340); }
	virtual bool32 GetBilerpFilterEnabled() { XCALL(0x006523D0); }
	virtual void SetDitherEnabled(bool32 b) { XCALL(0x006523E0); }
	virtual bool32 GetDitherEnabled() { XCALL(0x006523F0); }
	virtual int GetPolySortMode() { XCALL(0x006524D0); }
	virtual void SetPolySortMode(int &smode) { XCALL(0x00652400); }
	virtual bool32 GetZBufferWriteEnabled() { XCALL(0x00652520); }
	virtual void SetZBufferWriteEnabled(bool32 flag) { XCALL(0x006524E0); }
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
	bool32 sunOn;
	bool32 cloudShadowOn;

	zCSkyLayerData layer[2];
};

class zCSkyControler : public zCObject
{
public:
	static int &s_skyEffectsEnabled;

	zCOLOR *polyLightCLUTPtr;
	float cloudShadowScale;
	bool32 m_bColorChanged;
	int m_enuWeather;

	zCOLOR backgroundColor;
	bool32 fillBackground;
	zCTexture *backgroundTexture;
	int relightCtr;
	float lastRelightTime;
	float m_fRelightTime;
};

class zCSkyControler_Mid : public zCSkyControler
{
public:
	bool32 underwaterFX;
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
	zTRnd_AlphaBlendFunc scrPolyAlphaFunc;
};

class zCSkyControler_Outdoor : public zCSkyControler_Mid
{
public:
	struct zTRainFX
	{
		zCOutdoorRainFX *outdoorRainFX;
		int camLocationHint;
		float outdoorRainFXWeight;
		float soundVolume;
		float timerInsideSectorCantSeeOutside;
		float timeStartRain;
		float timeStopRain;
		bool32 renderLightning;
		bool32 m_bRaining;
		int m_iRainCtr;
	};

public:
	static zCClassDef &classDef;

	bool32 initDone;
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
	bool32 m_bSkyScaleChanged;
	zVEC3 m_overrideColor;
	bool32 m_bOverrideColorFlag;
	bool32 m_bDontRain;
	bool32 m_bLevelChanged;
	bool32 m_bDarkSky;

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
	bool32 m_bSunVisible;
	float m_fFadeScale;

	zCVob *vobSkyPfx;
	float skyPFXTimer;

	bool32 m_bIsMainControler;
	zVEC3 m_bWindVec;

	zTRainFX rainFX;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	void ReadFogColorsFromINI() { XCALL(0x005E7EA0); }
	void RenderSkyPre() { XCALL(0x005EA850); }
};

class oCSkyControler_Barrier : public zCSkyControler_Outdoor
{
public:
	oCBarrier *barrier;
	bool32 bFadeInOut;

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
	bool32 drawVobBBox3D;
	int leafsRendered;
	int vobsRendered;
	bool32 m_bRenderedFirstTime;
	int masterFrameCtr;
	zCPolygon **actPolyPtr;
	bool32 compiled;
};

class zCVobBBox3DSorter
{
public:
	zCArray<zCBBox3DSorterBase::zTBoxSortHandle *> handles;
	zCArraySort<zTNode *> nodeList[3];
	bool32 sorted;

public:
	virtual ~zCVobBBox3DSorter(){ XCALL(0x006288D0); }
};

enum zTTraceRayFlags
{
	zTRACERAY_VOB_IGNORE_NO_CD_DYN = 1 << 0,
	zTRACERAY_VOB_IGNORE = 1 << 1,
	zTRACERAY_VOB_BBOX = 1 << 2,
	zTRACERAY_VOB_OBB = 1 << 3,
	zTRACERAY_STAT_IGNORE = 1 << 4,
	zTRACERAY_STAT_POLY = 1 << 5,
	zTRACERAY_STAT_PORTALS = 1 << 6,
	zTRACERAY_POLY_NORMAL = 1 << 7,
	zTRACERAY_POLY_IGNORE_TRANSP = 1 << 8,
	zTRACERAY_POLY_TEST_WATER = 1 << 9,
	zTRACERAY_POLY_2SIDED = 1 << 10,
	zTRACERAY_VOB_IGNORE_CHARACTER = 1 << 11,
	zTRACERAY_FIRSTHIT = 1 << 12,
	zTRACERAY_VOB_TEST_HELPER_VISUALS = 1 << 13,
	zTRACERAY_VOB_IGNORE_PROJECTILES = 1 << 14
};

class zCWorld : public zCObject
{
public:
	zCTree<zCVob> globalVobTree;

	zTTraceRayReport traceRayReport;

	zCSession *ownerSession;
	zCCSPlayer *csPlayer;

	zSTRING m_strlevelName;
	bool32 compiled;
	bool32 compiledEditorMode;
	bool32 traceRayIgnoreVobFlag;
	bool32 m_bIsInventoryWorld;
	int worldRenderMode;
	zCWayNet *wayNet;
	int masterFrameCtr;
	float vobFarClipZ;
	float vobFarClipZScalability;

	zCArray<zCVob *> traceRayVobList;
	zCArray<zCVob *> traceRayTempIgnoreVobList;

	bool32 renderingFirstTime;
	bool32 showWaynet;
	bool32 showTraceRayLines;

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
	zCBBox3DSorterBase::zTBoxSortHandle zoneActiveHandle;

	bool32 addZonesToWorld;
	bool32 showZonesDebugInfo;

	zCCBspTree *cbspTree;
	zCBspTree bspTree;

	zCArray<zCVob *> activeVobList;
	zCArray<zCVob *> walkList;

	zCArray<zCVob *> vobHashTable[2048];

public:
	bool32 __fastcall TraceRayFirstHit(zVEC3 &rayOrigin, zVEC3 &ray, zCArray<zCVob *> *ignoreVobList, int traceFlags) { XCALL(0x00621960); }
	bool32 __fastcall TraceRayFirstHit(zVEC3 &rayOrigin, zVEC3 &ray, zCVob *ignoreVob, int traceFlags) { XCALL(0x00621E70); }
	bool32 __fastcall TraceRayNearestHit(zVEC3 &rayOrigin, zVEC3 &ray, zCArray<zCVob *> *ignoreVobList, int traceFlags) { XCALL(0x006220D0); }
	bool32 __fastcall TraceRayNearestHit(zVEC3 &rayOrigin, zVEC3 &ray, zCVob *ignoreVob, int traceFlags) { XCALL(0x00621FA0); }
	zCTree<zCVob *> *AddVob(zCVob *vob) { XCALL(0x00624810); }
};

class zCCSPlayer
{
public:
	zCCSCutsceneContext *GetPlayingGlobalCutscene() { XCALL(0x00420770); }
};

#define zSND_SLOT_NONE 0

class zCSoundSystem
{
public:
	struct zTSound3DParams
	{
		float obstruction;
		float volume;
		float radius;
		int loopType;
		float coneAngleDeg;
		float reverbLevel;
		bool32 isAmbient3D;
		float pitchOffset;

		void SetDefaults() { obstruction = 0.0f; volume = 1.0f; radius = -1.0f; loopType = 0; coneAngleDeg = 0.0f; reverbLevel = 1.0f; isAmbient3D = FALSE; pitchOffset = -999999.0f; }
	};

public:
	virtual ~zCSoundSystem() { }
	virtual zCSoundFX *LoadSoundFX(zSTRING &fileName) = 0;
	virtual zCSoundFX *LoadSoundFXScript(zSTRING &scriptIdentifier) = 0;
	virtual zCParser *GetSFXParser() = 0;
	virtual float GetPlayingTimeMSEC(zSTRING &fileName) = 0;
	virtual void SetSound3DDefaultRadius(float defRad) = 0;
	virtual float GetSound3DDefaultRadius() = 0;
	virtual void SetMasterVolume(float vol) = 0;
	virtual float GetMasterVolume() = 0;
	virtual zTSoundHandle PlaySound(zCSoundFX *sfx, int slot) = 0;
	virtual zTSoundHandle PlaySound(zCSoundFX *sfx, int slot, int freq, float vol, float pan) = 0;
	virtual zTSoundHandle PlaySound3D(zCSoundFX *sfx, zCVob *sourceVob, int vobSlot, zTSound3DParams *sound3DParams) = 0;
	virtual zTSoundHandle PlaySound3D(zSTRING &soundName, zCVob *sourceVob, int vobSlot, zTSound3DParams *sound3DParams) = 0;
	virtual void StopSound(zTSoundHandle &sfxHandle) = 0;
	virtual void StopAllSounds() = 0;
	virtual bool32 GetSound3DProps(zTSoundHandle &sfxHandle, zTSound3DParams &sound3DParams) = 0;
	virtual bool32 UpdateSound3D(zTSoundHandle &sfxHandle, zTSound3DParams *sound3DParams) = 0;
	virtual void GetSoundProps(zTSoundHandle &sfxHandle, int &freq, float &vol, float &pan) = 0;
	virtual void UpdateSoundProps(zTSoundHandle &sfxHandle, int freq, float vol, float pan) = 0;
	virtual bool32 IsSoundActive(zTSoundHandle &sfxHandle) = 0;
};

class zCSndSys_MSS : public zCSoundSystem
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
	virtual void StopSound(zTSoundHandle &sfxHandle) { XCALL(0x004F2300); }
	virtual void StopAllSounds() { XCALL(0x004F23C0); }
	virtual bool32 GetSound3DProps(zTSoundHandle &sfxHandle, zTSound3DParams &sound3DParams) { XCALL(0x004F3780); }
	virtual bool32 UpdateSound3D(zTSoundHandle &sfxHandle, zTSound3DParams *sound3DParams) { XCALL(0x004F2410); }
	virtual void GetSoundProps(zTSoundHandle &sfxHandle, int &freq, float &vol, float &pan) { XCALL(0x004F3580); }
	virtual void UpdateSoundProps(zTSoundHandle &sfxHandle, int freq, float vol, float pan) { XCALL(0x004F3970); }
	virtual bool32 IsSoundActive(zTSoundHandle &sfxHandle) { XCALL(0x004F3FD0); }
};

class zCOption
{
public:
	bool32 ReadBool(zSTRING &secName, char *name, bool32 value) { XCALL(0x00462160); }
};

class zCInputCallback
{
public:
	virtual bool32 HandleEvent(int key) { XCALL(0x0043D4E0); }
};

class zCSoundManager
{
public:
	enum zTSndManMedium { };

	struct zTScriptSoundData
	{
		zSTRING file;
		int pitchOff;
		int pitchVar;
		int vol;
		int loop;
		float reverbLevel;
		zSTRING pfxName;
	};
};

class zCBspBase
{
public:
	void __fastcall CollectVobsInBBox3D(zCArray<zCVob *> &resVobList, zTBBox3D &inbbox3D) { XCALL(0x00531110); }
};

struct TNpcAIState
{
	int index;
	int loop;
	int end;
	int timeBehaviour;
	float restTime;
	int phase;
	bool32 valid;
	zSTRING name;
	float stateTime;
	int prgIndex;
	bool32 isRtnState;
};

struct TNpcPerc
{
	int percID;
	int percFunc;
};

struct TNpcSlot
{
	zSTRING name;
	bool32 inInventory;
	bool32 tmpLevel;
	zSTRING itemName;

	oCVob *vob;

	bool32 wasVobTreeWhenInserted : 1;
};

enum oTSndMaterial
{
	SND_MAT_WOOD,
	SND_MAT_STONE,
	SND_MAT_METAL,
	SND_MAT_LEATHER,
	SND_MAT_CLAY,
	SND_MAT_GLAS
};

class oCVob : public zCVob
{
public:
	virtual void Init() { }
	virtual void ShowDebugInfo(zCCamera *) { XCALL(0x0077B8D0); }
	virtual int GetInstance() { return -1; }
	virtual zSTRING GetInstanceName() { return ""; }
	virtual bool32 IsOwnedByGuild(int guild) { return FALSE; }
	virtual bool32 IsOwnedByNpc(int instance) { return FALSE; }
	virtual bool32 DoFocusCheckBBox() { return FALSE; }
	virtual oCAIVobMove *GetAIVobMove() { XCALL(0x0077D5F0); }
	virtual void GetSoundMaterial_AM(zCSoundManager::zTSndManMedium &med2, oTSndMaterial &mat2, int damage) { }
	virtual void SetSoundMaterial(oTSndMaterial mat) { }
	virtual oTSndMaterial GetSoundMaterial() { return SND_MAT_WOOD; }
};

class oCItem : public oCVob
{
public:
	static zCClassDef &classDef;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	bool32 MultiSlot() { XCALL(0x007125A0); }
};

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

class oCGame : public zCSession
{
public:
	struct TObjectRoutine
	{
		zSTRING objName;
		int stateNum;
		int hour1;
		int min1;
		int type;
	};

public:
	float cliprange;
	float fogrange;

	bool32 inScriptStartup;
	bool32 inLoadSaveGame;
	bool32 inLevelChange;

	zCView *array_view[6];
	bool32 array_view_visible[6];
	bool32 array_view_enabled[6];

	oCSavegameManager *savegameManager;
	zCView *game_text;
	zCView *load_screen;
	zCView *save_screen;
	zCView *pause_screen;
	oCViewStatusBar *hpBar;
	oCViewStatusBar *swimBar;
	oCViewStatusBar *manaBar;
	oCViewStatusBar *focusBar;
	bool32 showPlayerStatus;

	bool32 game_drawall;
	bool32 game_frameinfo;
	bool32 game_showaniinfo;
	bool32 game_showwaynet;
	bool32 game_testmode;
	bool32 game_editwaynet;
	bool32 game_showtime;
	bool32 game_showranges;
	bool32 drawWayBoxes;
	bool32 scriptStartup;
	bool32 showFreePoints;
	oCNpc *showRoutineNpc;

	bool32 loadNextLevel;
	zSTRING loadNextLevelName;
	zSTRING loadNextLevelStart;

	zVEC3 startpos;
	oCGameInfo *gameInfo;
	zCVobLight *pl_light;
	float pl_lightval;

	oCWorldTimer *wldTimer;
	float timeStep;
	bool32 singleStep;

	oCGuilds *guilds;
	oCInfoManager *infoman;
	oCNewsManager *newsman;
	oCSVMManger *svmman;
	oCTradeManager *trademan;
	oCPortalRoomManager *portalman;
	oCSpawnManager *spawnman;

	float music_delay;
	oCNpc *watchnpc;

	bool32 m_bWorldEntered;
	float m_fEnterWorldTimer;

	int initial_hour;
	int initial_minute;

	zCArray<zCVob *> debugInstances;
	int debugChannels;
	bool32 debugAllInstances;

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

	byte isWarning : 1;
	byte isShooting : 1;

public:
	void DoCheck() { XCALL(0x00473740); }
	void DoWarningFX(int level) { XCALL(0x00474CB0); }
	void DisposeWarningFX() { XCALL(0x00474E70); }
	void DoShootFX(zVEC3 &startPoint) { XCALL(0x00474EB0); }
	void DisposeShootFX() { XCALL(0x004751A0); }
};

class oCNpc_States
{
public:
	zSTRING name;

	oCNpc *npc;
	TNpcAIState curState;
	TNpcAIState nextState;
	int lastAIState;

	bool32 hasRoutine;
	bool32 rtnChanged;
	oCRtnEntry *rtnBefore;
	oCRtnEntry *rtnNow;
	zCRoute *rtnRoute;
	bool32 rtnOverlay;
	int rtnOverlayCount;
	int walkmode_routine;
	bool32 weaponmode_routine;
	bool32 startNewRoutine;

	int aiStateDriven;
	zVEC3 aiStatePosition;

	oCNpc *parOther;
	oCNpc *parVictim;
	oCItem *parItem;

	int rtnChangeCount;

public:
	virtual void Archive(zCArchiver &arc) { XCALL(0x0076EE70); }

	bool32 StartAIState(zSTRING &name, int endOldState, int timeBehaviour, float timed, bool32 isRtnState) { XCALL(0x0076C700); }
	void ClearAIState() { XCALL(0x0076D6E0); }
	void EndCurrentState() { XCALL(0x0076D880); }
	bool32 IsInState(int stateID) { XCALL(0x0076C040); }
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
	bool32 frame;
	bool32 right;
	bool32 ownList;
	bool32 prepared;
	bool32 passive;
	short TransferCount;

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
	bool32 m_bManipulateItemsDisabled;
	bool32 m_bCanTransferMoreThanOneItem;
};

class oCNpcInventory : public oCItemContainer
{
public:
	oCNpc *owner;
	bool32 packAbility;

	zCListSort<oCItem> inventory;

	zSTRING packString;
	int maxSlots;

public:
	bool32 IsEmpty(bool32 bIgnoreArmor, bool32 bIgnoreActive) { XCALL(0x0070D1A0); }
};

class oCNewsMemory
{
public:
	zCList<oCNews> iknow;

public:
	virtual void Archive(zCArchiver &arc) { XCALL(0x0072A140); }
};

enum oEIndexDamage
{
	oEDamageIndex_Barrier,
	oEDamageIndex_Blunt,
	oEDamageIndex_Edge,
	oEDamageIndex_Fire,
	oEDamageIndex_Fly,
	oEDamageIndex_Magic,
	oEDamageIndex_Point,
	oEDamageIndex_Fall,
	oEDamageIndex_MAX
};

typedef oEIndexDamage oEDamageIndex;
typedef oEIndexDamage oEProtectionIndex;

enum oETypeDamage
{
	oEDamageType_Unknown = 0,
	oEDamageType_Barrier = 1,
	oEDamageType_Blunt = oEDamageType_Barrier << 1,
	oEDamageType_Edge = oEDamageType_Blunt << 1,
	oEDamageType_Fire = oEDamageType_Edge << 1,
	oEDamageType_Fly = oEDamageType_Fire << 1,
	oEDamageType_Magic = oEDamageType_Fly << 1,
	oEDamageType_Point = oEDamageType_Magic << 1,
	oEDamageType_Fall = oEDamageType_Point << 1,
	oEDamageType_ForceDWORD = 0xffffffff
};

typedef oETypeDamage oEDamageType;

#define BS_MOD_CONTROLLED 2048
#define BS_MOD_CONTROLLING 8192

#define NPC_PERC_ASSESSCASTER 29
#define NPC_PERC_MAX 33

#define NPC_FLAG_IMMORTAL (1 << 1)
#define NPC_FLAG_GHOST (1 << 2)

#define NPC_ATR_HITPOINTS 0
#define NPC_ATR_HITPOINTSMAX 1
#define NPC_ATR_MANA 2
#define NPC_ATR_MANAMAX 3
#define NPC_ATR_STRENGTH 4
#define NPC_ATR_DEXTERITY 5
#define NPC_ATR_REGENERATEHP 6
#define NPC_ATR_REGENERATEMANA 7
#define NPC_ATR_MAX 8

#define NPC_AISTATE_UNCONSCIOUS (-4)
#define NPC_AISTATE_FADEAWAY (-5)

#define NPC_WEAPON_NONE 0
#define NPC_WEAPON_FIST 1
#define NPC_WEAPON_DAG 2
#define NPC_WEAPON_1HS 3
#define NPC_WEAPON_2HS 4
#define NPC_WEAPON_BOW 5
#define NPC_WEAPON_CBOW 6
#define NPC_WEAPON_MAG 7
#define NPC_WEAPON_MAX 8

#define NPC_TAL_INVALID (-1)
#define NPC_TAL_UNKNOWN 0
#define NPC_TAL_1H 1
#define NPC_TAL_2H 2
#define NPC_TAL_BOW 3
#define NPC_TAL_CROSSBOW 4
#define NPC_TAL_PICKLOCK 5
#define NPC_TAL_PICKPOCKET 6
#define NPC_TAL_MAGE 7
#define NPC_TAL_SNEAK 8
#define NPC_TAL_REGENERATE 9
#define NPC_TAL_FIREMASTER 10
#define NPC_TAL_ACROBAT 11
// #define NPC_TAL_PICKPOCKET 12
#define NPC_TAL_SMITH 13
#define NPC_TAL_RUNES 14
#define NPC_TAL_ALCHEMY 15
#define NPC_TAL_TAKEANIMALTROPHY 16
#define NPC_TAL_FOREIGNLANGUAGE 17
#define NPC_TAL_WISPDETECTOR 18
#define NPC_TAL_C 19
#define NPC_TAL_D 20
#define NPC_TAL_E 21
#define NPC_TAL_MAX 22

#define NPC_HITCHANCE_MAX 5

#define NPC_MIS_MAX 5

#define NPC_GIL_SKELETON 31
#define NPC_GIL_SUMMONED_SKELETON 32
#define NPC_GIL_SKELETON_MAGE 33

class oCNpc : public oCVob
{
public:
	struct oTRobustTrace
	{
		byte stand : 1;
		byte dirChoosed : 1;
		byte exactPosition : 1;
		byte targetReached : 1;
		byte standIfTargetReached : 1;
		byte waiting : 1;
		byte isObstVobSmall : 1;
		byte targetVisible : 1;
		byte useChasmChecks : 1;

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

	class oCNpcTimedOverlay
	{
	public:
		zSTRING mdsOverlayName;
		float timer;
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
		unsigned int aryDamage[oEDamageIndex_MAX];
		float fDamageTotal;
		float fDamageMultiplier;
		zVEC3 vecLocationHit;
		zVEC3 vecDirectionFly;
		zSTRING strVisualFX;
		float fTimeDuration;
		float fTimeInterval;
		float fDamagePerInterval;
		bool32 bDamageDontKill;

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
		unsigned int aryDamageEffective[oEDamageIndex_MAX];
		zCVob *pVobParticleFX;
		zCParticleFX *pParticleFX;
		oCVisualFX *pVisualFX;
	};

public:
	static zCClassDef &classDef;
	static oCNpc *&player;
	static bool32 &godmode;

	int idx;
	zSTRING name[5];
	zSTRING slot;
	zSTRING effect;

	int npcType;
	int variousFlags;
	int attribute[NPC_ATR_MAX];
	int hitChance[NPC_HITCHANCE_MAX];
	int protection[oEDamageIndex_MAX];
	int damage[oEDamageIndex_MAX];
	int damagetype;
	int guild;
	int level;
	int mission[NPC_MIS_MAX];
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
	bool32 noFocus;
	int parserEnd;

	bool32 bloodEnabled;
	int bloodDistance;
	int bloodAmount;
	int bloodFlow;
	zSTRING bloodEmitter;
	zSTRING bloodTexture;
	bool32 didHit;
	bool32 didParade;
	bool32 didShoot;
	bool32 hasLockedEnemy;
	bool32 isDefending;
	bool32 wasAming;
	bool32 lastAction;
	oCNpc *enemy;

	float speedTurn;
	bool32 foundFleePoint;
	bool32 reachedFleePoint;
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

	bool32 showaidebug : 1;
	bool32 showNews : 1;
	bool32 csAllowedAsRole : 1;

	bool32 isSummoned : 1;
	bool32 respawnOn : 1;
	bool32 movlock : 1;
	bool32 drunk : 1;
	bool32 mad : 1;
	bool32 overlay_wounded : 1;
	bool32 inOnDamage : 1;
	bool32 autoremoveweapon : 1;
	bool32 openinventory : 1;
	bool32 askroutine : 1;
	bool32 spawnInRange : 1;

	int body_TexVarNr : 16;
	int body_TexColorNr : 16;
	int head_TexVarNr : 16;
	int teeth_TexVarNr : 16;
	int guildTrue : 8;
	int drunk_heal : 8;
	int mad_heal : 8;
	int spells : 8;
	int bodyState : 19;
	bool32 m_bAniMessageRunning : 1;

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
	bool32 knowsPlayer;

	TNpcPerc percList[NPC_PERC_MAX];
	int percActive;
	float percActiveTime;
	float percActiveDelta;

	bool32 overrideFallDownHeight;
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
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	void OnDamage_Hit(oSDamageDescriptor &descDamage) { XCALL(0x00666610); }
	void OnDamage_Events(oSDamageDescriptor &descDamage) { XCALL(0x0067ABE0); }
	void StandUp(bool32 walkingallowed, bool32 startAniTransition) { XCALL(0x00682B40); }
	zCModel *GetModel() { XCALL(0x00738720); }
	oCSpell *IsSpellActive(int nr) { XCALL(0x0073D340); }
	void InsertActiveSpell(oCSpell *spell) { XCALL(0x0073D070); }
	bool32 CheckForOwner(zCVob *v) { XCALL(0x007405F0); }
	void SetAsPlayer() { XCALL(0x007426A0); }
	void ClearVobList() { XCALL(0x0075D7F0); }
	void CreateVobList(float max_dist) { XCALL(0x0075DA40); }
	bool32 HasBodyStateModifier(int nr) { XCALL(0x0075EBF0); }
	void SetBodyStateModifier(int nr) { XCALL(0x0075EC10); }
	int ModifyBodyState(int add, int remove) { XCALL(0x0075EF50); }
	void CreatePassivePerception(int percType, zCVob *other, zCVob *victim) { XCALL(0x0075B270); }
	bool32 IsDead() { return this->attribute[NPC_ATR_HITPOINTS] <= 0; }
	bool32 IsUnconscious() { return this->state.IsInState(NPC_AISTATE_UNCONSCIOUS); }
	bool32 IsFadingAway() { return this->state.IsInState(NPC_AISTATE_FADEAWAY); }
	bool32 IsHuman() { XCALL(0x00742640); }
	bool32 IsOrc() { XCALL(0x00742670); }
	bool32 IsGoblin() { XCALL(0x00742650); }
	bool32 IsSkeleton() { XCALL(0x00742680); }
	bool32 IsMonster() { XCALL(0x00742600); }
	bool32 IsHalfMonster() { return !this->IsHuman() && !this->IsOrc() && !this->IsGoblin() && !this->IsSkeleton(); }
	int GetWeaponMode() { XCALL(0x00738C40); }
	int GetTalentValue(int talentNr) { XCALL(0x00730DC0); }
	bool32 HasFlag(unsigned int dwValue, unsigned int dwFlag) { XCALL(0x0067BD20); }
	bool32 HasFlag(int nr) { XCALL(0x007309E0); }
	bool32 IsSelfPlayer() { return this == oCNpc::player; }
	void ChangeAttribute(int nr, int value) { XCALL(0x0072FF60); }
	bool32 AssessMagic_S(oCNpc *other, int spellType) { XCALL(0x0075CC30); }
	bool32 AssessStopMagic_S(oCNpc *other, int spellType) { XCALL(0x0075CF30); }
	oCItem *RemoveFromInv(oCItem *item, int num) { XCALL(0x007495A0); }
};

enum zTPFX_EmitterShape { };
enum zTPFX_EmitterFOR { };
enum zTPFX_DistribType { };
enum zTPFX_EmitterDirMode { };
enum zTPFX_EmitterVisOrient { };
enum zTPFX_FlockMode { };

class zCParticleEmitter
{
public:
	float ppsValue;
	zSTRING ppsScaleKeys_S;
	bool32 ppsIsLooping;
	bool32 ppsIsSmooth;
	float ppsFPS;
	zSTRING ppsCreateEm_S;
	float ppsCreateEmDelay;

	zSTRING shpType_S;
	zSTRING shpFOR_S;
	zSTRING shpOffsetVec_S;
	zSTRING shpDistribType_S;
	float shpDistribWalkSpeed;
	bool32 shpIsVolume;
	zSTRING shpDim_S;
	zSTRING shpMesh_S;
	bool32 shpMeshRender_B;
	zSTRING shpScaleKeys_S;
	bool32 shpScaleIsLooping;
	bool32 shpScaleIsSmooth;
	float shpScaleFPS;

	zSTRING dirMode_S;
	zSTRING dirFOR_S;
	zSTRING dirModeTargetFOR_S;
	zSTRING dirModeTargetPos_S;
	float dirAngleHead;
	float dirAngleHeadVar;
	float dirAngleElev;
	float dirAngleElevVar;
	float velAvg;
	float velVar;

	float lspPartAvg;
	float lspPartVar;
	zSTRING flyGravity_S;
	bool32 flyCollDet_B;

	zSTRING visName_S;
	zSTRING visOrientation_S;
	bool32 visTexIsQuadPoly;
	float visTexAniFPS;
	bool32 visTexAniIsLooping;

	zSTRING visTexColorStart_S;
	zSTRING visTexColorEnd_S;

	zSTRING visSizeStart_S;
	float visSizeEndScale;

	zSTRING visAlphaFunc_S;
	float visAlphaStart;
	float visAlphaEnd;

	float trlFadeSpeed;
	zSTRING trlTexture_S;
	float trlWidth;

	float mrkFadeSpeed;
	zSTRING mrkTexture_S;
	float mrkSize;

	zSTRING m_flockMode_S;
	float m_fFlockWeight;

	bool32 m_bSlowLocalFOR;

	zSTRING m_timeStartEnd_S;
	bool32 m_bIsAmbientPFX;

	int endOfDScriptPart;

	zSTRING particleFXName;

	zCArray<float> ppsScaleKeys;
	zCParticleEmitter *ppsCreateEmitter;

	zTPFX_EmitterShape shpType;
	float shpCircleSphereRadius;
	zVEC3 shpLineBoxDim;
	zVEC3 *shpMeshLastPolyNormal;
	zCMesh *shpMesh;
	zCProgMeshProto *shpProgMesh;
	zCModel *shpModel;

	zTPFX_EmitterFOR shpFOR;
	zTPFX_DistribType shpDistribType;
	zVEC3 shpOffsetVec;
	zCArray<float> shpScaleKeys;

	zTPFX_EmitterDirMode dirMode;
	zTPFX_EmitterFOR dirFOR;
	zTPFX_EmitterFOR dirModeTargetFOR;
	zVEC3 dirModeTargetPos;
	zTBBox3D dirAngleBox;
	zVEC3 dirAngleBoxDim;

	zVEC3 flyGravity;

	zCTexture *visTexture;
	zTPFX_EmitterVisOrient visOrientation;
	
	zVEC2 visSizeStart;

	zVEC3 visTexColorRGBAStart;
	zVEC3 visTexColorRGBAEnd;

	zTRnd_AlphaBlendFunc visAlphaFunc;

	zCTexture *trlTexture;
	zCTexture *mrkTexture;

	bool32 isOneShotFX;
	float dirAngleHeadVarRad;
	float dirAngleElevVarRad;
	zTPFX_FlockMode m_flockMode;
	float m_ooAlphaDist;
	float m_startTime;
	float m_endTime;
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

class zCParticleEmitterVars
{
public:
	float ppsScaleKeysActFrame;
	float ppsNumParticlesFraction;
	float ppsTotalLifeTime;
	bool32 ppsDependentEmitterCreated;

	float shpScaleKeysActFrame;
	float uniformValue;
	float uniformDelta;
};

class zCParticleFX : public zCVisual
{
public:
	static zCClassDef &classDef;

	zSParticle *firstPart;
	zCParticleEmitterVars emitterVars;
	zCParticleEmitter *emitter;

	zTBBox3D bbox3DWorld;
	zCVob *connectedVob;
	int bboxUpdateCtr;

	byte emitterIsOwned : 1;
	byte dontKillPFXWhenDone : 1;
	byte dead : 1;
	byte isOneShotFX : 1;
	byte forceEveryFrameUpdate : 1;
	byte renderUnderWaterOnly : 1;
	
	zCParticleFX *nextPfx;
	zCParticleFX *prevPFx;
	float privateTotalTime;
	float lastTimeRendered;

	float timeScale;
	float localFrameTimeF;
	zCQuadMark *quadMark;
	zTBBox3D quadMarkBBox3DWorld;
	float m_BboxYRangeInv;
	bool32 m_bVisualNeverDies;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	bool32 CalcIsDead() { XCALL(0x005AF0D0); }
	void StopEmitterOutput() { this->isOneShotFX = TRUE; }
};

class zCModel : public zCVisualAnimate
{
public:
	static zCClassDef &classDef;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	zCModelAni *GetAniFromAniID(int aniID) { XCALL(0x00472F50); }
	bool32 IsAniActive(zSTRING &aniName);
	bool32 IsAniActive(zCModelAni *modelAni) { XCALL(0x00472F90); }
	int GetAniIDFromAniName(zSTRING &aniName) { XCALL(0x00612070); }
	zCModelNodeInst *SearchNode(zSTRING &nodeName) { XCALL(0x0057DFF0); }
};

class oCAniCtrl_Human : public zCObject
{
public:
	void SearchStandAni(bool32 forceStartAni) { XCALL(0x006A4D20); }
	int GetWaterLevel() { XCALL(0x006B89D0); }
	bool32 IsInWater() { return this->GetWaterLevel() > 0; }
};

class oCMag_Book
{
public:
	oCSpell *GetSelectedSpell() { XCALL(0x00477780); }
	oCItem *GetSpellItem(oCSpell *spell) { XCALL(0x00479C00); }
};

#define SPL_STATUS_DONTINVEST 0
#define SPL_STATUS_CANINVEST 1
#define SPL_STATUS_CAST 2
#define SPL_STATUS_STOP 3
#define SPL_STATUS_NEXTLEVEL 4
#define SPL_STATUS_CANINVEST_NO_MANADEC 8
#define SPL_STATUS_FORCEINVEST (1 << 16)

#define TARGET_COLLECT_NONE 0
#define TARGET_COLLECT_CASTER 1
#define TARGET_COLLECT_FOCUS 2
#define TARGET_COLLECT_ALL 3
#define TARGET_COLLECT_FOCUS_FALLBACK_NONE 4
#define TARGET_COLLECT_FOCUS_FALLBACK_CASTER 5
#define TARGET_COLLECT_ALL_FALLBACK_NONE 6
#define TARGET_COLLECT_ALL_FALLBACK_CASTER 7

#define TARGET_FLAG_NONE 0
#define TARGET_FLAG_ALL 1
#define TARGET_FLAG_ITEMS 2
#define TARGET_FLAG_NPCS 4
#define TARGET_FLAG_ORCS 8
#define TARGET_FLAG_HUMANS 16
#define TARGET_FLAG_UNDEAD 32
#define TARGET_FLAG_LIVING 64

#define SPL_PALLIGHT 0
#define SPL_PALLIGHTHEAL 1
#define SPL_PALHOLYBOLT 2
#define SPL_PALMEDIUMHEAL 3
#define SPL_PALREPELEVIL 4
#define SPL_PALFULLHEAL 5
#define SPL_PALDESTROYEVIL 6
#define SPL_PALTELEPORTSECRET 7
#define SPL_TELEPORTSEAPORT 8
#define SPL_TELEPORTMONASTERY 9
#define SPL_TELEPORTFARM 10
#define SPL_TELEPORTXARDAS 11
#define SPL_TELEPORTPASSNW 12
#define SPL_TELEPORTPASSOW 13
#define SPL_TELEPORTOC 14
#define SPL_TELEPORT_1 15
#define SPL_TELEPORT_2 16
#define SPL_TELEPORT_3 17
#define SPL_LIGHT 18
#define SPL_FIREBOLT 19
#define SPL_ICEBOLT 20
#define SPL_LIGHTHEAL 21
#define SPL_SUMMONGOBLINSKELETON 22
#define SPL_INSTANTFIREBALL 23
#define SPL_ZAP 24
#define SPL_SUMMONWOLF 25
#define SPL_WINDFIST 26
#define SPL_SLEEP 27
#define SPL_MEDIUMHEAL 28
#define SPL_LIGHTNINGFLASH 29
#define SPL_CHARGEFIREBALL 30
#define SPL_SUMMONSKELETON 31
#define SPL_FEAR 32
#define SPL_ICECUBE 33
#define SPL_THUNDERBALL 34
#define SPL_SUMMONGOLEM 35
#define SPL_DESTROYUNDEAD 36
#define SPL_CHARGEFIRESTORM 37 // SPL_PYROKINESIS
#define SPL_INSTANTFIRESTORM 38 // SPL_FIRESTORM
#define SPL_ICEWAVE 39
#define SPL_SUMMONDEMON 40
#define SPL_FULLHEAL 41
#define SPL_FIRERAIN 42
#define SPL_BREATHOFDEATH 43
#define SPL_MASSDEATH 44
#define SPL_ARMYOFDARKNESS 45
#define SPL_SHRINK 46
#define SPL_TRFSHEEP 47
#define SPL_TRFSCAVENGER 48
#define SPL_TRFGIANTRAT 49
#define SPL_TRFGIANTBUG 50
#define SPL_TRFWOLF 51
#define SPL_TRFWARAN 52
#define SPL_TRFSNAPPER 53
#define SPL_TRFWARG 54
#define SPL_TRFFIREWARAN 55
#define SPL_TRFLURKER 56
#define SPL_TRFSHADOWBEAST 57
#define SPL_TRFDRAGONSNAPPER 58
#define SPL_CHARM 59
#define SPL_MASTEROFDESASTER 60
#define SPL_DEATHBOLT 61
#define SPL_DEATHBALL 62
#define SPL_CONCUSSIONBOLT 63
#define SPL_RESERVED_64 64
#define SPL_RESERVED_65 65
#define SPL_RESERVED_66 66
#define SPL_RESERVED_67 67
#define SPL_RESERVED_68 68
#define SPL_RESERVED_69 69
#define SPL_THUNDERSTORM 70
#define SPL_WHIRLWIND 71
#define SPL_WATERFIST 72
#define SPL_ICELANCE 73
#define SPL_INFLATE 74
#define SPL_GEYSER 75
#define SPL_WATERWALL 76
#define SPL_RESERVED_77 77
#define SPL_RESERVED_78 78
#define SPL_RESERVED_79 79
#define SPL_PLAGUE 80
#define SPL_SUMMONSWARM 81
#define SPL_GREENTENTACLE 82
#define SPL_EARTHQUAKE 83
#define SPL_SUMMONGUARDIAN 84
#define SPL_BELIARSRAGE 85
#define SPL_SUCKENERGY 86
#define SPL_SKULL 87
#define SPL_SUMMONZOMBIE 88
#define SPL_RESERVED_89 89
#define SPL_RESERVED_90 90
#define SPL_RESERVED_91 91
#define SPL_RESERVED_92 92
#define SPL_RESERVED_93 93
#define SPL_RESERVED_94 94
#define SPL_RESERVED_95 95
#define SPL_RESERVED_96 96
#define SPL_RESERVED_97 97
#define SPL_RESERVED_98 98
#define SPL_RESERVED_99 99
#define SPL_MAX_SPELL 100

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
	bool32 spellEnabled;
	bool32 spellInitDone;

	bool32 timerEffect;
	bool32 canBeDeleted;

	float up;
	float hoverY;
	float hoverOld;
	float hoverDir;

	int spellEnergy;

	float manaInvestTime;
	int damagePerLevel;
	int damageType;
	int spellType;
	bool32 canTurnDuringInvest;
	bool32 canChangeTargetDuringInvest;
	int isMultiEffect;
	int targetCollectAlgo;
	int targetCollectType;
	int targetCollectRange;
	int targetCollectAzi;
	int targetCollectElev;

public:
	oCVisualFX *CreateEffect() { XCALL(0x004842E0); }
	void StopTargetEffects(zCVob *vob) { XCALL(0x00485C40); }
	bool32 IsValidTarget(zCVob *v) { XCALL(0x004861D0); }
	bool32 IsInvestSpell() { XCALL(0x00486630); }
	void DoLogicInvestEffect() { XCALL(0x00486950); }
	bool32 CastSpecificSpell() { XCALL(0x00486960); }
	void EndTimedEffect() { XCALL(0x00486E10); }
	void DoTimedEffect() { XCALL(0x00487280); }
	void SetReleaseStatus() { XCALL(0x00486670); }
	void CallScriptInvestedMana() { XCALL(0x00485D30); }
	zSTRING GetSpellInstanceName(int _spellID);
	void InitByScript(const zSTRING &name) { XCALL(0x00484550); }
};

class oCMOB : public oCVob
{
public:
	static zCClassDef &classDef;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	bool32 IsMoveable() { XCALL(0x0071BF20); }
};

class oCMobInter : public oCMOB
{
public:
	static zCClassDef &classDef;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	bool32 IsOccupied() { XCALL(0x00718CC0); }
};

class zCEffect : public zCVob { };

struct zSVisualFXColl
{
	zCVob *foundVob;
	zVEC3 foundContactPoint;
	zVEC3 foundNormal;
};

class oCTrajectory
{
public:
	zCArray<zCPositionKey *> keyList;

	zCKBSpline *spl;
	int mode;
	float length;
	zMAT4 res;
	int lastKey;

public:
	void SetByList(zCArray<zCVob *> vobList) { XCALL(0x00488AB0); }
	int VobCross(zCVob *vob) { XCALL(0x00488C20); }
};

class oCEmitterKey
{
public:
	zSTRING visName_S;
	float visSizeScale;
	float scaleDuration;

	float pfx_ppsValue;
	bool32 pfx_ppsIsSmoothChg;
	bool32 pfx_ppsIsLoopingChg;

	float pfx_scTime;
	zSTRING pfx_flyGravity_S;

	zSTRING pfx_shpDim_S;
	bool32 pfx_shpIsVolumeChg;
	float pfx_shpScaleFPS;
	float pfx_shpDistribWalkSpeed;
	zSTRING pfx_shpOffsetVec_S;
	zSTRING pfx_shpDistribType_S;
	zSTRING pfx_dirMode_S;
	zSTRING pfx_dirFOR_S;
	zSTRING pfx_dirModeTargetFOR_S;
	zSTRING pfx_dirModeTargetPos_S;
	float pfx_velAvg;
	float pfx_lspPartAvg;
	float pfx_visAlphaStart;

	zSTRING lightPresetName;
	float lightRange;
	zSTRING sfxID;
	bool32 sfxIsAmbient;

	zSTRING emCreateFXID;
	float emFlyGravity;
	zSTRING emSelfRotVel_S;
	zSTRING emTrjMode_S;
	float emTrjEaseVel;
	bool32 emCheckCollision;
	float emFXLifeSpan;

	byte dScriptEnd;

	zVEC3 pfx_flyGravity;
	zVEC3 pfx_shpOffsetVec;
	zTPFX_DistribType pfx_shpDistribType;
	zTPFX_EmitterDirMode pfx_dirMode;
	zTPFX_EmitterFOR pfx_dirFOR;
	zTPFX_EmitterFOR pfx_dirModeTargetFOR;
	zVEC3 pfx_dirModeTargetPos;

	zVEC3 emSelfRotVel;
	int emTrjMode;

	zSTRING name;

	zCVob *vob;
	zVEC3 targetPos;

	zCSoundFX *sfx;
	zTSoundHandle sxfHnd;
};

#define VFX_NUM_USERSTRINGS 5

#define TACTION_COLL_NONE 0

enum zTVFXState
{
	zVFXSTATE_UNDEF,
	zVFXSTATE_OPEN,
	zVFXSTATE_INIT,
	zVFXSTATE_INVESTNEXT,
	zVFXSTATE_CAST,
	zVFXSTATE_STOP,
	zVFXSTATE_COLLIDE
};

class oCVisualFX : public zCEffect
{
public:
	static zCClassDef &classDef;

	zSTRING visName_S;
	zSTRING visSize_S;
	float visAlpha;
	zSTRING visAlphaBlendFunc_S;
	float visTexAniFPS;
	bool32 visTexAniIsLooping;
	zSTRING emTrjMode_S;
	zSTRING emTrjOriginNode_S;
	zSTRING emTrjTargetNode_S;
	float emTrjTargetRange;
	float emTrjTargetAzi;
	float emTrjTargetElev;
	int emTrjNumKeys;
	int emTrjNumKeysVar;
	float emTrjAngleElevVar;
	float emTrjAngleHeadVar;
	float emTrjKeyDistVar;
	zSTRING emTrjLoopMode_S;
	zSTRING emTrjEaseFunc_S;
	float emTrjEaseVel;
	float emTrjDynUpdateDelay;
	int emTrjDynUpdateTargetOnly;
	zSTRING emFXCreate_S;
	zSTRING emFXInvestOrigin_S;
	zSTRING emFXInvestTarget_S;
	float emFXTriggerDelay;
	int emFXCreatedOwnTrj;
	zSTRING emActionCollDyn_S;
	zSTRING emActionCollStat_S;
	zSTRING emFXCollStat_S;
	zSTRING emFXCollDyn_S;
	zSTRING emFXCollDynPerc_S;
	zSTRING emFXCollStatAlign_S;
	zSTRING emFXCollDynAlign_S;
	float emFXLifeSpan;

	bool32 emCheckCollision;
	bool32 emAdjustShpToOrigin;
	float emInvestNextKeyDuration;
	float emFlyGravity;
	zSTRING emSelfRotVel_S;
	zSTRING userString[VFX_NUM_USERSTRINGS];

	zSTRING lightPresetName;
	zSTRING sfxID;
	bool32 sfxIsAmbient;
	bool32 sendAssessMagic;
	float secsPerDamage;

	byte dScriptEnd;

	zVEC3 visSize;
	int emTrjMode;
	int emActionCollDyn;
	int emActionCollStat;
	zVEC3 emSelfRotVel;
	int emTrjEaseFunc;
	int emTrjLoopMode;
	zTVFXState fxState;

	oCVisualFX *root;
	oCVisualFX *parent;
	oCVisualFX *fxInvestOrigin;
	oCVisualFX *fxInvestTarget;
	oCVisualFXAI *ai;

	zCArray <oCVisualFX *> fxList;
	zCArray <oCVisualFX *> childList;
	zCArray <oCEmitterKey *> emKeyList;
	zCArray <zCVob *> vobList;
	zCArray <zCVob *> ignoreVobList;
	zCArray <zCVob *> allowedCollisionVobList;
	zCArray <zCVob *> collidedVobs;
	zCArray <zSVisualFXColl> queuedCollisions;
	oCTrajectory trajectory;
	zCEarthquake *earthQuake;
	zCVobScreenFX *screenFX;
	float screenFXTime;
	int screenFXDir;

	zCModelNodeInst *orgNode;
	zCModelNodeInst *targetNode;

	zCVisual *lastSetVisual;
	zCVob *origin;
	zCVob *inflictor;
	zCVob *target;
	zCVobLight *light;
	float lightRange;
	zCSoundFX *sfx;
	zTSoundHandle sfxHnd;

	zSTRING fxName;

	oCEmitterKey *fxBackup;
	oCEmitterKey *lastSetKey;
	oCEmitterKey *actKey;

	float frameTime;
	float collisionTime;
	float deleteTime;
	float damageTime;

	zVEC3 targetPos;
	zVEC3 lastTrjDir;
	zVEC3 keySize;
	zVEC3 actSize;
	zVEC3 castEndSize;

	float nextLevelTime;
	float easeTime;
	float age;
	float trjUpdateTime;
	float emTrjDist;
	float trjSign;
	float levelTime;
	float lifeSpanTimer;
	float fxStartTime;
	float oldFovX;
	float oldFovY;

	bool32 collisionOccured : 1;

	bool32 showVisual : 1;
	bool32 isChild : 1;
	bool32 isDeleted : 1;
	bool32 initialized : 1;
	bool32 shouldDelete : 1;
	bool32 lightning : 1;
	bool32 fxInvestOriginInitialized : 1;
	bool32 fxInvestTargetInitialized : 1;
	bool32 fxInvestStopped : 1;
	bool32 timeScaled : 1;
	int fovMorph : 2;
	int level : 5;
	int collisionCtr : 3;
	int queueSetLevel : 5;

	float damage;
	int damageType;

	int spellType;
	int spellCat;
	int spellTargetTypes;
	float savePpsValue;
	zVEC2 saveVisSizeStart;
	zVEC3 transRing[10];
	int ringPos;
	bool32 emTrjFollowHitLastCheck;
	bool32 bIsProjectile;
	bool32 bPfxMeshSetByVisualFX;
	bool32 m_bAllowMovement;
	float m_fSleepTimer;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }
	static oCVisualFX *_CreateNewInstance() { XCALL(0x0049A230); }
	static oCVisualFX *CreateAndPlay(zSTRING &id, zCVob *org, zCVob *target, int level, float damage, int damageType, bool32 bIsProjectile) { XCALL(0x0048E760); }
	static oCVisualFX *CreateAndPlay(zSTRING &id, zVEC3 &orgPos, zCVob *target, int level, float damage, int damageType, bool32 bIsProjectile) { XCALL(0x0048EA80); }

	oCVisualFX() { XCALL(0x00489AA0); }

	virtual zCClassDef *_GetClassDef() { XCALL(0x0048A010); }
	virtual void Archive(zCArchiver &arc) { XCALL(0x00499B40); }
	virtual void Unarchive(zCArchiver &arc) { XCALL(0x00499B50); }
	virtual ~oCVisualFX() { XCALL(0x0048A1F0); }

	virtual void OnTick() { XCALL(0x00499A20); }
	virtual bool32 CanThisCollideWith(zCVob *vob) { XCALL(0x00496AC0); }

	virtual void Open() { XCALL(0x004918E0); }
	virtual void SetOrigin(zCVob *orgVob, bool32 recalcTrj) { XCALL(0x004910F0); }
	virtual void SetTarget(zCVob *targetVob, bool32 recalcTrj) { XCALL(0x004912E0); }
	virtual void SetTarget(zVEC3 &targetPos, bool32 recalcTrj) { XCALL(0x00491450); }
	virtual void SetInflictor(zCVob *inflictorVob) { XCALL(0x00491220); }
	virtual zCVob *GetOrigin() { return this->origin; }
	virtual zCVob *GetTarget() { return this->target; }
	virtual zCVob *GetInflictor() { return this->inflictor; }
	virtual void Init(zCVob *orgVob, zVEC3 &targetPos) { XCALL(0x00491E10); }
	virtual void Init(zCVob *orgVob, zCVob *trgtVob, zCVob *inflictorVob) { XCALL(0x00491F20); }
	virtual void Init(zCArray<zCVob *> &trajectoryVobs) { XCALL(0x004926A0); }
	virtual void InvestNext() { XCALL(0x00492830); }
	virtual void SetLevel(int level, bool32 force) { XCALL(0x00492CB0); }
	virtual int GetLevel() { return this->level; }
	virtual void Cast(bool32 killAfterDone) { XCALL(0x00493160); }
	virtual void Stop(bool32 killAfterDone) { XCALL(0x00493BE0); }
	virtual void Kill() { XCALL(0x00493F70); }
	virtual void Play(float keyCycleTime, zMAT4 *orgTrafo, zMAT4 *targetTrafo) { }
	virtual bool32 CanBeDeleted() { XCALL(0x004942B0); }
	virtual bool32 IsFinished() { XCALL(0x004942F0); }
	virtual bool32 IsLooping() { XCALL(0x00494370); }
	virtual void SetByScript(const zSTRING &id) { XCALL(0x0048D4B0); }
	virtual void SetDuration(float fSecDuration) { this->emFXLifeSpan = fSecDuration; }
	virtual void Reset() { XCALL(0x00491C20); }
	virtual void ResetForEditing() { XCALL(0x0049E950); }
	virtual void ReportCollision(zCCollisionReport &collisionReport) { XCALL(0x00494E80); }
	virtual void SetCollisionEnabled(bool32 en) { XCALL(0x0048D330); }
	virtual void SetCollisionCandidates(zCArray<zCVob *> collisionVobs) { XCALL(0x004968E0); }
	virtual void GetCollisionCandidates(zCArray<zCVob *> &collisionVobs) { XCALL(0x0048A070); }
	virtual int GetNumCollisionCandidates() { return this->allowedCollisionVobList.numInArray; }
	virtual bool32 GetCollidedCandidates(zCArray<zCVob *> &collidedVobs) { XCALL(0x00496A00); }
	virtual void SetDamage(float dam) { this->damage = dam; }
	virtual void SetDamageType(int damType) { this->damageType = damType; }
	virtual float GetDamage() { return this->damage; }
	virtual int GetDamageType() { return this->damageType; }
	virtual bool32 IsASpell() { return this->sendAssessMagic; }
	virtual void SetSpellType(int _type) { this->spellType = _type; }
	virtual int GetSpellType() { return this->spellType; }
	virtual void SetSpellCat(int cat) { this->spellCat = cat; }
	virtual int GetSpellCat() { return this->spellCat; }
	virtual int GetSpellTargetTypes() { return this->spellTargetTypes; }
	virtual void SetSpellTargetTypes(int types) { this->spellTargetTypes = types; }
	virtual bool32 GetSendsAssessMagic() { XCALL(0x0048B350); }
	virtual void SetSendsAssessMagic(bool32 a_bSendAssessMagic) { XCALL(0x0048B2C0); }
	virtual bool32 GetIsProjectile() { return this->bIsProjectile; }
	virtual void SetIsProjectile(bool32 b) { this->bIsProjectile = b; }
	virtual void SetVisualByString(const zSTRING &visName) { XCALL(0x0048C220); }
	virtual void CalcTrajectory(bool32 &updateTargetOnly) { XCALL(0x0048F620); }
	virtual void Collide(bool32 killAfterDone) { XCALL(0x00493A00); }
	virtual void CollisionResponse(zVEC3 &collisionNormal, bool32 alignCollNormal) { XCALL(0x00496380); }

	void InitValues() { XCALL(0x0048B820); }
	oCVisualFX *CreateAndCastFX(const zSTRING &id, zCVob *org, zCVob *inflictor) { XCALL(0x0048EE80); }
	void StopInvestFX() { XCALL(0x00491830); }
	void InitInvestFX() { XCALL(0x00491690); }
};

class oCVisFX_MultiTarget : public oCVisualFX
{
public:
	zCArray<oCVisualFX *> visList;

public:
	static oCVisFX_MultiTarget *_CreateNewInstance() { XCALL(0x0049F750); }
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

#define GAME_LEFT 1
#define GAME_RIGHT 2
#define GAME_UP 3

class zCInput_Win32
{
public:
	virtual ~zCInput_Win32() { XCALL(0x004D4CD0); }
	virtual float GetState(word logicalID) { XCALL(0x00830B68); }
};

class zCActiveSnd
{
public:
	void AutoCalcObstruction(bool32 immediate) { XCALL(0x004F9830); }
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
	// TODO ...
};

class zCPar_Symbol
{
public:
	void GetValue(int &val, int nr) { XCALL(0x007A1FE0); }
	void GetValue(zSTRING &s, int nr) { XCALL(0x007A2040); }
	void *GetInstanceAdr() { XCALL(0x007A1DC0); }
};

class zCParser
{
public:
	zCPar_Symbol *GetSymbol(const zSTRING &s) { XCALL(0x007938D0); }
	void GetParameter(int &value) { XCALL(0x007A0760); }
	void GetParameter(zSTRING &s) { XCALL(0x007A07B0); }
};

class zERROR
{
public:
	void (*onexit)();

	zSTRING filter_authors;
	unsigned int filter_flag;
	char filter_level;
	int target;

	int ack_type;
	zFILE *log_file;
	byte indent_depth;
	HWND spyHandle;
	zCMutex *spyMutex;

public:
	virtual ~zERROR() { XCALL(0x0044C650); }

	int Report(int xLevel, int xId, const zSTRING &xMessage, char level, unsigned int flag, int line, char *file, char *function) { XCALL(0x0044C8D0); }
};

class zCViewBase
{
public:
	virtual int anx(int x) { return 0; }
	virtual int any(int y) { return 0; }
	virtual int nax(int x) { return 0; }
	virtual int nay(int y) { return 0; }

	virtual int ClipLine(int &x0, int &y0, int &x1, int &y1) { return 0; }
};

class zCView : public zCViewBase
{

};

enum zTArchiveMode { };

class zCArchiver : public zCObject
{
public:
	virtual void __fastcall WriteInt(const char *entryName, const int value) = 0;
	virtual void __fastcall WriteByte(const char *entryName, const byte value) = 0;
	virtual void __fastcall WriteWord(const char *entryName, const word value) = 0;
	virtual void __fastcall WriteFloat(const char *entryName, const float value) = 0;
	virtual void __fastcall WriteBool(const char *entryName, const bool32 value) = 0;
	virtual void __fastcall WriteString(const char *entryName, const zSTRING &value) = 0;
	virtual void __fastcall WriteVec3(const char *entryName, const zVEC3 &value) = 0;
	virtual void __fastcall WriteColor(const char *entryName, const zCOLOR &value) = 0;
	virtual void __fastcall WriteRaw(const char *entryName, void *buffer, const dword size) = 0;
	virtual void __fastcall WriteRawFloat(const char *entryName, void *buffer, const dword size) = 0;
	virtual void __fastcall WriteEnum(const char *entryName, const char *enumChoices, const int value) = 0;
	virtual void __fastcall WriteObject(zCObject *object) = 0;
	virtual void __fastcall WriteObject(const char *chunkName, zCObject *object) = 0;
	virtual void __fastcall WriteChunkStart(const char *chunkName, word chunkVersion = 0) = 0;
	virtual void __fastcall WriteChunkEnd() = 0;
	virtual void __fastcall WriteGroupBegin(const char *groupName) = 0;
	virtual void __fastcall WriteGroupEnd(const char *groupName) = 0;

	virtual void __fastcall ReadInt(const char *entryName, int &value) = 0;
	virtual void __fastcall ReadByte(const char *entryName, byte &value) = 0;
	virtual void __fastcall ReadWord(const char *entryName, word &value) = 0;
	virtual void __fastcall ReadFloat(const char *entryName, float &value) = 0;
	virtual void __fastcall ReadBool(const char *entryName, bool32 &value) = 0;
	virtual void __fastcall ReadString(const char *entryName, zSTRING &value) = 0;
	virtual void __fastcall ReadVec3(const char *entryName, zVEC3 &value) = 0;
	virtual void __fastcall ReadColor(const char *entryName, zCOLOR &value) = 0;
	virtual void __fastcall ReadEnum(const char *entryName, int &value) = 0;
	virtual void __fastcall ReadRaw(const char *entryName, void *buffer, const word size) = 0;
	virtual void __fastcall ReadRawFloat(const char *entryName, void *buffer, const word size) = 0;

	virtual zCObject *__fastcall ReadObject(zCObject *objectUseThis) = 0;
	virtual zCObject *__fastcall ReadObject(const char *chunkName, zCObject *objectUseThis) = 0;

	virtual zCObject *__fastcall ReadObjectAccount(const char *file, int line, zCObject *objectUseThis) { XCALL(0x0051AFF0); }
	virtual zCObject *__fastcall ReadObjectAccount(const char *file, int line, const char *chunkName, zCObject *objectUseThis) { XCALL(0x0051B020); }

	virtual	bool32 __fastcall ReadChunkStart(zSTRING &chunkName, word &chunkVersion) = 0;
	virtual bool32 __fastcall ReadChunkStartNamed(const char *chunkName, word &chunkVersion) = 0;
	virtual void __fastcall SkipOpenChunk() = 0;
	virtual word __fastcall GetCurrentChunkVersion() = 0;

	virtual int	__fastcall ReadInt(const char *entryName) = 0;
	virtual byte __fastcall ReadByte(const char *entryName) = 0;
	virtual word __fastcall ReadWord(const char *entryName) = 0;
	virtual float __fastcall ReadFloat(const char *entryName) = 0;
	virtual bool32 __fastcall ReadBool(const char *entryName) = 0;
	virtual zSTRING	__fastcall ReadString(const char *entryName) = 0;
	virtual zVEC3 __fastcall ReadVec3(const char *entryName) = 0;
	virtual zCOLOR __fastcall ReadColor(const char *entryName) = 0;
	virtual int	__fastcall ReadEnum(const char *entryName) = 0;

	virtual zFILE *GetFile() const = 0;
	virtual void __fastcall GetBufferString(zSTRING &result) = 0;
	virtual zCBuffer *__fastcall GetBuffer() = 0;
	virtual bool32 __fastcall EndOfArchive() = 0;

	virtual void Close() = 0;

	virtual void SetStringEOL(const zSTRING &eol) = 0;
	virtual zSTRING	GetStringEOL() const = 0;
	virtual bool32 IsStringValid(const char *string) = 0;
	virtual bool32 GetChecksumEnabled() const = 0;
	virtual void SetChecksumEnabled(const bool32 b) = 0;
	virtual void SetNoReadSearchCycles(const bool32 b) = 0;
	virtual bool32 InProperties() const = 0;
	virtual bool32 InSaveGame() const = 0;
	virtual bool32 InBinaryMode() const = 0;

	virtual zCObject *__fastcall GetParentObject() = 0;

	virtual bool32 OpenWriteBuffer(zCBuffer *buffer, zTArchiveMode arcMode, bool32 saveGame, int arcFlags, bool32 arcOwnsMedium) = 0;
	virtual void OpenWriteFile(zFILE *fileWrite, zTArchiveMode arcMode, bool32 saveGame, int arcFlags, bool32 arcOwnsMedium) = 0;

	virtual void __fastcall WriteChunkStart(const char *chunkName, const char *className, word classVersion, dword objectIndex) = 0;
	virtual void __fastcall WriteObject(const char *chunkName, zCObject &object) = 0;
	virtual void __fastcall WriteHeader(const int arcFlags = 0) = 0;
	virtual void __fastcall WriteHeaderNumObj() = 0;
	virtual void __fastcall WriteASCIILine(const char *entryName, const char *typeName, const zSTRING &value) = 0;
	virtual void __fastcall StoreBuffer(void *buffer, const dword size) = 0;
	virtual void __fastcall StoreString(const char *string) = 0;
	virtual void __fastcall StoreStringEOL(const char *string) = 0;
	virtual dword __fastcall StoreGetPos() = 0;
	virtual void __fastcall StoreSeek(const dword newPos) = 0;

	virtual	bool32 OpenReadBuffer(zCBuffer &buffer, zTArchiveMode arcMode, bool32 saveGame, int arcFlags) = 0;
	virtual void OpenReadFile(zFILE *fileRead, zTArchiveMode arcMode, bool32 saveGame, int arcFlags, bool32 deleteFileOnClose) = 0;

	virtual zCClassDef *__fastcall GetClassDefByString(const zSTRING &className) = 0;
	virtual zCObject *__fastcall CreateObject(const zSTRING &className) = 0;
	virtual void __fastcall SkipChunk(const bool32 removeFromChunkStack) = 0;
	virtual bool32 __fastcall ReadChunkStart(const char *chunkName) = 0;
	virtual void __fastcall ReadChunkStartASCII(const char *chunkName, zSTRING &resultLine) = 0;
	virtual void __fastcall ReadChunkEnd() = 0;
	virtual bool32 __fastcall ReadEntryASCII(const char *entryName, zSTRING &resultValue) = 0;
	virtual void __fastcall ReadHeader() = 0;
	virtual void __fastcall RestoreBuffer(void *buffer, const dword size) = 0;
	virtual void __fastcall RestoreStringEOL(zSTRING &string) = 0;
	virtual void __fastcall RestoreString0(zSTRING &string) = 0;
	virtual bool32 __fastcall RestoreGetPos() = 0;
	virtual void __fastcall RestoreSeek(const dword newPos) = 0;

	virtual void __fastcall	DeleteBuffer() = 0;
};

class zCTriggerBase : public zCVob
{
public:
	zSTRING triggerTarget;
};

class zCTrigger : public zCTriggerBase
{
public:
	struct
	{
		byte reactToOnTrigger : 1;
		byte reactToOnTouch : 1;
		byte reactToOnDamage : 1;

		byte respondToObject : 1;
		byte respondToPC : 1;
		byte respondToNPC : 1;
	} filterFlags;

	struct
	{
		byte startEnabled : 1;
		byte isEnabled : 1;
		byte sendUntrigger : 1;
	} flags;
};
