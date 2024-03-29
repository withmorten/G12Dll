class hCMagFrontier : public oCMagFrontier
{
public:
	void DoCheck();
	float GetDistance(zVEC3 &pos, float &dist, zVEC3 &nearestPoint);
	void Init();
};

class hCBarrier : public oCBarrier
{
public:
	void Init();
	void AddTremor(zTRenderContext &renderContext);
	void AddEarthQuake();
	void Render(zTRenderContext &rndContext);
};

class hCSkyControler_Barrier : public oCSkyControler_Barrier
{
public:
	void RenderSkyPre();
};
