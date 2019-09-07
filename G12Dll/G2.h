zCInput_Win32 *&zinput = *(zCInput_Win32 **)0x008D1650;
zCRnd_D3D *&zrenderer = *(zCRnd_D3D **)0x00982F08;
zCSndSys_MSS *&zsound = *(zCSndSys_MSS **)0x0099B03C;
zCTimer &ztimer = *(zCTimer *)0x0099B3D4;
oCGame *&ogame = *(oCGame **)0x00AB0884;

zMAT4 &zMAT4::s_identity = *(zMAT4 *)0x008D45E8;

zCCamera *&zCCamera::activeCam = *(zCCamera **)0x008D7F94;

bool &zCSkyControler::s_skyEffectsEnabled = *(bool *)0x008A5DB0;

oCNpc *&oCNpc::player = *(oCNpc **)0x00AB2684;
bool &oCNpc::godmode = *(bool *)0x00AB2660;

zCClassDef &zCSkyControler_Outdoor::classDef = *(zCClassDef *)0x0099ACD8;
zCClassDef &oCNpc::classDef = *(zCClassDef *)0x00AB1E20;
zCClassDef &oCMOB::classDef = *(zCClassDef *)0x00AB1A10;
zCClassDef &oCMobInter::classDef = *(zCClassDef *)0x00AB19A0;
zCClassDef &oCItem::classDef = *(zCClassDef *)0x00AB1168;

zCOLOR &GFX_BLUE = *(zCOLOR *)0x00AB3E24;
zCOLOR &GFX_LBLUE = *(zCOLOR *)0x00AB3E48;

zCCollisionObjectDef &zCCollObjectBoxPassThrough::s_oCollObjClass = *(zCCollisionObjectDef *)0x008D832C;
zCCollisionObjectDef &zCCollObjectProjectile::s_oCollObjClass = *(zCCollisionObjectDef *)0x008D8368;

WRAPPER int _rand(void) { EAXJMP(0x007D2F98); }

WRAPPER void GetNearestPointFromLineSegment2D(zVEC2 &a, zVEC2 &b, zVEC2 &c, zVEC2 &nearest) { EAXJMP(0x00474080); }
WRAPPER void DrawLineSegment2D(zVEC2 &a, zVEC2 &b, float z, zCOLOR color) { EAXJMP(0x00474140); }

zVEC3 operator +(zVEC3 &v, float f)
{
	zVEC3 n;

	n.n[0] = v.n[0] + f;
	n.n[1] = v.n[1] + f;
	n.n[2] = v.n[2] + f;

	return n;
}

zVEC3 operator +(zVEC3 &a, zVEC3 &b)
{
	zVEC3 v;

	v.n[0] = a.n[0] + b.n[0];
	v.n[1] = a.n[1] + b.n[1];
	v.n[2] = a.n[2] + b.n[2];

	return v;
}

zVEC3 operator -(zVEC3 &v)
{
	zVEC3 n;

	n.n[0] = -v.n[0];
	n.n[1] = -v.n[1];
	n.n[2] = -v.n[2];

	return n;
}

zVEC3 operator -(zVEC3 &v, float f)
{
	zVEC3 n;

	n.n[0] = v.n[0] - f;
	n.n[1] = v.n[1] - f;
	n.n[2] = v.n[2] - f;

	return n;
}

zVEC3 operator -(zVEC3 &a, zVEC3 &b)
{
	zVEC3 v;

	v.n[0] = a.n[0] - b.n[0];
	v.n[1] = a.n[1] - b.n[1];
	v.n[2] = a.n[2] - b.n[2];

	return v;
}

zVEC3 operator *(zVEC3 &v, float f)
{
	zVEC3 n;

	n.n[0] = v.n[0] * f;
	n.n[1] = v.n[1] * f;
	n.n[2] = v.n[2] * f;

	return n;
}

float operator *(zVEC3 &a, zVEC3 &b)
{
	return a.n[0] * b.n[0] + a.n[1] * b.n[1] + a.n[2] * b.n[2];
}


zVEC3 operator ^(zVEC3 &a, zVEC3 &b)
{
	zVEC3 v;

	v.n[0] = a.n[1] * b.n[2] - a.n[2] * b.n[1];
	v.n[1] = a.n[2] * b.n[0] - a.n[0] * b.n[2];
	v.n[2] = a.n[0] * b.n[1] - a.n[1] * b.n[0];

	return v;
}

void zCArray<zCVob *>::RemoveIndex(int index)
{
	if (this->numInArray > 0)
	{
		this->numInArray--;

		if (index != this->numInArray)
		{
			this->array[index] = this->array[this->numInArray];
		}
	}
}

bool zCModel::IsAniActive(zSTRING &aniName)
{
	return this->IsAniActive(this->GetAniFromAniID(this->GetAniIDFromAniName(aniName)));
}
