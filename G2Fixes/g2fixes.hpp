class hCNpc : public oCNpc
{
public:
	void OnDamage_Hit(oSDamageDescriptor &descDamage);
	void CreateVobList(float max_dist);
	bool IsSkeleton();
};

class hCSkyControler_Outdoor : public zCSkyControler_Outdoor
{
public:
	void ReadFogColorsFromINI();
};

class hCActiveSnd : public zCActiveSnd
{
public:
	void AutoCalcObstruction(bool immediate) { }
};
