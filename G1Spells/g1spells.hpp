class hCFXScanner : public oCFXScanner
{
public:
	bool32 Initialized();
	void SetAlpha(byte a) { this->alpha = a; }
	void Enable(zCVob *orgVob);
	void Disable();
	bool32 Enabled() { return this->enabled; }
	void Run();
};

class hCVisFX_Lightning : public oCVisualFX
{
public:
	int phase;
	float lightRange;

	bool32 showScanner;
	bool32 investedNext;
	bool32 castOnSelf;

	int unknown; // this value is unused and its name unknown

	zCModelNodeInst *targetNode;

	zCArray<zCVob *> burnVobs;
	zCArray<zCModelNodeInst *> burnNodes;
	zCArray<zCVob *> decalVobs;
	zCArray<oCVisualFX *> electricFX;

	hCFXScanner scanner;

public:
	virtual void OnTick(); // { XCALL(0x00495370); }
	virtual void Open(); // { XCALL(0x004949B0); }
	virtual void Init(zCArray<zCVob *> &trajectoryVobs); // { XCALL(0x004949E0); }
	virtual void InvestNext(); // { XCALL(0x00494BC0); }
	virtual void Cast(bool32 killAfterDone); // { XCALL(0x00494D30); }
	virtual void Stop(bool32 killAfterDone); // { XCALL(0x00494E30); }

	void _OnTick() { this->hCVisFX_Lightning::OnTick(); }
	void _Open() { this->hCVisFX_Lightning::Open(); }
	void _Init(zCArray<zCVob *> &trajectoryVobs) { this->hCVisFX_Lightning::Init(trajectoryVobs); }
	void _InvestNext() { this->hCVisFX_Lightning::InvestNext(); }
	void _Cast(bool32 killAfterDone) { this->hCVisFX_Lightning::Cast(killAfterDone); }
	void _Stop(bool32 killAfterDone) { this->hCVisFX_Lightning::Stop(killAfterDone); }

	bool32 CheckDeletion();
	void UpdateBurnVobs();
	bool32 UpdateBurnVobsInvestNext();
	void Draw();
	void CreateScanner(zCVob *orgVob);
	void UpdateScanner() { if (this->scanner.Enabled()) this->scanner.Run(); }
	void DeleteScanner() { if (this->scanner.Initialized()) this->scanner.Disable(); }
};
