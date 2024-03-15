#pragma once

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_9.h"

class cWSSAnvilHandler_1_10_0 : public cWSSAnvilHandler_1_9_4
{
	using Super = cWSSAnvilHandler_1_9_4;

public:

	using Super::Super;
	static const eDataVersion m_DataVersion = eDataVersion::v1_10_0;

	~cWSSAnvilHandler_1_10_0() override = default;

	// Todo: new metadata was added in 1.10
	void LoadAreaEffectCloud (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadAreaEffectCloud"); }
	void LoadArmorStand      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadArmorStand"); }  // Numbering changed ?!?
	void LoadPolarBear       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadPolarBear"); }
	void LoadSkeleton        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadSkeleton"); }  // Numbering changed ?!?

};





class cWSSAnvilHandler_1_10_1 : public cWSSAnvilHandler_1_10_0
{
	using Super = cWSSAnvilHandler_1_10_0;
	static const eDataVersion m_DataVersion = eDataVersion::v1_10_1;

public:
	using Super::Super;
	~cWSSAnvilHandler_1_10_1() override = default;
};


class cWSSAnvilHandler_1_10_2 : public cWSSAnvilHandler_1_10_1
{
	using Super = cWSSAnvilHandler_1_10_1;
	static const eDataVersion m_DataVersion = eDataVersion::v1_10_2;

public:

	using Super::Super;
	~cWSSAnvilHandler_1_10_2() override = default;
};
