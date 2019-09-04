class hMesh : public zCMesh
{
public:
	int LoadMSH(zCFileBIN &file, int tryToShare);
};

class hWorld : public oCWorld
{
public:
	void Unarchive(zCArchiver &arc);
};
