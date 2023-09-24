#pragma once

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_10.h"

class cWSSAnvilHandler_1_11_0 : public cWSSAnvilHandler_1_10_2
{
	using Super = cWSSAnvilHandler_1_10_2;
	static const eDataVersion m_DataVersion = eDataVersion::v1_11_0;

public:

	using Super::Super;
	~cWSSAnvilHandler_1_11_0() override = default;


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
};
