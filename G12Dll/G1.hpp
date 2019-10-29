WRAPPER void *_malloc(size_t Size) { EAXJMP(0x0075AD60); }
#define malloc(Size) _malloc(Size)

WRAPPER void *_calloc(size_t NumOfElements, size_t SizeOfElements) { EAXJMP(0x0075AD65); }
#define calloc(NumOfElements, SizeOfElements) _calloc(NumOfElements, SizeOfElements)

WRAPPER void *_realloc(void *Memory, size_t NewSize) { EAXJMP(0x0075AD6A); }
#define realloc(Memory, NewSize) _realloc(Memory, NewSize)

WRAPPER void _free(void *Memory) { EAXJMP(0x0075AD6F); }
#define free(Memory) _free(Memory)

void *operator new(size_t Size) { return malloc(Size); }
void operator delete(void *Memory) { free(Memory); }
void *operator new[](size_t Size) { return malloc(Size); }
void operator delete[](void *Memory) { free(Memory); }

WRAPPER void *_memmove(void *Dst, const void *Src, size_t Size) { EAXJMP(0x00776B70); }
#define memmove(Dst, Src, Size) _memmove(Dst, Src, Size)

WRAPPER int _rand(void) { EAXJMP(0x007795F8); }
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
	float LengthApprox() { XCALL(0x00488FD0); }
	zVEC3 &Normalize() { XCALL(0x00489060); }
	zVEC3 &NormalizeApprox() { XCALL(0x0049EF70); }
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
	zMAT4 InverseLinTrafo() { XCALL(0x00505730); }
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

	bool IsInList(const T &data)
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
	virtual ~zSTRING() { XCALL(0x00401260); };

	zSTRING() { XCALL(0x00402B30); }
	zSTRING(const zSTRING &xStr) { XCALL(0x00415F40); }
	zSTRING(const zSTRING *pstr) { XCALL(0x006E9570); }
	zSTRING(char *pstring) { XCALL(0x004013A0); }
	zSTRING(char ch) { XCALL(0x0050AC70); }
	zSTRING(int xWert) { XCALL(0x00465810); }
	zSTRING(unsigned int xWert) { XCALL(0x00465A10); }
	zSTRING(const float xWert, int digits = 20) { XCALL(0x00465B10); }
	zSTRING(const double xWert, int digits = 20) { XCALL(0x00465C60); }

	byte IsEmpty() const { return this->_Len == 0; }
	int Length() const { return this->_Len; }
	char *ToChar() const { return this->_Ptr; }
	void Clear() { XCALL(0x0057E220); }

	int Search(int startIndex, const char *substr, unsigned int num = 1) { XCALL(0x00467750); }
	int Search(int startIndex, const zSTRING &substr, unsigned int num = 1) { return this->Search(startIndex, substr.ToChar(), num); }
	int Search(const zSTRING &substr, unsigned int num = 1) { return this->Search(0, substr.ToChar(), num); }
	int Search(const char *substr, unsigned int num = 1) { return this->Search(0, substr, num); }

	bool Contains(char *substr) { return this->Search(0, substr, 1) != -1; }
	bool Contains(const zSTRING &substr) { return Search(0, substr.ToChar(), 1) != -1; }

	friend zSTRING operator+(const zSTRING &xStr1, const zSTRING &xStr2) { XCALL(0x00404480); }
	friend zSTRING operator+(const zSTRING &xStr1, const char *pstring) { XCALL(0x00404740); }
	friend zSTRING operator+(const zSTRING &xStr1, const char ch) { XCALL(0x00441E70); }
	friend zSTRING operator+(const char *pstring, const zSTRING &xStr2) { XCALL(0x004045D0); }
	friend zSTRING operator+(const char ch, const zSTRING &xStr2) { XCALL(0x00445E60); }

	friend bool operator==(const zSTRING &xStr1, const zSTRING &xStr2) { XCALL(0x0073F450); }
	friend bool operator==(const zSTRING &xStr1, const char *xStr2) { XCALL(0x00644FA0); }
	friend bool operator==(const char *xStr1, const zSTRING &xStr2) { return xStr2 == xStr1; }

	friend bool operator!=(const zSTRING &xStr1, const zSTRING &xStr2) { return !(xStr1 == xStr1); }
	friend bool operator!=(const zSTRING &xStr1, const char *xStr2) { return !(xStr1 == xStr2); }
	friend bool operator!=(const char *xStr1, const zSTRING &xStr2) { return !(xStr1 == xStr2); }
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
	zCOLOR hintLightingHighlightColor;

	byte hintLightingFullbright : 1;
	byte hintLightingSwell : 1;
	byte hintLightingHighlight : 1;
};

struct zTTraceRayReport
{
	bool foundHit;
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

	bool active;
};

struct myThunder
{
	zVEC3 originVec;
	myThunder *childs;
	int numChilds;
	float startTime[5];
	zCPolyStrip *polyStrip;
	int numSegs;
	bool valid;
	float t0;
	float t1;
	int numSplits;
	bool dead;
	bool isChild;
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
	static void ObjectCreated(zCObject *object, zCClassDef *objClassDef) { XCALL(0x0058B380); }
	static void ObjectDeleted(zCObject *object, zCClassDef *objClassDef) { XCALL(0x0058B4A0); }
};

class zCObject
{
public:
	int refCtr;
	word hashIndex;
	zCObject *hashNext;
	zSTRING objectName;

public:
	static bool CheckInheritance(zCClassDef *baseClass, zCClassDef *subClass) { XCALL(0x00470130); }

	zCObject() { XCALL(0x00401C20); }

	virtual zCClassDef *_GetClassDef() { XCALL(0x00401D80); }
	virtual void Archive(zCArchiver &arc) { }
	virtual void Unarchive(zCArchiver &arc) { }
	virtual ~zCObject() { XCALL(0x005891E0); }

	zCObject *AddRef() { this->refCtr++; return this; }
	int Release() { XCALL(0x0042AC30); }
};

#define zRELEASE(obj) { if (obj) { obj->Release(); obj = NULL; } }

class zCAIBase : public zCObject { };

class oCVisualFXAI : public zCAIBase
{
public:
	zCVob *vob;
	bool delete_it;
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
	virtual bool Render(zTRenderContext &renderContext) = 0;
	virtual bool IsBBox3DLocal() = 0;
	virtual zTBBox3D GetBBox3D() = 0;
	virtual zCOBBox3D *GetOBBox3D() = 0;
	virtual zSTRING GetVisualName() = 0;
	virtual bool GetVisualDied() = 0;
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
		byte lodFlag : 1;
		byte portalIndoorOutdoor : 1;
		byte ghostOccluder : 1;
		byte normalMainAxis : 2;
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
	bool hasLightmaps;

	int numVertAlloc;
	int numPolyAlloc;

public:
	static zCMesh *Load(zSTRING &meshFileName) { XCALL(0x005501A0); }

	virtual bool Render(zTRenderContext &renderContext, zCOLOR *vertexColor) { XCALL(0x005537E0); }
};

class zCTexAniCtrl
{
public:
	int aniChannel;
	float actFrame;
	float aniFPS;
	int frameCtr;
	bool bOneShotAni;
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

