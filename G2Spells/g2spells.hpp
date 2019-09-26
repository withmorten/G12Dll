#pragma warning(disable : 4005) // disable macro redefinition warning
// SPL_* should probably be read from INI, just using hardcoded values for now - same as the game does it :)
#define SPL_TELEPORT5 13 // Sumpflager
#define SPL_TELEPORT1 14 // Feuermagier
#define SPL_TELEPORT3 15 // Dämonenbeschwörer
#define SPL_TELEPORT2 16 // Wassermagier
#define SPL_TELEPORT4 17 // Orkisch
#define SPL_LIGHT 18
#define SPL_FIREBOLT 19
#define SPL_THUNDERBOLT 20
#define SPL_CHAINLIGHTNING 24
#define SPL_WINDFIST 26
#define SPL_SLEEP 27
#define SPL_FIREBALL 30
#define SPL_SUMMONSKELETON 31
#define SPL_FEAR 32
#define SPL_ICECUBE 33
#define SPL_THUNDERBALL 34
#define SPL_SUMMONGOLEM 35
#define SPL_DESTROYUNDEAD 36
#define SPL_PYROKINESIS 37
#define SPL_ICEWAVE 39
#define SPL_SUMMONDEMON 40
#define SPL_FIRERAIN 42
#define SPL_BREATHOFDEATH 43
#define SPL_MASSDEATH 44
#define SPL_ARMYOFDARKNESS 45
#define SPL_SHRINK 46
#define SPL_TRF_MEATBUG 47
#define SPL_TRF_SCAVENGER 48
#define SPL_TRF_MOLERAT 49
#define SPL_TRF_CRAWLER 50
#define SPL_TRF_WOLF 51
#define SPL_TRF_WARAN 52
#define SPL_TRF_SNAPPER 53
#define SPL_TRF_ORCDOG 54
#define SPL_TRF_BLOODFLY 55
#define SPL_TRF_LURKER 56
#define SPL_TRF_SHADOWBEAST 57
#define SPL_TRANSFORM 58
#define SPL_CHARM 59
#define SPL_NEW1 60
#define SPL_CONTROL 64
#define SPL_TELEKINESIS 65
#define SPL_TELEKINESIS2 66
#define SPL_BERZERK 67
#define SPL_HEAL 68
#define SPL_FIRESTORM  69
#define SPL_STORMFIST 72
#pragma warning(default : 4005) // restore macro redefinition warning

class hCNpc : public oCNpc
{
public:
	void OnDamage_Events(oSDamageDescriptor &descDamage);
};

class hCSpell : public oCSpell
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
	oCVisualFX *CreateEffect();
	void InitValues(int _spellID);
	void InitByScript(const zSTRING &name) { this->oCSpell::InitByScript(name); this->spellLevel = 0; }
};

class hCVisualFX : public oCVisualFX
{
public:
	virtual void SetCollisionEnabled(bool en);

	void _SetCollisionEnabled(bool en) { this->hCVisualFX::SetCollisionEnabled(en); }

	void InitValues() { this->oCVisualFX::InitValues(); this->level = 0; }
	void InitInvestFX() { if (this->level >= 1) this->oCVisualFX::InitInvestFX(); }
};

#define NUM_SCANNER_VOBS 3

class hCFXScanner
{
public:
	bool enabled;
	byte alpha;
	zCVob *initVob;
	zCVob *scannerVobs[NUM_SCANNER_VOBS];

public:
	hCFXScanner *Constructor();
	void Destructor();

	bool Initialized();
	void SetAlpha(byte a) { this->alpha = a; }
	void Enable(zCVob *orgVob);
	void Disable();
	bool Enabled() { return this->enabled; }
	void Run();
};

class hCVisFX_Lightning : public hCVisualFX
{
public:
	static zCClassDef &hCVisFX_Lightning::classDef; // oCVisFX_MultiTarget::classDef

	int phase;
	// float lightRange;

	bool showScanner;
	bool investedNext;
	bool castOnSelf;

	// zCModelNodeInst *targetNode;

	zCArray<zCVob *> burnVobs;
	zCArray<zCModelNodeInst *> burnNodes;
	zCArray<zCVob *> decalVobs;
	zCArray<oCVisualFX *> electricFX;

	hCFXScanner scanner;

public:
	static hCVisFX_Lightning *_CreateNewInstance() { XCALL(0x0049F750); } // oCVisFX_MultiTarget::_CreateNewInstance()

	hCVisFX_Lightning *Constructor();
	void Destructor();

	virtual void OnTick();
	virtual void Open();
	virtual void Init(zCArray<zCVob *> &trajectoryVobs);
	virtual void InvestNext();
	virtual void Cast(bool killAfterDone);
	virtual void Stop(bool killAfterDone);
	
	// virtual overrides
	void _OnTick() { this->hCVisFX_Lightning::OnTick(); }
	void _Open() { this->hCVisFX_Lightning::Open(); }
	void _Init_3(zCArray<zCVob *> &trajectoryVobs) { this->hCVisFX_Lightning::Init(trajectoryVobs); }
	void _InvestNext() { this->hCVisFX_Lightning::InvestNext(); }
	void _Cast(bool killAfterDone) { this->hCVisFX_Lightning::Cast(killAfterDone); }
	void _Stop(bool killAfterDone) { this->hCVisFX_Lightning::Stop(killAfterDone); }

	bool CheckDeletion();
	void UpdateBurnVobs();
	bool UpdateBurnVobsInvestNext();
	void Draw();
	void CreateScanner(zCVob *orgVob);
	void UpdateScanner() { if (this->scanner.Enabled()) this->scanner.Run(); }
	void DeleteScanner() { if (this->scanner.Initialized()) this->scanner.Disable(); }
};
