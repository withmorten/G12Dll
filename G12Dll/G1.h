zCInput_Win32 *&zinput = *(zCInput_Win32 **)0x0086CCA0;
zCRenderer *&zrenderer = *(zCRenderer **)0x008C5ED0;
zCSoundSystem *&zsound = *(zCSoundSystem **)0x008CEE4C;
zCTimer &ztimer = *(zCTimer *)0x008CF1E8;
oCGame *&ogame = *(oCGame **)0x008DA6BC;
zCParser &parser = *(zCParser *)0x008DCE08;
zCParser *&cur_parser = *(zCParser **)0x008DDFD4;
zERROR &zerr = *(zERROR *)0x008699D8;
zCView *&screen = *(zCView **)0x008DE1BC;

zMAT4 &zMAT4::s_identity = *(zMAT4 *)0x0086F868;

zCCamera *&zCCamera::activeCam = *(zCCamera **)0x00873240;

bool &zCSkyControler::s_skyEffectsEnabled = *(bool *)0x008422A0;

oCNpc *&oCNpc::player = *(oCNpc **)0x008DBBB0;
bool &oCNpc::godmode = *(bool *)0x008DBB98;

bool &showTarget = *(bool *)0x00869EA0;

#define SIN_TABLE_SIZE 6284
float (&SinTable)[SIN_TABLE_SIZE] = *(float (*)[SIN_TABLE_SIZE])0x007D5100;

zCClassDef &zCSkyControler_Outdoor::classDef = *(zCClassDef *)0x008CEA60;
zCClassDef &oCNpc::classDef = *(zCClassDef *)0x008DB408;
zCClassDef &oCMOB::classDef = *(zCClassDef *)0x008DB0D0;
zCClassDef &oCMobInter::classDef = *(zCClassDef *)0x008DB060;
zCClassDef &oCItem::classDef = *(zCClassDef *)0x008DAA80;
zCClassDef &oCVisualFX::classDef = *(zCClassDef *)0x00869E00;
zCClassDef &zCModel::classDef = *(zCClassDef *)0x00873B10;
zCClassDef &zCParticleFX::classDef = *(zCClassDef *)0x008742F0;

zCOLOR GFX_WHITE(255, 255, 255, 255);
zCOLOR GFX_BLUE(0, 0, 255, 255);
zCOLOR GFX_LBLUE(173, 216, 230, 255);

zCCollisionObjectDef &zCCollObjectBoxPassThrough::s_oCollObjClass = *(zCCollisionObjectDef *)0x008735BC;
zCCollisionObjectDef &zCCollObjectProjectile::s_oCollObjClass = *(zCCollisionObjectDef *)0x008735D4;

WRAPPER void ApplyDamages(unsigned int damageTypes, int *damage, int &damageTotal) { EAXJMP(0x00752BC0); }

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

bool zCModel::IsAniActive(zSTRING &aniName)
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

void GetNearestPointFromLineSegment2D(zVEC2 &a, zVEC2 &b, zVEC2 &c, zVEC2 &nearest)
{
	int dotA, dotB;

	dotA = (int)((c[VX] - a[VX]) * (b[VX] - a[VX]) + (c[VY] - a[VY]) * (b[VY] - a[VY]));

	if (dotA <= 0)
	{
		nearest[VX] = a[VX];
		nearest[VY] = a[VY];

		return;
	}

	dotB = (int)((c[VX] - b[VX]) * (a[VX] - b[VX]) + (c[VY] - b[VY]) * (a[VY] - b[VY]));

	if (dotB <= 0)
	{
		nearest[VX] = b[VX];
		nearest[VY] = b[VY];

		return;
	}

	nearest[VX] = a[VX] + ((b[VX] - a[VX]) * dotA) / (dotA + dotB);
	nearest[VY] = a[VY] + ((b[VY] - a[VY]) * dotA) / (dotA + dotB);
}

void Alg_ClipAtZ0(zVEC3 &p1, zVEC3 &p2)
{
	if ((p1[VZ] >= 0) && (p2[VZ] >= 0))	return;

	if ((p1[VZ] <= 0) && (p2[VZ] <= 0))	return;

	zVEC3 p;
	float t;
	zVEC3 d = p2 - p1;

	if (d[VZ] == 0) return;

	t = -p1[VZ] / d[VZ];

	p[VX] = p1[VX] + t * d[VX];
	p[VY] = p1[VY] + t * d[VY];
	p[VZ] = 10.0F;

	if (p1[VZ] < 0)	p1 = p;
	else p2 = p;
}