	zCTexture *detailTexture;
	float detailTextureScale;
	zVEC2 texAniMapDelta;

	zVEC2 default_mapping;
	zVEC2 texScale;

public:
	void SetTexture(zSTRING &texName) { XCALL(0x0054DAC0); }
};

class zCEventManager : public zCObject
{
public:
	virtual void OnTrigger(zCVob *otherVob, zCVob *vobInstigator) { XCALL(0x006DE760); }
	virtual void OnUntrigger(zCVob *otherVob, zCVob *vobInstigator) { XCALL(0x006DE820); }
	virtual void OnTouch(zCVob *otherVob) { XCALL(0x006DE5D0); }
	virtual void OnUntouch(zCVob *otherVob) { XCALL(0x006DE690); }
	virtual void OnTouchLevel() { XCALL(0x006DE750); }
	virtual void OnDamage(zCVob *otherVob, zCVob *inflictorVob, float damage, int damageType, zVEC3 &hitLocation) { XCALL(0x006DE8E0); }
	virtual void OnMessage(zCEventMessage *eventMessage, zCVob *sourceVob) { XCALL(0x006DD090); }
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
	void SetVelocity(zVEC3 &vel) { XCALL(0x00595380); }
};

class zCCollisionObjectDef
{
public:
	zCCollisionObject *(*m_createNewInstance)(void);
	bool m_bIsVolatile;
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

	zCArray<zCVob *> touchVobList;

	int type;
	unsigned int groundShadowSizePacked;
	zCWorld *homeWorld;
	zCPolygon *groundPoly;
	zCAIBase *callback_ai;
	zMAT4 *trafo;

	zCVisual *visual;
	float visualAlpha;

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

	zTVobSleepingMode sleepingMode : 2;
	byte mbHintTrafoLocalConst : 1;
	byte mbInsideEndMovementMethod : 1;

	zTVisualCamAlign visualCamAlign : 2;

	byte collButNoMove : 8;
	byte dontWriteIntoArchive : 1;

	zCCollisionObjectDef *m_poCollisionObjectClass;
	zCCollisionObject *m_poCollisionObject;

public:
	static zCVob *_CreateNewInstance() { XCALL(0x005D2760); }

	zCVob() { XCALL(0x005D3030); }
	~zCVob() { XCALL(0x005D32A0); }

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
	virtual void SetSleepingMode(zTVobSleepingMode smode) { XCALL(0x005D7280); }
	virtual void EndMovement() { XCALL(0x005F0B60); }
	virtual bool CanThisCollideWith(zCVob *vob) { return TRUE; }
	virtual bool __fastcall Render(zTRenderContext &renderContext) { XCALL(0x005D6090); }
	virtual void SetVisual(zCVisual *v) { XCALL(0x005D6E10); }
	virtual void SetVisual(zSTRING &visualFileName) { XCALL(0x005D6FA0); }
	virtual bool GetScriptInstance(zSTRING *&scriptInstanceName, int &scriptInstanceIndex) { scriptInstanceName = NULL; scriptInstanceIndex = 0; return FALSE; }
	virtual bool SetByScriptInstance(zSTRING *scriptInstanceName, const int scriptInstanceIndex) { return FALSE; }
	virtual int GetCSStateFlags() { return 0; }
	virtual bool TraceRay(zVEC3 &rayOrigin, zVEC3 &ray, int traceFlags, zTTraceRayReport &report) { XCALL(0x005D49E0); }
	virtual zSTRING *GetTriggerTarget(int i) { return NULL; }
	virtual void ThisVobAddedToWorld(zCWorld *homeWorld) { XCALL(0x005D66B0); }
	virtual void ThisVobRemovedFromWorld(zCWorld *homeWorld) { XCALL(0x005D66D0); }

	zCEventManager *__fastcall GetEM(bool dontCreate) { XCALL(0x005D49B0); }
	void SetSleeping(bool sleep) { if (sleep) this->SetSleepingMode(zVOB_SLEEPING); else this->SetSleepingMode(zVOB_AWAKE); }
	float GetDistanceToVob2(zCVob &v) { XCALL(0x005EE530); }
	void SetPositionWorld(zVEC3 &posWorld) { XCALL(0x005EE650); }
	void SetPhysicsEnabled(bool enable) { XCALL(0x005EFC20); }
	void BeginMovement() { XCALL(0x005F0510); }
	void SetCollisionClass(zCCollisionObjectDef *collClass) { XCALL(0x005F1080); }
	void RemoveVobFromWorld() { XCALL(0x005D6670); }
	void RotateLocalX(float angle) { XCALL(0x005EE1A0); }
	void RotateLocalY(float angle) { XCALL(0x005EE210); }
	void RotateLocalZ(float angle) { XCALL(0x005EE280); }
	void UpdateVisualDependencies() { XCALL(0x005D7460); }
	void SetTrafo(zMAT4 &intrafo) { XCALL(0x005EE6B0); }
	void SetVobName(const zSTRING &n) { XCALL(0x005D4970); }
	void __fastcall SetCollDetStat(bool b) { XCALL(0x005EF8D0); }
	void __fastcall SetCollDetDyn(bool b) { XCALL(0x005EF9C0); }
	void SetCollDet(bool b) { SetCollDetStat(b); SetCollDetDyn(b); }
	void SetAI(zCAIBase *cbai) { XCALL(0x005D3730); }
	zVEC3 GetPositionWorld() { return trafoObjToWorld.GetTranslation(); }
	zMAT4 GetTrafoModelNodeToWorld(zCModelNodeInst *modelNode) { XCALL(0x005D84D0); }
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
};

class zCVobLight : public zCVob
{
public:
	zCVobLightData lightData;
	zSTRING lightPresetInUse;

public:
	void SetRange(float r, bool doBackup) { XCALL(0x005DBAF0); }
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

	bool tremorToggle;
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

	bool screenFadeEnabled;
	zCOLOR screenFadeColor;
	bool cinemaScopeEnabled;
	zCOLOR cinemaScopeColor;

	int projection;
	int drawMode;
	int shadeMode;
	bool drawWire;

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
	void SetTransform(int trafoType, zMAT4 &trafo) { XCALL(0x00536300); }
	zMAT4 &GetTransform(int trafoType) { XCALL(0x00536460); }
	void SetFarClipZ(float z) { XCALL(0x00536D30); }
	void AddTremor(zVEC3 &posWorldSpace, float radiusSquare, float timeMsec, zVEC3 &amplitude) { XCALL(0x00537140); }
	zVEC3 Transform(zVEC3 &point) { return camMatrix * point; }
};

class zCDecal : public zCVisual
{
public:
	zCMaterial *decalMaterial;

	float xdim;
	float ydim;
	float xoffset;
	float yoffset;
	bool decal2Sided;

public:
	virtual bool Render(zTRenderContext &renderContext) { XCALL(0x00542280); }

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
	virtual bool Render(zTRenderContext &renderContext) { XCALL(0x0059BF50); }

	void SetVisibleSegments(float visibleFirst, float visibleLast) { XCALL(0x0059BE80); }
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

	bool bFadeInOut;
	int fadeState;

	bool fadeIn;
	bool fadeOut;

