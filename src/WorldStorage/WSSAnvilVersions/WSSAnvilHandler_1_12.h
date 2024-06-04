#pragma once

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_11.h"

class cWSSAnvilHandler_1_12_0 : public cWSSAnvilHandler_1_11_2
{
	using Super = cWSSAnvilHandler_1_11_2;
	static const eDataVersion m_DataVersion = eDataVersion::v1_12_0;

public:

	using Super::Super;
	~cWSSAnvilHandler_1_12_0() override = default;

	virtual void LoadEvoker     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadEvoker"); }
	virtual void LoadIllusioner (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadIllusioner"); }
	virtual void LoadParrot     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadParrot"); }
};
/**
The burger website lists a bunch of entities that should have changed, but in
there arent any changes present AFAIK.
*/




class cWSSAnvilHandler_1_12_1 : public cWSSAnvilHandler_1_12_0
{
	using Super = cWSSAnvilHandler_1_12_0;
	static const eDataVersion m_DataVersion = eDataVersion::v1_12_1;

public:

	using Super::Super;
	~cWSSAnvilHandler_1_12_1() override = default;
};





class cWSSAnvilHandler_1_12_2 : public cWSSAnvilHandler_1_12_1
{
	using Super = cWSSAnvilHandler_1_12_1;
	static const eDataVersion m_DataVersion = eDataVersion::v1_12_2;

public:

	using Super::Super;
	~cWSSAnvilHandler_1_12_2() override = default;
};
