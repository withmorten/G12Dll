#ifdef PlaySound
#undef PlaySound
#endif

class zCObject;
class zCFileBIN;

struct zTBBox3D;

template<class T> class zCArray
{
public:
	T *array;
	int numAlloc;
	int numInArray;
};

class zSTRING
{
public:
	unsigned char allocator;
	char *_Ptr;
	int _Len;
	int _Res;

public:
	virtual ~zSTRING() { XCALL(0x00401100); };
	
	zSTRING() { XCALL(0x004035E0); }
	zSTRING(char *pstring) { XCALL(0x00401060); }

	zSTRING &operator+=(char *pstring) { XCALL(0x0066D650); }

	int Search(const char *substr, unsigned int num) { XCALL(0x0059F960); }
	int Delete(zSTRING &xStr, int) { XCALL(0x00469880); }
	zSTRING PickWord(unsigned int num, zSTRING &trenn, zSTRING &skip) { XCALL(0x00468B40); }

	void TrimLeft(char ch) { XCALL(0x0046A550); }
	void TrimRight(char ch) { XCALL(0x0046A690); }
};

class zFILE
{
public:
	virtual ~zFILE() { XCALL(0x0043CD50); }

	int Exists() { XCALL(0x0043D4F0); }
	int Open(unsigned char writeMode) { XCALL(0x0043DBF0); }
	int Close() { XCALL(0x0043E080); }
	unsigned char Eof() { XCALL(0x0043E350); }
	int Read(zSTRING &s) { XCALL(0x0043E7F0); }
	int Reset() { XCALL(0x0043E320); }
	long Pos() { XCALL(0x0043E360); }
	long Size() { XCALL(0x0043D740); }
	int Seek(long fpos) { XCALL(0x0043E370); }
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
	virtual zCClassDef *_GetClassDef() { XCALL(0x00402800); }

	int Release() { XCALL(0x00472A60); }
};

class zCMaterial : public zCObject
{
public:
	static zCMaterial *SearchName(zSTRING &name) { XCALL(0x00568B00); }
};

class zCObjectFactory
{
public:
	zFILE *CreateZFile(zSTRING &filename) { XCALL(0x005AE510); }
};

class zCMesh : public zCObject
{
public:
	void SaveMSH(zSTRING &fileName) { XCALL(0x00574880); }
	int LoadMSH(zCFileBIN &file) { XCALL(0x00575D80); }
};

class zCOption
{
public:
	void ChangeDir(int nr) { XCALL(0x00461BB0); }
	int Parm(zSTRING &parmname) { XCALL(0x00461CE0); }
};

class zCWorld
{
public:
	void GenerateStaticWorldLighting(int &lightmode, zTBBox3D *updateBBox3D) { XCALL(0x006233A0); }
};

class oCGame
{
public:
	int dummy[40];
	int game_testmode;

public:
	virtual int HandleEvent(int key) { XCALL(0x0068BE40); }

	void SaveWorld(zSTRING &pwf, int savemode, int savemesh) { XCALL(0x00662EF0); }
	void LoadWorld(int slotID, zSTRING &levelpath) { XCALL(0x00666430); }
	void CompileWorld() { XCALL(0x00666E00); }
};

class zCArchiver : public zCObject
{
public:
	void Close() { XCALL(0x00534430); }

	int __fastcall ReadChunkStart(zSTRING &chunkName, unsigned short &chunkVersion) { XCALL(0x00538740); }
	zSTRING __fastcall ReadString(char *entryName) { XCALL(0x00539790); }
	zCObject *__fastcall ReadObject(zCObject *objectUseThis) { XCALL(0x00538FD0); }
};

class zCArchiverFactory
{
public:
	zCArchiver *CreateArchiverRead(zFILE *fileRead, int arcFlags) { XCALL(0x0052B8F0); }
};