	zCSoundFX *sfx1;
	zTSoundHandle sfxHandle1;
	zCSoundFX *sfx2;
	zTSoundHandle sfxHandle2;
	zCSoundFX *sfx3;
	zTSoundHandle sfxHandle3;
	zCSoundFX *sfx4;
	zTSoundHandle sfxHandle4;

	zCDecal *thunderStartDecal;

	bool activeThunder_Sector1;
	bool activeThunder_Sector2;
	bool activeThunder_Sector3;
	bool activeThunder_Sector4;

	zVEC2 *originalTexUVList;

public:
	void Initialise(int newNumMaxThunders) { XCALL(0x00630470); }
	void AddTremor(zTRenderContext &renderContext) { XCALL(0x00630570); }
	void RenderLayer(zTRenderContext &rndContext, int layerNumber, int &addNewThunder) { XCALL(0x00630580); }
	bool Render(zTRenderContext &rndContext, bool fadeInOut, bool alwaysVisible) { XCALL(0x006307C0); }
	void RemoveThunder(myThunder *thunder) { XCALL(0x00631220); }
	int AddThunderSub(myThunder *rootThunder, int startIndex, int startNexIntex, int length, int mumSplits) { XCALL(0x006312D0); }
	int AddThunder(int startIndex, int length, float random, int sector) { XCALL(0x00631620); }
	bool RenderThunder(myThunder *thunder, zTRenderContext &rndContext) { XCALL(0x00631D10) }
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
	virtual void SetFog(bool foggy) = 0;
	virtual bool GetFog() = 0;
	virtual void SetFogColor(zCOLOR &col) = 0;
	virtual zCOLOR GetFogColor() = 0;
	virtual void SetFogRange(float nearz, float farz, int falloff) = 0;
	virtual void GetFogRange(float &nearz, float &farz, int &falloff) = 0;
	virtual int GetPolyDrawMode() = 0;
	virtual void SetPolyDrawMode(int &drawMode) = 0;
	virtual void SetTextureWrapEnabled(bool b) = 0;
	virtual bool GetTextureWrapEnabled() = 0;
	virtual void SetBilerpFilterEnabled(bool b) = 0;
	virtual bool GetBilerpFilterEnabled() = 0;
	virtual void SetDitherEnabled(bool b) = 0;
	virtual bool GetDitherEnabled() = 0;
	virtual int GetPolySortMode() = 0;
	virtual void SetPolySortMode(int &smode) = 0;
	virtual bool GetZBufferWriteEnabled() = 0;
	virtual void SetZBufferWriteEnabled(bool flag) = 0;

	zTRnd_AlphaBlendFunc AlphaBlendFuncStringToType(const zSTRING &s) { XCALL(0x005AE600); }
};

class zCRnd_D3D : public zCRenderer
{
public:
	virtual ~zCRnd_D3D() { XCALL(0x00713D30); }
	virtual void BeginFrame() { XCALL(0x00716F60); }
	virtual void EndFrame() { XCALL(0x00717150); }
	virtual void FlushPolys() { XCALL(0x00716F50); }
	virtual void DrawPoly(zCPolygon *poly) { XCALL(0x00714B60); }
	virtual void DrawLightmapList(zCPolygon **polyList, int numPoly) { XCALL(0x00716860); }
	virtual void DrawLine(float x1, float x2, float y1, float y2, zCOLOR col) { XCALL(0x00716B00); }
	virtual void DrawLineZ(float x1Scr, float y1Scr, float z1CamSpaceInv, float x2Scr, float y2Scr, float z2CamSpaceInv, zCOLOR col) { XCALL(0x00716D20); }
	virtual void SetPixel(float x, float y, zCOLOR col) { XCALL(0x00716920); }
	virtual void DrawPolySimple(zCTexture *texture, zTRndSimpleVertex *vertex, int numVert) { XCALL(0x007143F0); }
	virtual void SetFog(bool foggy) { XCALL(0x00719B70); }
	virtual bool GetFog() { XCALL(0x00719C10); }
	virtual void SetFogColor(zCOLOR &col) { XCALL(0x00719C20); }
	virtual zCOLOR GetFogColor() { XCALL(0x00719C50); }
	virtual void SetFogRange(float nearz, float farz, int falloff) { XCALL(0x00719C60); }
	virtual void GetFogRange(float &nearz, float &farz, int &falloff) { XCALL(0x00719CB0); }
	virtual int GetPolyDrawMode() { XCALL(0x00719CF0); }
	virtual void SetPolyDrawMode(int &drawMode) { XCALL(0x00719CE0); }
	virtual void SetTextureWrapEnabled(bool b) { XCALL(0x00719D00); }
	virtual bool GetTextureWrapEnabled() { XCALL(0x00719D10); }
	virtual void SetBilerpFilterEnabled(bool b) { XCALL(0x00719D20); }
	virtual bool GetBilerpFilterEnabled() { XCALL(0x00719DA0); }
	virtual void SetDitherEnabled(bool b) { XCALL(0x00719DB0); }
	virtual bool GetDitherEnabled() { XCALL(0x00719DC0); }
	virtual int GetPolySortMode() { XCALL(0x00719E20); }
	virtual void SetPolySortMode(int &smode) { XCALL(0x00719DD0); }
	virtual bool GetZBufferWriteEnabled() { XCALL(0x00719E50); }
	virtual void SetZBufferWriteEnabled(bool flag) { XCALL(0x00719E30); }
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
	zCTexture *texBox[5];
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
	bool sunOn;
	bool cloudShadowOn;

	zCSkyLayerData layer[2];
};

class zCSkyControler : public zCObject
{
public:
	static int &s_skyEffectsEnabled;

	zCOLOR *polyLightCLUTPtr;
	float cloudShadowScale;

	zCOLOR backgroundColor;
	bool fillBackground;
	zCTexture *backgroundTexture;
};

class zCSkyControler_Mid : public zCSkyControler
{
public:
	bool underwaterFX;
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
	};

public:
	static zCClassDef &classDef;

	bool initDone;
	float masterTime;
	float masterTimeLast;

	zCSkyState masterState;
	zCSkyState *state0;
	zCSkyState *state1;

	zCArray<zCSkyState *> stateList;

	zCOLOR polyLightClut[256];
	int relightCtr;
	float lastRelightTime;

	float dayCounter;

	zCArray<zVEC3> fogColorDayVariations;

	float resultFogScale;
	float heightFogMinY;
	float heightFogMaxY;
	float userFogFar;
	float resultFogNear;
	float resultFogFar;
	float resultFogSkyNear;
	float resultFogSkyFar;
	zCOLOR resultFogColor;
	float userFarZScalability;

	zCSkyState *skyLaterState[2];
	zCSkyLayer skyLayer[2];
	zCSkyLayer skyLayerRainClouds;
	zCTexture *skyCloudLayerTex;

	zCSkyPlanet planets[2];

	zCVob *vobSkyPfx;
	float skyPFXTimer;

	zTRainFX rainFX;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	void ReadFogColorsFromINI() { XCALL(0x005BD970); }
	void RenderSkyPre() { XCALL(0x005C0900); }
};

