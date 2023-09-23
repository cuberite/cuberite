#include "Globals.h"

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler.h"

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_8.h"
#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_9_0.h"





template <size_t NumDoubles> constexpr bool cWSSAnvilHandler::LoadDoublesList(std::array<double, NumDoubles> & a_Doubles, const cParsedNBT & a_NBT, int a_TagIdx)
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_List) || (a_NBT.GetChildrenType(a_TagIdx) != TAG_Double))
	{
		return false;
	}
	size_t idx = 0;
	for (int Tag = a_NBT.GetFirstChild(a_TagIdx); (Tag > 0) && (idx < NumDoubles); Tag = a_NBT.GetNextSibling(Tag), ++idx)
	{
		a_Doubles[idx] = a_NBT.GetDouble(Tag);
	}  // for Tag - PosTag[]
	return (idx == NumDoubles);  // Did we read enough doubles?
}





template <size_t NumFloats> constexpr bool cWSSAnvilHandler::LoadFloatsList(std::array<float, NumFloats> & a_Floats, const cParsedNBT & a_NBT, int a_TagIdx)
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_List) || (a_NBT.GetChildrenType(a_TagIdx) != TAG_Double))
	{
		return false;
	}
	size_t idx = 0;
	for (int Tag = a_NBT.GetFirstChild(a_TagIdx); (Tag > 0) && (idx < NumFloats); Tag = a_NBT.GetNextSibling(Tag), ++idx)
	{
		a_Floats[idx] = a_NBT.GetFloat(Tag);
	}  // for Tag - PosTag[]
	return (idx == NumFloats);  // Did we read enough floats?
}





const cWSSAnvilHandler & cWSSAnvilHandler::From(eDataVersion a_DataVersion)
{
	const static cWSSAnvilHandler_1_8  Handler_1_8;
	const static cWSSAnvilHandler_1_9_0 Handler_1_9_0;
	const static cWSSAnvilHandler_1_9_1 Handler_1_9_1;
	const static cWSSAnvilHandler_1_9_2 Handler_1_9_2;
	const static cWSSAnvilHandler_1_9_3 Handler_1_9_3;
	const static cWSSAnvilHandler_1_9_4 Handler_1_9_4;

	switch (a_DataVersion)
	{
		case eDataVersion::v1_8:   return Handler_1_8;
		case eDataVersion::v1_9_0: return Handler_1_9_0;
		case eDataVersion::v1_9_1: return Handler_1_9_1;
		case eDataVersion::v1_9_2: return Handler_1_9_2;
		case eDataVersion::v1_9_3: return Handler_1_9_3;
		case eDataVersion::v1_9_4: return Handler_1_9_4;
		default:
		{
			ASSERT(!"Unsupported data version");
			return Handler_1_8;
		}
	}
}
