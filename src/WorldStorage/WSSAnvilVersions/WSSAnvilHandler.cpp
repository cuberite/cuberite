#include "Globals.h"

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler.h"

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_8.h"
#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_9.h"
#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_10.h"
#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_11.h"
#include "WSSAnvilHandler_1_12.h"

const cWSSAnvilHandler & cWSSAnvilHandler::From(eDataVersion a_DataVersion)
{
	const static cWSSAnvilHandler_1_8    Handler_1_8;
	const static cWSSAnvilHandler_1_9_0  Handler_1_9_0;
	const static cWSSAnvilHandler_1_9_1  Handler_1_9_1;
	const static cWSSAnvilHandler_1_9_2  Handler_1_9_2;
	const static cWSSAnvilHandler_1_9_3  Handler_1_9_3;
	const static cWSSAnvilHandler_1_9_4  Handler_1_9_4;
	const static cWSSAnvilHandler_1_10_0 Handler_1_10_0;
	const static cWSSAnvilHandler_1_10_1 Handler_1_10_1;
	const static cWSSAnvilHandler_1_10_2 Handler_1_10_2;
	const static cWSSAnvilHandler_1_11_0 Handler_1_11_0;
	const static cWSSAnvilHandler_1_11_1 Handler_1_11_1;
	const static cWSSAnvilHandler_1_11_2 Handler_1_11_2;
	const static cWSSAnvilHandler_1_12_0 Handler_1_12_0;
	const static cWSSAnvilHandler_1_12_1 Handler_1_12_1;
	const static cWSSAnvilHandler_1_12_2 Handler_1_12_2;

	switch (a_DataVersion)
	{
		case eDataVersion::v1_8:    return Handler_1_8;
		case eDataVersion::v1_9_0:  return Handler_1_9_0;
		case eDataVersion::v1_9_1:  return Handler_1_9_1;
		case eDataVersion::v1_9_2:  return Handler_1_9_2;
		case eDataVersion::v1_9_3:  return Handler_1_9_3;
		case eDataVersion::v1_9_4:  return Handler_1_9_4;
		case eDataVersion::v1_10_0: return Handler_1_10_0;
		case eDataVersion::v1_10_1: return Handler_1_10_1;
		case eDataVersion::v1_10_2: return Handler_1_10_2;
		case eDataVersion::v1_11_0: return Handler_1_11_0;
		case eDataVersion::v1_11_1: return Handler_1_11_1;
		case eDataVersion::v1_11_2: return Handler_1_11_2;
		case eDataVersion::v1_12_0: return Handler_1_12_0;
		case eDataVersion::v1_12_1: return Handler_1_12_1;
		case eDataVersion::v1_12_2: return Handler_1_12_2;
		default:
		{
			ASSERT(!"Unsupported data version");
			return Handler_1_8;
		}
	}
}