class oCSkyControler_Barrier : public zCSkyControler_Outdoor
{
public:
	oCBarrier *barrier;
	bool bFadeInOut;

public:
	void RenderSkyPre() { XCALL(0x00632140); }
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
	bool drawVobBBox3D;
	int leafsRendered;
	int vobsRendered;
	int masterFrameCtr;
	zCPolygon **actPolyPtr;
	bool compiled;
};

class zCVobBBox3DSorter
{
public:
	zCArray<zCBBox3DSorterBase::zTBoxSortHandle *> handles;
	zCArraySort<zTNode *> nodeList[3];
	bool sorted;

public:
	virtual ~zCVobBBox3DSorter() { XCALL(0x005FA0E0); }
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

	bool compiled;
	bool compiledEditorMode;
	bool traceRayIgnoreVobFlag;
	int worldRenderMode;
	zCWayNet *wayNet;
	int masterFrameCtr;
	float vobFarClipZ;
	float vobFarClipZScalability;

	zCArray<zCVob *> traceRayVobList;
	zCArray<zCVob *> traceRayTempIgnoreVobList;

	bool renderingFirstTime;
	bool showWaynet;
	bool showTraceRayLines;
	bool showTextureStats;

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

	bool addZonesToWorld;
	bool showZonesDebugInfo;

	zCCBspTree *cbspTree;
	zCBspTree bspTree;

	zCArray<zCVob *> activeVobList;
	zCArray<zCVob *> walkList;

	zCArray<zCVob *> vobHashTable[2048];

public:
	bool __fastcall TraceRayFirstHit(zVEC3 &rayOrigin, zVEC3 &ray, zCArray<zCVob *> *ignoreVobList, int traceFlags) { XCALL(0x005F40C0); }
	bool __fastcall TraceRayFirstHit(zVEC3 &rayOrigin, zVEC3 &ray, zCVob *ignoreVob, int traceFlags) { XCALL(0x005F4590); }
	bool __fastcall TraceRayNearestHit(zVEC3 &rayOrigin, zVEC3 &ray, zCArray<zCVob *> *ignoreVobList, int traceFlags) { XCALL(0x005F42C0); }
	bool __fastcall TraceRayNearestHit(zVEC3 &rayOrigin, zVEC3 &ray, zCVob *ignoreVob, int traceFlags) { XCALL(0x005F46E0); }
	zCTree<zCVob *> *AddVob(zCVob *vob) { XCALL(0x005F6340); }
};

class zCCSPlayer
{
public:
	zCCSCutsceneContext *GetPlayingGlobalCutscene() { XCALL(0x00420020); }
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
		bool isAmbient3D;
		float pitchOffset;
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
	virtual bool GetSound3DProps(zTSoundHandle &sfxHandle, zTSound3DParams &sound3DParams) = 0;
	virtual bool UpdateSound3D(zTSoundHandle &sfxHandle, zTSound3DParams *sound3DParams) = 0;
	virtual void GetSoundProps(zTSoundHandle &sfxHandle, int &freq, float &vol, float &pan) = 0;
	virtual void UpdateSoundProps(zTSoundHandle &sfxHandle, int freq, float vol, float pan) = 0;
	virtual bool IsSoundActive(zTSoundHandle &sfxHandle) = 0;
};

class zCSndSys_MSS : public zCSoundSystem
{
public:
	virtual ~zCSndSys_MSS() { XCALL(0x004DDFF0); }
	virtual zCSoundFX *LoadSoundFX(zSTRING &fileName) { XCALL(0x004E0370); }
	virtual zCSoundFX *LoadSoundFXScript(zSTRING &scriptIdentifier) { XCALL(0x004E0B50); }
	virtual zCParser *GetSFXParser() { XCALL(0x004E6130); }
	virtual float GetPlayingTimeMSEC(zSTRING &fileName) { XCALL(0x004E5FA0); }
	virtual void SetSound3DDefaultRadius(float defRad) { XCALL(0x004DDE20); }
	virtual float GetSound3DDefaultRadius() { XCALL(0x004DDE30); }
	virtual void SetMasterVolume(float vol) { XCALL(0x004E02E0); }
	virtual float GetMasterVolume() { XCALL(0x004E0130); }
	virtual zTSoundHandle PlaySound(zCSoundFX *sfx, int slot) { XCALL(0x004E2140); }
	virtual zTSoundHandle PlaySound(zCSoundFX *sfx, int slot, int freq, float vol, float pan) { XCALL(0x004E3220); }
	virtual zTSoundHandle PlaySound3D(zCSoundFX *sfx, zCVob *sourceVob, int vobSlot, zTSound3DParams *sound3DParams) { XCALL(0x004E37D0); }
	virtual zTSoundHandle PlaySound3D(zSTRING &soundName, zCVob *sourceVob, int vobSlot, zTSound3DParams *sound3DParams) { XCALL(0x004E3740); }
	virtual void StopSound(zTSoundHandle &sfxHandle) { XCALL(0x004E4610); }
	virtual void StopAllSounds() { XCALL(0x004E46D0); }
	virtual bool GetSound3DProps(zTSoundHandle &sfxHandle, zTSound3DParams &sound3DParams) { XCALL(0x004E5830); }
	virtual bool UpdateSound3D(zTSoundHandle &sfxHandle, zTSound3DParams *sound3DParams) { XCALL(0x004E4700); }
	virtual void GetSoundProps(zTSoundHandle &sfxHandle, int &freq, float &vol, float &pan) { XCALL(0x004E5630); }
	virtual void UpdateSoundProps(zTSoundHandle &sfxHandle, int freq, float vol, float pan) { XCALL(0x004E5A20); }
	virtual bool IsSoundActive(zTSoundHandle &sfxHandle) { XCALL(0x004E5E90); }
};

class zCOption
{
public:
	bool ReadBool(zSTRING &secName, char *name, bool value) { XCALL(0x0045CB80); }
};

class zCInputCallback
{
public:
	virtual bool HandleEvent(int key) { XCALL(0x00439A60); }
};

class zCSoundManager
{
public:
	enum zTSndManMedium { };
};

class zCBspBase
{
public:
	void __fastcall CollectVobsInBBox3D(zCArray<zCVob *> &resVobList, zTBBox3D &inbbox3D) { XCALL(0x0051E7C0); }
};

struct TNpcAIState
{
	int index;
	int loop;
	int end;
	int timeBehaviour;
	float restTime;
	int phase;
	bool valid;
	zSTRING name;
	float stateTime;
	int prgIndex;
	bool isRtnState;
};

struct TNpcPerc
{
	int percID;
	int percFunc;
};

struct TNpcSlot
{
	zSTRING name;
	bool inInventory;
	bool tmpLevel;
	zSTRING itemName;

	oCVob *vob;

	bool wasVobTreeWhenInserted : 1;
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
	virtual bool IsOwnedByGuild(int guild) { return FALSE; }
	virtual bool IsOwnedByNpc(int instance) { return FALSE; }
	virtual bool DoFocusCheckBBox() { return FALSE; }
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

	bool inScriptStartup;
	bool inLoadSaveGame;
	bool inLevelChange;

	zCView *array_view[6];
	bool array_view_visible[6];
	bool array_view_enabled[6];

