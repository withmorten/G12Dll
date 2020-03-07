zCInput_Win32 *&zinput = *(zCInput_Win32 **)0x008D1650;
zCRenderer *&zrenderer = *(zCRenderer **)0x00982F08;
zCSoundSystem *&zsound = *(zCSoundSystem **)0x0099B03C;
zCTimer &ztimer = *(zCTimer *)0x0099B3D4;
oCGame *&ogame = *(oCGame **)0x00AB0884;
zCParser &parser = *(zCParser *)0x00AB40C0;
zCParser *&cur_parser = *(zCParser **)0x00AB628C;
zERROR &zerr = *(zERROR *)0x008CDCD0;
zCView *&screen = *(zCView **)0x00AB6468;

zMAT4 &zMAT4::s_identity = *(zMAT4 *)0x008D45E8;

zCCamera *&zCCamera::activeCam = *(zCCamera **)0x008D7F94;

bool32 &zCSkyControler::s_skyEffectsEnabled = *(bool32 *)0x008A5DB0;

oCNpc *&oCNpc::player = *(oCNpc **)0x00AB2684;
bool32 &oCNpc::godmode = *(bool32 *)0x00AB2660;

bool32 &showTarget = *(bool32 *)0x008CE7E8;

#define SIN_TABLE_SIZE 6284
float (&SinTable)[SIN_TABLE_SIZE] = *(float (*)[SIN_TABLE_SIZE])0x00833628;

zCClassDef &zCSkyControler_Outdoor::classDef = *(zCClassDef *)0x0099ACD8;
zCClassDef &oCNpc::classDef = *(zCClassDef *)0x00AB1E20;
zCClassDef &oCMOB::classDef = *(zCClassDef *)0x00AB1A10;
zCClassDef &oCMobInter::classDef = *(zCClassDef *)0x00AB19A0;
zCClassDef &oCItem::classDef = *(zCClassDef *)0x00AB1168;
zCClassDef &oCVisualFX::classDef = *(zCClassDef *)0x008CE658;
zCClassDef &zCModel::classDef = *(zCClassDef *)0x008D8920;
zCClassDef &zCParticleFX::classDef = *(zCClassDef *)0x008D91A8;

zCOLOR GFX_WHITE(255, 255, 255, 255);
zCOLOR GFX_BLUE(0, 0, 255, 255);
zCOLOR GFX_LBLUE(173, 216, 230, 255);

zCCollisionObjectDef &zCCollObjectBoxPassThrough::s_oCollObjClass = *(zCCollisionObjectDef *)0x008D832C;
zCCollisionObjectDef &zCCollObjectProjectile::s_oCollObjClass = *(zCCollisionObjectDef *)0x008D8368;

WRAPPER void GetNearestPointFromLineSegment2D(zVEC2 &a, zVEC2 &b, zVEC2 &c, zVEC2 &nearest) { EAXJMP(0x00474080); }
WRAPPER void DrawLineSegment2D(zVEC2 &a, zVEC2 &b, float z, zCOLOR color) { EAXJMP(0x00474140); }

WRAPPER void ApplyDamages(unsigned int damageTypes, int *damage, int &damageTotal) { EAXJMP(0x0065E5A0); }

int zRand(int max)
{
	return rand() % max;
}

float zSinApprox(float a)
{
	const int lookup = (int)(1000.0f * a);
	const float sinValue = SinTable[(lookup + (SIN_TABLE_SIZE / 2) + SIN_TABLE_SIZE * 1000) % SIN_TABLE_SIZE];

	return sinValue;
}

float zSinusEase(float t)
{
	const float _PI = 3.14159265358979323846f;

	return (float)((zSinApprox(t * _PI - _PI / 2) + 1) / 2);
}

zMAT4 Alg_Identity3D()
{
	return zMAT4(
		zVEC4(1.0, 0.0, 0.0, 0.0),
		zVEC4(0.0, 1.0, 0.0, 0.0),
		zVEC4(0.0, 0.0, 1.0, 0.0),
		zVEC4(0.0, 0.0, 0.0, 1.0)
	);
}

zVEC3 operator*(zMAT4 &m, zVEC3 &v)
{
	return zVEC3
	(
		m[0][VX] * v[VX] + m[0][VY] * v[VY] + m[0][VZ] * v[VZ] + m[0][VW],
		m[1][VX] * v[VX] + m[1][VY] * v[VY] + m[1][VZ] * v[VZ] + m[1][VW],
		m[2][VX] * v[VX] + m[2][VY] * v[VY] + m[2][VZ] * v[VZ] + m[2][VW]
	);
}

bool32 zCModel::IsAniActive(zSTRING &aniName)
{
	return this->IsAniActive(this->GetAniFromAniID(this->GetAniIDFromAniName(aniName)));
}

zSTRING oCSpell::GetSpellInstanceName(int _spellID)
{
	zSTRING failString("default");
	zSTRING res;

	zCPar_Symbol *sym = parser.GetSymbol("spellFXInstanceNames");
	if (!sym) return failString;

	sym->GetValue(res, _spellID);

	return res;
}

template<class T> T *zDYNAMIC_CAST(zCObject *pObject)
{
	return pObject && zCObject::CheckInheritance(T::GetStaticClassDef(), pObject->_GetClassDef()) ? (T *)pObject : NULL;
}

template<class T> T *zSTATIC_CAST(zCObject *pObject)
{
	return (T *)pObject;
}
