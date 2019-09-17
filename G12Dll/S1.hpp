#ifdef PlaySound
#undef PlaySound
#endif

class zCObject;
class zCFileBIN;
class zCArchiver;

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
	unsigned char _Al;
	char *_Ptr;
	int _Len;
	int _Res;

public:
	virtual void *__vector_deleting_destructor(unsigned int __flags) { XCALL(0x00448710); }
	
	zSTRING() { XCALL(0x00401860); }
	zSTRING(char *pstring) { XCALL(0x004010C0); }
};

class zCClassDef
{
public:
	zSTRING className;
	zSTRING baseClassName;
	zSTRING scriptClassName;
	zCClassDef *baseClassDef;
	zCObject(*createNewInstance) (void);
	zCObject(*createNewInstanceBackup) (void);
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
	virtual zCClassDef *_GetClassDef() { XCALL(0x00401880); }
};

class zCMesh : public zCObject
{
public:
	void SaveMSH(zSTRING &fileName) { XCALL(0x004FC680); }
	int LoadMSH(zCFileBIN &file, int tryToShare) { XCALL(0x004FE150); }
};

class zCOption
{
public:
	int Parm(zSTRING &parmname) { XCALL(0x005E4840); }
};

class zFILE
{
public:

};

class zCObjectFactory
{
public:
	zFILE *CreateZFile(zSTRING &filename) { XCALL(0x00535A80); }
};

class zCWorld
{
public:
	int LoadBspTree(zFILE &file, int skipThisChunk) { XCALL(0x005A46C0); }
	void SaveBspTree(zFILE &file) { XCALL(0x005A4520); }
	void Unarchive(zCArchiver &arc) { XCALL(0x007073A2); }
};

class oCWorld : public zCWorld
{
public:

};