	oCSavegameManager *savegameManager;
	zCView *game_text;
	zCView *load_screen;
	zCView *save_screen;
	zCView *pause_screen;
	oCViewStatusBar *hpBar;
	oCViewStatusBar *swimBar;
	oCViewStatusBar *manaBar;
	oCViewStatusBar *focusBar;
	bool showPlayerStatus;

	bool game_drawall;
	bool game_frameinfo;
	bool game_showaniinfo;
	bool game_showwaynet;
	bool game_testmode;
	bool game_editwaynet;
	bool game_showtime;
	bool drawWayBoxes;
	bool scriptStartup;
	bool showFreePoints;
	oCNpc *showRoutineNpc;

	bool loadNextLevel;
	zSTRING loadNextLevelName;
	zSTRING loadNextLevelStart;

	zVEC3 startpos;
	oCGameInfo *gameInfo;
	zCVobLight *pl_light;
	float pl_lightval;

	oCWorldTimer *wldTimer;
	float timeStep;
	bool singleStep;

	oCGuilds *guilds;
	oCInfoManager *infoman;
	oCNewsManager *newsman;
	oCSVMManger *svmman;
	oCTradeManager *trademan;
	oCPortalRoomManager *portalman;
	oCSpawnManager *spawnman;

	float music_delay;
	oCNpc *watchnpc;

	int initial_hour;
	int initial_minute;

	zCArray<zCVob *> debugInstances;
	int debugChannels;
	bool debugAllInstances;

	int oldRoutineDay;
	zCListSort<TObjectRoutine> objRoutineList;
	zCListSort<TObjectRoutine> *currentObjectRoutine;
	zCViewProgressBar *progressBar;
	zCArray<zCVisual *> visualList;

public:
	void Render() { XCALL(0x0063DBE0); }
	oCWorld *GetGameWorld() { XCALL(0x00638A90); }
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
	void DoCheck() { XCALL(0x0046D630); }
	void DoWarningFX(int level) { XCALL(0x0046E380); }
	void DisposeWarningFX() { XCALL(0x0046E550); }
	void DoShootFX(zVEC3 &startPoint) { XCALL(0x0046E590); }
	void DisposeShootFX() { XCALL(0x0046E8A0); }
};

class oCNpc_States
{
public:
	zSTRING name;

	oCNpc *npc;
	TNpcAIState curState;
	TNpcAIState nextState;
	int lastAIState;

	bool hasRoutine;
	bool rtnChanged;
	oCRtnEntry *rtnBefore;
	oCRtnEntry *rtnNow;
	zCRoute *rtnRoute;
	bool rtnOverlay;
	int rtnOverlayCount;
	int walkmode_routine;
	bool weaponmode_routine;
	bool startNewRoutine;

	int aiStateDriven;
	zVEC3 aiStatePosition;

	oCNpc *parOther;
	oCNpc *parVictim;
	oCItem *parItem;

	int rtnChangeCount;

public:
	virtual void Archive(zCArchiver &arc) { XCALL(0x006C7810); }

	bool StartAIState(zSTRING &name, bool endOldState, int timeBehaviour, float timed, bool isRtnState) { XCALL(0x006C5350); }
	void ClearAIState() { XCALL(0x006C61A0); }
	void EndCurrentState() { XCALL(0x006C6340); }
	bool IsInState(int stateID) { XCALL(0x006C4C90); }
};

class oCItemContainer : public zCInputCallback
{
public:
	zCListSort<oCItem> *contents;
	oCNpc *npc;
	int selectedItem;
	int offset;
	int drawItemMax;
	int itemListMode;
	bool frame;
	bool right;
	bool ownList;
	bool prepared;
	bool passive;

	zCView *viewCat;
	zCView *viewItem;
	zCView *viewItemActive;
	zCView *viewItemHightlighted;
	zCView *viewItemActiveHighlighted;
	zCView *viewItemFocus;
	zCView *viewItemActiveFocus;
	zCView *viewItemHighlightedFocus;
	zCView *viewItemActiveHighlightedFocus;
	zCView *viewItemInfo;
	zCView *viewItemInfoItem;
	zCView *textView;
	zCView *viewArrowAtTop;
	zCView *viewArrowAtBottom;

	zCWorld *rndWorld;
	int posx;
	int posy;
	zSTRING textCategoryStatic;
	bool m_bManipulateItemsDisabled;
	bool m_bCanTransferMoreThanOneItem;
	zCOLOR image_chroma;
	zCOLOR blit_chroma;
};

#define INV_MAX 9

class oCNpcInventory : public oCItemContainer
{
public:
	oCNpc *owner;
	bool packAbility;

	zCListSort<oCItem> inventory[INV_MAX];

	zSTRING packString[INV_MAX];
	int offset[INV_MAX];
	int itemnr[INV_MAX];
	int maxSlots[INV_MAX];
	int invnr;

public:
	bool IsEmpty() { XCALL(0x0066D550); }
};

class oCNewsMemory
{
public:
	zCList<oCNews> iknow;

public:
	virtual void Archive(zCArchiver &arc) { XCALL(0x00687FC0); }
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
#define NPC_TAL_MAX 12

#define NPC_MIS_MAX 5

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
	static bool &godmode;

	int idx;
	zSTRING name[5];
	zSTRING slot;

	int npcType;
	int variousFlags;
	int attribute[NPC_ATR_MAX];
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
	int aiscriptvars[50];
	zSTRING wpname;
	int experience_points;
	int experience_points_next_level;
	int learn_points;
	int parserEnd;

	bool bloodEnabled;
	int bloodDistance;
	int bloodAmount;
	int bloodFlow;
	zSTRING bloodEmitter;
	zSTRING bloodTexture;
	bool didHit;
	bool didParade;
	bool didShoot;
	bool hasLockedEnemy;
	bool isDefending;
	bool wasAming;
	oCNpc *enemy;

	float speedTurn;
	bool foundFleePoint;
	bool reachedFleePoint;
	zVEC3 vecFlee;
	zVEC3 posFlee;
	zCWaypoint *waypointFlee;
	oTRobustTrace rbt;

	zCList<oCNpcTimedOverlay> timedOverlays;
	zCArray<oCNpcTalent *> talents;

	int spellMana;

	oCVisualFX *visualFX;
	oCMagFrontier magFrontier;
	oCNpc_States state;
	oCNpcInventory inventory2;
	oCItemContainer *trader;
	oCNpc *tradeNpc;

	float rangeToPlayer;

	zCArray<zTSoundHandle> listOfVoiceHandles;
	int voiceIndex;

	bool showaidebug : 1;
	bool showNews : 1;
	bool csAllowedAsRole : 1;

	bool isSummoned : 1;
	bool respawnOn : 1;
	bool movlock : 1;
	bool drunk : 1;
	bool mad : 1;
	bool overlay_wounded : 1;
	bool inOnDamage : 1;
	bool autoremoveweapon : 1;
	bool openinventory : 1;
	bool askroutine : 1;
	bool spawnInRange : 1;

	int body_TexVarNr : 16;
	int body_TexColorNr : 16;
	int head_TexVarNr : 16;
	int teeth_TexVarNr : 16;
	int guildTrue : 8;
	int drunk_heal : 8;
	int mad_heal : 8;
	int spells : 8;
	int bodyState : 19;

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
	bool knowsPlayer;

