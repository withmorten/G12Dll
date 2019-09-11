class hCMagFrontier : public oCMagFrontier
{
public:
	void DoCheck();
	float GetDistance(zVEC3 &pos, float &dist, zVEC3 &nearestPoint);
};

class hCBarrier : public oCBarrier
{
public:
	void Init();
	void AddTremor(zTRenderContext &renderContext);
	void AddEarthQuake();
	bool Render(zTRenderContext &rndContext, bool fadeInOut, bool alwaysVisible);
	int AddThunderSub(myThunder *rootThunder, int startIndex, int startNexIntex, int length, int mumSplits);
	int AddThunder(int startIndex, int length, float random, int sector);
	bool RenderThunder(myThunder *thunder, zTRenderContext &rndContext);
};

class hCSkyControler_Barrier : public oCSkyControler_Barrier
{
public:
	void RenderSkyPre();
};
