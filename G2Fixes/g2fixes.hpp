class hNpc : public oCNpc
{
public:
	void CreateVobList(float max_dist);
};

class hSkyControler_Outdoor : public zCSkyControler_Outdoor
{
public:
	void ReadFogColorsFromINI();
};

class hActiveSnd : public zCActiveSnd
{
public:
	void AutoCalcObstruction(int immediate) { return; }
};