	TNpcPerc percList[NPC_PERC_MAX];
	int percActive;
	float percActiveTime;
	float percActiveDelta;

	bool overrideFallDownHeight;
	float fallDownHeight;
	int fallDownDamage;

	oCMag_Book *mag_book;
	zCList<oCSpell> activeSpells;

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

	float fight_waitTime;
	int fight_waitForAniEnd;
	float fight_lastStrafeFrame;

	int soundType;
	zCVob *soundVob;
	zVEC3 soundPosition;

	zCPlayerGroup *playerGroup;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	void OnDamage_Hit(oSDamageDescriptor &descDamage) { XCALL(0x00731410); }
	void OnDamage_Events(oSDamageDescriptor &descDamage) { XCALL(0x00746950); }
	void StandUp(bool walkingallowed, bool startAniTransition) { XCALL(0x0074D5A0); }
	zCModel *GetModel() { XCALL(0x00695300); }
	oCSpell *IsSpellActive(int nr) { XCALL(0x00699F70); }
	void InsertActiveSpell(oCSpell *spell) { XCALL(0x00699CA0); }
	bool CheckForOwner(zCVob *v) { XCALL(0x0069CED0); }
	void SetAsPlayer() { XCALL(0x0069EAE0); }
	void ClearVobList() { XCALL(0x006B6EB0); }
	void CreateVobList(float max_dist) { XCALL(0x006B7110); }
	bool HasBodyStateModifier(int nr) { XCALL(0x006B8250); }
	void SetBodyStateModifier(int nr) { XCALL(0x006B8270); }
	int ModifyBodyState(int add, int remove) { XCALL(0x006B85B0); }
	void CreatePassivePerception(int percType, zCVob *other, zCVob *victim) { XCALL(0x006B49C0); }
	bool IsDead() { return this->attribute[NPC_ATR_HITPOINTS] <= 0; }
	bool IsUnconscious() { return this->state.IsInState(NPC_AISTATE_UNCONSCIOUS); }
	bool IsFadingAway() { return this->state.IsInState(NPC_AISTATE_FADEAWAY); }
	bool IsHuman() { XCALL(0x0069EA90); }
	bool IsOrc() { XCALL(0x0069EAB0); }
	bool IsGoblin() { XCALL(0x0069EAA0); }
	bool IsSkeleton() { XCALL(0x0069EAD0); }
	bool IsMonster() { XCALL(0x0069EA30); }
	bool IsHalfMonster() { return !this->IsHuman() && !this->IsOrc() && !this->IsGoblin() && !this->IsSkeleton(); }
	int GetWeaponMode() { XCALL(0x00695820); }
	int GetTalentValue(int talentNr) { XCALL(0x0068E570); }
	bool HasFlag(unsigned int dwValue, unsigned int dwFlag) { XCALL(0x00747B40); }
	bool HasFlag(int nr) { XCALL(0x0068E150); }
	bool IsSelfPlayer() { return this == oCNpc::player; }
	void ChangeAttribute(int nr, int value) { XCALL(0x0068D7C0); }
	bool AssessMagic_S(oCNpc *other, int spellType) { XCALL(0x006B62F0); }
	bool AssessStopMagic_S(oCNpc *other, int spellType) { XCALL(0x006B65F0); }
};

enum zTPFX_EmitterShape { };
enum zTPFX_EmitterFOR { };
enum zTPFX_DistribType { };
enum zTPFX_EmitterDirMode { };
enum zTPFX_EmitterVisOrient { };

class zCParticleEmitter
{
public:
	float ppsValue;
	zSTRING ppsScaleKeys_S;
	bool ppsIsLooping;
	bool ppsIsSmooth;
	float ppsFPS;
	zSTRING ppsCreateEm_S;
	float ppsCreateEmDelay;

	zSTRING shpType_S;
	zSTRING shpFOR_S;
	zSTRING shpOffsetVec_S;
	zSTRING shpDistribType_S;
	float shpDistribWalkSpeed;
	bool shpIsVolume;
	zSTRING shpDim_S;
	zSTRING shpMesh_S;
	bool shpMeshRender_B;
	zSTRING shpScaleKeys_S;
	bool shpScaleIsLooping;
	bool shpScaleIsSmooth;
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
	bool flyCollDet_B;

	zSTRING visName_S;
	zSTRING visOrientation_S;
	bool visTexIsQuadPoly;
	float visTexAniFPS;
	bool visTexAniIsLooping;

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

	int endOfDScriptPart;

	zSTRING particleFXName;

	zCArray<float> ppsScaleKeys;
	zCParticleEmitter *ppsCreateEmitter;

	zTPFX_EmitterShape shpType;
	float shpCircleSphereRadius;
	zVEC3 shpLineBoxDim;
	zCMesh *shpMesh;
	zCPolygon *shpMeshLastPoly;

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
	zCMesh *visMesh;
	zTPFX_EmitterVisOrient visOrientation;

	zVEC2 visSizeStart;

	zVEC3 visTexColorRGBAStart;
	zVEC3 visTexColorRGBAEnd;

	zTRnd_AlphaBlendFunc visAlphaFunc;

	zCTexture *trlTexture;
	zCTexture *mrkTexture;

	bool isOneShotFX;
	float dirAngleHeadVarRad;
	float dirAngleElevVarRad;
};

struct zSParticle
{
	zSParticle *next;
	zVEC3 position;
	zVEC3 vel;
	float lifeSpan;
	float alpha;
	float alphaVel;
	zVEC2 size;
	zVEC2 sizeVel;
	zVEC3 color;
	zVEC3 colorVel;
	float texAniFrame;
	zCPolyStrip *polyStrip;
};

typedef zSParticle zTParticle;

class zCParticleEmitterVars
{
public:
	float ppsScaleKeysActFrame;
	float ppsNumParticlesFraction;
	float ppsTotalLifeTime;
	bool ppsDependentEmitterCreated;

	float shpScaleKeysActFrame;
	float uniformValue;
	float uniformDelta;
};

class zCParticleFX : public zCVisual
{
public:
	static zCClassDef &classDef;

	zTParticle *firstPart;
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

	zCParticleFX *nextPfx;
	zCParticleFX *prevPFx;
	float privateTotalTime;
	float lastTimeRendered;

	float timeScale;
	float localFrameTimeF;
	zCQuadMark *quadMark;
	zTBBox3D quadMarkBBox3DWorld;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	bool CalcIsDead() { XCALL(0x0058F0B0); }
	void StopEmitterOutput() { this->isOneShotFX = TRUE; }
};

class zCModel : public zCVisualAnimate
{
public:
	static zCClassDef &classDef;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	zCModelAni *GetAniFromAniID(int aniID) { XCALL(0x0046D1E0); }
	bool IsAniActive(zSTRING &aniName);
	bool IsAniActive(zCModelAni *modelAni) { XCALL(0x0046D220); }
	int GetAniIDFromAniName(zSTRING &aniName) { XCALL(0x0047EC50); }
	zCModelNodeInst *SearchNode(zSTRING &nodeName) { XCALL(0x00563F80); }
};

