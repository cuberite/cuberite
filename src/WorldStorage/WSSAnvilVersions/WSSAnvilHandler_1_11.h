#pragma once

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_10.h"

/**
This is the version where mojang changed the name from any entity.
The loading doesn't care about this and loads with any name.

Because of this, we don't have to redo any of the loading functions.
*/

class cWSSAnvilHandler_1_11_0 : public cWSSAnvilHandler_1_10_2
{
	using Super = cWSSAnvilHandler_1_10_2;
	static const eDataVersion m_DataVersion = eDataVersion::v1_11_0;

public:

	using Super::Super;
	~cWSSAnvilHandler_1_11_0() override = default;

	virtual void LoadEvocationIllager (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadEvocationIllager"); }
	virtual void LoadEvocationFangs   (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadEvocationFangs"); }
	virtual void LoadHusk             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadHusk"); }
	virtual void LoadLlama            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadLlama"); }
	virtual void LoadLlamaSpit        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadLlamaSpit"); }
	virtual void LoadMule             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadMule"); }
	virtual void LoadStray            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadStray"); }
	virtual void LoadVex              (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadVex"); }
	virtual void LoadVindicator       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadVindicator"); }

};





class cWSSAnvilHandler_1_11_1 : public cWSSAnvilHandler_1_11_0
{
	using Super = cWSSAnvilHandler_1_11_0;
	static const eDataVersion m_DataVersion = eDataVersion::v1_11_1;

public:

	using Super::Super;
	~cWSSAnvilHandler_1_11_1() override = default;
};





class cWSSAnvilHandler_1_11_2 : public cWSSAnvilHandler_1_11_1
{
	using Super = cWSSAnvilHandler_1_11_1;
	static const eDataVersion m_DataVersion = eDataVersion::v1_11_2;

public:

	using Super::Super;
	~cWSSAnvilHandler_1_11_2() override = default;

	virtual void LoadArrow          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadArrow"); }
	virtual void LoadFireworkRocket (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadFireworkRocket"); }
	virtual void LoadPig            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadPig"); }
};
