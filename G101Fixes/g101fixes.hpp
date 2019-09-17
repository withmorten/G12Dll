class hCMesh : public zCMesh
{
public:
	int LoadMSH(zCFileBIN &file);
};

class hCWorld : public zCWorld
{
public:
	void GenerateStaticWorldLighting(int &lightmode, zTBBox3D *updateBBox3D);
};

class hCGame : public oCGame
{
public:
	void LoadWorld(int slotID, zSTRING &levelpath);
	void LoadMatlib();
	void LoadPML(zSTRING &matfilename);
};