class oCAniCtrl_Human : public zCObject
{
public:
	void SearchStandAni(bool forceStartAni) { XCALL(0x0061C700); }
	int GetWaterLevel() { XCALL(0x006B89D0); }
	bool IsInWater() { return this->GetWaterLevel() > 0; }
};

#define SPL_STATUS_DONTINVEST 0
#define SPL_STATUS_CANINVEST 1
#define SPL_STATUS_CAST 2
#define SPL_STATUS_STOP 3
#define SPL_STATUS_NEXTLEVEL 4

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

#define SPL_LIGHT 0
#define SPL_FIREBALL 1
#define SPL_TRANSFORM 2
#define SPL_FEAR 3
#define SPL_HEAL 4
#define SPL_LIGHTNING 5
#define SPL_SUMMONDEMON 6
#define SPL_SUMMONSKELETON 7
#define SPL_FORGET 8
#define SPL_WINDFIST 9
#define SPL_TELEKINESIS 10
#define SPL_CHARM 11
#define SPL_SLEEP 12
#define SPL_PYROKINESIS 13
#define SPL_MASSDEATH 14
#define SPL_CONTROL 15
#define SPL_DESTROYUNDEAD 16
#define SPL_FIREBOLT 17
#define SPL_FIRESTORM 18
#define SPL_FIRERAIN 19
#define SPL_SPEED 20
#define SPL_TELEPORT1 21
#define SPL_TELEPORT2 22
#define SPL_TELEPORT3 23
#define SPL_TELEPORT4 24
#define SPL_TELEPORT5 25
#define SPL_TRF_BLOODFLY 26
#define SPL_TRF_BLOODHOUND 27
#define SPL_TRF_CRAWLER 28
#define SPL_TRF_LURKER 29
#define SPL_TRF_MEATBUG 30
#define SPL_TRF_MOLERAT 31
#define SPL_TRF_ORCDOG 32
#define SPL_TRF_RAZOR 33
#define SPL_TRF_SCAVENGER 34
#define SPL_TRF_SCAVENGER2 35
#define SPL_TRF_SHADOWBEAST 36
#define SPL_TRF_SNAPPER 37
#define SPL_TRF_WARAN 38
#define SPL_TRF_WOLF 39
#define SPL_CHAINLIGHTNING 40
#define SPL_THUNDERBOLT 41
#define SPL_THUNDERBALL 42
#define SPL_ICECUBE 43
#define SPL_ICEWAVE 44
#define SPL_SUMMONGOLEM 45
#define SPL_ARMYOFDARKNESS 46
#define SPL_STORMFIRST 47
#define SPL_TELEKINESIS2 48
#define SPL_BREATHOFDEATH 49
#define SPL_SHRINK 50
#define SPL_UNDRESS 51
#define SPL_DANCE 52
#define SPL_BERZERK 53
#define SPL_NEW1 54
#define SPL_NEW2 55
#define SPL_NEW3 56
#define SPL_NEW4 57
#define SPL_NEW5 58
#define SPL_MAX_SPELL 59

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
	bool spellEnabled;
	bool spellInitDone;

	bool timerEffect;
	bool canBeDeleted;

	float up;
	float hoverY;
	float hoverOld;
	float hoverDir;

	int spellEnergy;

	float manaInvestTime;
	int damagePerLevel;
	int damageType;
	int spellType;
	bool canTurnDuringInvest;
	bool canChangeTargetDuringInvest;
	int isMultiEffect;
	int targetCollectAlgo;
	int targetCollectType;
	int targetCollectRange;
	int targetCollectAzi;
	int targetCollectElev;

public:
	oCVisualFX *CreateEffect() { XCALL(0x0047BED0); }
	void StopTargetEffects(zCVob *vob) { XCALL(0x0047D790); }
	bool IsValidTarget(zCVob *v) { XCALL(0x0047DD80); }
	bool IsInvestSpell() { XCALL(0x0047E340); }
	void DoLogicInvestEffect() { XCALL(0x0047E690); }
	bool CastSpecificSpell() { XCALL(0x0047EC70); }
	void EndTimedEffect() { XCALL(0x0047F330); }
	void DoTimedEffect() { XCALL(0x0047F8F0); }
	void SetReleaseStatus() { XCALL(0x0047E3B0); }
	void CallScriptInvestedMana() { XCALL(0x0047D930); }
	zSTRING GetSpellInstanceName(int _spellID);
};

class oCMOB : public oCVob
{
public:
	static zCClassDef &classDef;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	bool IsMoveable() { XCALL(0x0067AD40); }
};

class oCMobInter : public oCMOB
{
public:
	static zCClassDef &classDef;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }

	bool IsOccupied() { return FALSE; } // TODO ???
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
	void SetByList(zCArray<zCVob *> vobList) { XCALL(0x004810D0); }
	int VobCross(zCVob *vob) { XCALL(0x00481260); }
};

class oCEmitterKey
{
public:
	zSTRING visName_S;
	float visSizeScale;
	float scaleDuration;

	float pfx_ppsValue;
	bool pfx_ppsIsSmoothChg;
	bool pfx_ppsIsLoopingChg;

	float pfx_scTime;
	zSTRING pfx_flyGravity_S;

	zSTRING pfx_shpDim_S;
	bool pfx_shpIsVolumeChg;
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
	bool sfxIsAmbient;

	zSTRING emCreateFXID;
	float emFlyGravity;
	zSTRING emSelfRotVel_S;
	zSTRING emTrjMode_S;
	float emTrjEaseVel;
	bool emCheckCollision;
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

#define VFX_NUM_USERSTRINGS 3

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
	bool visTexAniIsLooping;
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
	zSTRING emFXCollStatAlign_S;
	zSTRING emFXCollDynAlign_S;
	float emFXLifeSpan;

	bool emCheckCollision;
	bool emAdjustShpToOrigin;
	float emInvestNextKeyDuration;
	float emFlyGravity;
	zSTRING emSelfRotVel_S;
	zSTRING userString[VFX_NUM_USERSTRINGS];

	zSTRING lightPresetName;
	zSTRING sfxID;
	bool sfxIsAmbient;
	bool sendAssessMagic;
	float secsPerDamage;

	byte dScriptEnd; // isLightning

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

	bool fxInvestOriginInitialized;
	bool fxInvestTargetInitialized;
	bool fxInvestStopped;

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

	int level;
	bool collisionOccured;
	int collisionCtr;
	bool showVisual;
	bool isChild;
	bool isDeleted;
	bool initialized;
	bool shouldDelete;
	bool lightning;
	int queueSetLevel;

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

	float damage;
	int damageType;

	int spellType;
	int spellTargetTypes;
	float savePpsValue;
	zVEC2 saveVisSizeStart;
	zVEC3 transRing[10];
	int ringPos;
	bool emTrjFollowHitLastCheck;
	bool bIsProjectile;

public:
	static zCClassDef *GetStaticClassDef() { return &classDef; }
	static oCVisualFX *_CreateNewInstance() { XCALL(0x0048FE60); }
	static oCVisualFX *CreateAndPlay(zSTRING &id, zCVob *org, zCVob *target, int level, float damage, int damageType, bool bIsProjectile) { XCALL(0x00486A70); }
	static oCVisualFX *CreateAndPlay(zSTRING &id, zVEC3 &orgPos, zCVob *target, int level, float damage, int damageType, bool bIsProjectile) { XCALL(0x00486D60); }

