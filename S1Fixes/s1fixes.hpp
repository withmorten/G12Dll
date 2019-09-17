class hCMesh : public zCMesh
{
public:
	int LoadMSH(zCFileBIN &file, int tryToShare);
};

class hCWorld : public oCWorld
{
public:
	void Unarchive(zCArchiver &arc);
};
