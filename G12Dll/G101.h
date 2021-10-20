zCOption *&zoptions = *(zCOption **)0x00887C78;
zCObjectFactory *&zfactory = *(zCObjectFactory **)0x00898948;
zCArchiverFactory &zarcFactory = *(zCArchiverFactory *)0x00891874;

zCClassDef &zCMaterial::classDef = *(zCClassDef *)0x008976A8;

template<class T> T *zDYNAMIC_CAST(zCObject *pObject)
{
	return pObject && zCObject::CheckInheritance(T::GetStaticClassDef(), pObject->_GetClassDef()) ? (T *)pObject : NULL;
}

template<class T> T *zSTATIC_CAST(zCObject *pObject)
{
	return (T *)pObject;
}