	oCVisualFX() { XCALL(0x004820C0); }

	virtual zCClassDef *_GetClassDef() { XCALL(0x004825E0); }
	virtual void Archive(zCArchiver &arc) { XCALL(0x0048FC40); }
	virtual void Unarchive(zCArchiver &arc) { XCALL(0x0048FC50); }
	virtual ~oCVisualFX() { XCALL(0x00482820); }

	virtual void OnTick() { XCALL(0x0048FB60); }
	virtual bool CanThisCollideWith(zCVob *vob) { XCALL(0x0048DEC0); }

	virtual void Open() { XCALL(0x004898F0); }
	virtual void SetOrigin(zCVob *orgVob) { XCALL(0x004892D0); }
	virtual void SetTarget(zCVob *targetVob) { XCALL(0x00489390); }
	virtual void SetTarget(zVEC3 &targetPos) { XCALL(0x004894B0); }
	virtual void SetInflictor(zCVob *inflictorVob) { XCALL(0x00489340); }
	virtual zCVob *GetOrigin() { return this->origin; }
	virtual zCVob *GetTarget() { return this->target; }
	virtual zCVob *GetInflictor() { return this->inflictor; }
	virtual void Init(zCVob *orgVob, zVEC3 &targetPos) { XCALL(0x00489E00); }
	virtual void Init(zCVob *orgVob, zCVob *trgtVob, zCVob *inflictorVob) { XCALL(0x00489F10); }
	virtual void Init(zCArray<zCVob *> &trajectoryVobs) { XCALL(0x0048A640); }
	virtual void InvestNext() { XCALL(0x0048A7D0); }
	virtual void SetLevel(int level) { XCALL(0x0048AB70); }
	virtual int GetLevel() { return this->level; }
	virtual void Cast(bool killAfterDone) { XCALL(0x0048AF40); }
	virtual void Stop(bool killAfterDone) { XCALL(0x0048BB30); }
	virtual void Kill() { XCALL(0x0048BEE0); }
	virtual void Play(float keyCycleTime, zMAT4 *orgTrafo, zMAT4 *targetTrafo) { }
	virtual bool CanBeDeleted() { XCALL(0x0048C150); }
	virtual bool IsFinished() { XCALL(0x0048C190); }
	virtual bool IsLooping() { XCALL(0x0048C220); }
	virtual void SetByScript(const zSTRING &id) { XCALL(0x00485790); }
	virtual void SetDuration(float fSecDuration) { this->emFXLifeSpan = fSecDuration; }
	virtual void Reset() { XCALL(0x00489C20); }
	virtual void ReportCollision(zCCollisionReport &collisionReport) { XCALL(0x0048C6F0); }
	virtual void SetCollisionEnabled(bool en) { XCALL(0x00485640); }
	virtual void SetCollisionCandidates(zCArray<zCVob *> collisionVobs) { XCALL(0x0048DCD0); }
	virtual void GetCollisionCandidates(zCArray<zCVob *> &collisionVobs) { XCALL(0x00482640); }
	virtual int GetNumCollisionCandidates() { return this->allowedCollisionVobList.numInArray; }
	virtual bool GetCollidedCandidates(zCArray<zCVob *> &collidedVobs) { XCALL(0x0048DDF0); }
	virtual void SetDamage(float dam) { this->damage = dam; }
	virtual void SetDamageType(int damType) { this->damageType = damType; }
	virtual float GetDamage() { return this->damage; }
	virtual int GetDamageType() { return this->damageType; }
	virtual bool IsASpell() { return this->sendAssessMagic; }
	virtual void SetSpellType(int _type) { this->spellType = _type; }
	virtual int GetSpellType() { return this->spellType; }
	virtual int GetSpellTargetTypes() { return this->spellTargetTypes; }
	virtual void SetSpellTargetTypes(int types) { this->spellTargetTypes = types; }
	virtual bool GetIsProjectile() { return this->bIsProjectile; }
	virtual void SetIsProjectile(bool b) { this->bIsProjectile = b; }
	virtual void SetVisualByString(const zSTRING &visName) { XCALL(0x00484580); }
	virtual void CalcTrajectory(bool &updateTargetOnly) { XCALL(0x00487870); }
	virtual void Collide(bool killAfterDone) { XCALL(0x0048B750); }
	virtual void CollisionResponse(zVEC3 &collisionNormal, bool alignCollNormal) { XCALL(0x0048D850); }

	void InitValues() { XCALL(0x00483B90); }
	oCVisualFX *CreateAndCastFX(const zSTRING &id, zCVob *org, zCVob *inflictor) { XCALL(0x00487170); }
};

class oCVisFX_MultiTarget : public oCVisualFX
{
public:
	zCArray<oCVisualFX *> visList;

public:
	static oCVisFX_MultiTarget *_CreateNewInstance() { XCALL(0x00498320); }
};

#define NUM_SCANNER_VOBS 3

class oCFXScanner
{
public:
	bool enabled;
	byte alpha;

	zCVob *initVob;
	zCVob *scannerVobs[NUM_SCANNER_VOBS];
};

class oCVisFX_Lightning : public oCVisualFX
{
public:
	int phase;
	float lightRange;
	
	bool showScanner;
	bool investedNext;
	bool castOnSelf;

	int unknown;

	zCModelNodeInst *targetNode;

	zCArray<zCVob *> burnVobs;
	zCArray<zCModelNodeInst *> burnNodes;
	zCArray<zCVob *> decalVobs;
	zCArray<oCVisualFX *> electricFX;

	oCFXScanner scanner;

public:
	void Draw() { XCALL(0x004960E0); }
	void UpdateBurnVobs() { XCALL(0x00495D50); }
	bool UpdateBurnVobsInvestNext() { XCALL(0x00496800); }
};

class zCTimer
{
public:
	float factorMotion;
	float frameTimeFloat;
	float totalTimeFloat;
	unsigned int lastTimer;
	unsigned int frameTime;
	unsigned int totalTime;
	unsigned int minFrameTime;
};

#define GAME_LEFT 1
#define GAME_RIGHT 2
#define GAME_UP 3

class zCInput_Win32
{
public:
	virtual ~zCInput_Win32() { XCALL(0x004C8250); }
	virtual float GetState(word logicalID) { XCALL(0x004C8310); }
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
	void GetValue(int &val, int nr) { XCALL(0x006F86E0); }
	void GetValue(zSTRING &s, int nr) { XCALL(0x006F8740); }
	void *GetInstanceAdr() { XCALL(0x006F84C0); }
};

class zCParser
{
public:
	zCPar_Symbol *GetSymbol(const zSTRING &s) { XCALL(0x006EA520); }
	void GetParameter(int &value) { XCALL(0x006F6DF0); }
	void GetParameter(zSTRING &s) { XCALL(0x006F6E40); }
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
	virtual ~zERROR() { XCALL(0x00447FD0); }

	int Report(int xLevel, int xId, const zSTRING &xMessage, char level, unsigned int flag, int line, char *file, char *function) { XCALL(0x00448250); }
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
