
#pragma once

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_8.h"

class cWSSAnvilHandler_1_9_0 : public cWSSAnvilHandler_1_8
{
	using Super = cWSSAnvilHandler_1_8;

public:
	using Super::Super;
	~cWSSAnvilHandler_1_9_0() override = default;
	static const eDataVersion m_DataVersion = eDataVersion::v1_9_0;

protected:
	void LoadBoat            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override;
	void LoadEndCrystal      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override;
};




class cWSSAnvilHandler_1_9_1 : public cWSSAnvilHandler_1_9_0
{
	using Super = cWSSAnvilHandler_1_9_0;

public:
	using Super::Super;
	eDataVersion m_DataVersion = eDataVersion::v1_9_1;

	~cWSSAnvilHandler_1_9_1() override = default;
};





class cWSSAnvilHandler_1_9_2 : public cWSSAnvilHandler_1_9_1
{
	using Super = cWSSAnvilHandler_1_9_1;

public:
	using Super::Super;
	static const eDataVersion m_DataVersion = eDataVersion::v1_9_2;

	~cWSSAnvilHandler_1_9_2() override = default;
};





class cWSSAnvilHandler_1_9_3 : public cWSSAnvilHandler_1_9_2
{
	using Super = cWSSAnvilHandler_1_9_2;

public:
	using Super::Super;
	static const eDataVersion m_DataVersion = eDataVersion::v1_9_3;

	~cWSSAnvilHandler_1_9_3() override = default;
};





class cWSSAnvilHandler_1_9_4 : public cWSSAnvilHandler_1_9_3
{
	using Super = cWSSAnvilHandler_1_9_3;

public:
	using Super::Super;
	static const eDataVersion m_DataVersion = eDataVersion::v1_9_4;

	~cWSSAnvilHandler_1_9_4() override = default;
};
