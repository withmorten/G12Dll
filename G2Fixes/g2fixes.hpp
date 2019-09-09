class hNpc : public oCNpc
{
public:
	void OnDamage_Hit(oSDamageDescriptor &descDamage);
	void OnDamage_Events(oSDamageDescriptor &descDamage);
	void CreateVobList(float max_dist);
};

class hSpell : public oCSpell
{
public:
	void StopTargetEffects(zCVob *vob);
	bool IsValidTarget(zCVob *v);
	bool IsInvestSpell();
	void DoLogicInvestEffect();
	bool CastSpecificSpell();
	void EndTimedEffect();
	void DoTimedEffect();
	bool Invest();
};

class hSkyControler_Outdoor : public zCSkyControler_Outdoor
{
public:
	void ReadFogColorsFromINI();
};

class hActiveSnd : public zCActiveSnd
{
public:
	void AutoCalcObstruction(bool immediate) { }
};

class hVisualFX : public oCVisualFX
{
public:
	void SetCollisionEnabled(bool en);
};
