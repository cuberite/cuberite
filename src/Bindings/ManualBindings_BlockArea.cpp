// ManualBindings_BlockArea.cpp

// Implements the manual bindings for functions in the cBlockArea class

#include "Globals.h"
#include "tolua++/include/tolua++.h"
#include "../BlockArea.h"
#include "../World.h"
#include "ManualBindings.h"
#include "LuaState.h"
#include "PluginLua.h"
#include "../WorldStorage/SchematicFileSerializer.h"





/** Template for the bindings for the DoWithXYZAt(X, Y, Z) functions that need to check their coords. */
template <
	class SELF,
	class ITEM,
	bool (SELF::*DoWithFn)(Vector3i, cFunctionRef<bool(ITEM &)>),
	bool (SELF::*CoordCheckFn)(Vector3i) const
>
static int DoWithXYZ(lua_State * tolua_S)
{
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamNumber(2, 4) ||
		!L.CheckParamFunction(5) ||
		!L.CheckParamEnd(6)
	)
	{
		return 0;
	}

	// Get parameters:
	SELF * Self = nullptr;
	int BlockX = 0;
	int BlockY = 0;
	int BlockZ = 0;
	cLuaState::cRef FnRef;
	L.GetStackValues(1, Self, BlockX, BlockY, BlockZ, FnRef);
	if (Self == nullptr)
	{
		return L.ApiParamError("Invalid 'self'");
	}
	if (!FnRef.IsValid())
	{
		return L.ApiParamError("Expected a valid callback function for parameter #5");
	}
	if (!(Self->*CoordCheckFn)({ BlockX, BlockY, BlockZ }))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("The provided coordinates ({0}) are not valid"),
			Vector3i{BlockX, BlockY, BlockZ}
		));
	}

	// Call the DoWith function:
	bool res = (Self->*DoWithFn)({ BlockX, BlockY, BlockZ }, [&](ITEM & a_Item)
		{
			bool ret = false;
			L.Call(FnRef, &a_Item, cLuaState::Return, ret);
			return ret;
		}
	);

	// Push the result as the return value:
	L.Push(res);
	return 1;
}





/** Reads params that together form a Cuboid.
These can be:
	- 6 numbers (MinX, MaxX, MinY, MaxY, MinZ, MaxZ)
	- 2 Vector3-s (Min, Max)
	- cCuboid
Returns the index of the first parameter following the Cuboid spec.
Raises an Api error if the params don't specify a Cuboid.
*/
static int readCuboidOverloadParams(cLuaState & a_LuaState, int a_StartParam, cCuboid & a_Cuboid)
{
	if (a_LuaState.IsParamNumber(a_StartParam))
	{
		// Assume the 6-number version:
		if (!a_LuaState.GetStackValues(a_StartParam, a_Cuboid.p1.x, a_Cuboid.p2.x, a_Cuboid.p1.y, a_Cuboid.p2.y, a_Cuboid.p1.z, a_Cuboid.p2.z))
		{
			return a_LuaState.ApiParamError("Cannot read the bounds parameters, expected 6 numbers");
		}
		return a_StartParam + 6;
	}
	else if (a_LuaState.IsParamUserType(a_StartParam, "cCuboid"))
	{
		// Assume the cCuboid version:
		cCuboid * c;
		if (!a_LuaState.GetStackValues(a_StartParam, c))
		{
			return a_LuaState.ApiParamError("Cannot read the bounds parameter, expected a cCuboid instance");
		}
		a_Cuboid = *c;
		return a_StartParam + 1;
	}
	else
	{
		// Assume the 2-Vector3i version:
		if (!a_LuaState.GetStackValues(a_StartParam, a_Cuboid.p1, a_Cuboid.p2))
		{
			return a_LuaState.ApiParamError("Cannot read the bounds parameter, expected two Vector3i instances");
		}
		return a_StartParam + 2;
	}
}





/** Reads params that together form a Vector3i.
These can be:
	- 3 numbers (x, y, z)
	- Vector3i
Returns the index of the first parameter following the Vector3i spec.
Raises an Api error if the params don't specify a Vector3i.
*/
static int readVector3iOverloadParams(cLuaState & a_LuaState, int a_StartParam, Vector3i & a_Coords, const char * a_ParamName)
{
	if (a_LuaState.IsParamNumber(a_StartParam))
	{
		// Assume the 3-number version:
		if (!a_LuaState.GetStackValues(a_StartParam, a_Coords.x, a_Coords.y, a_Coords.z))
		{
			return a_LuaState.ApiParamError(fmt::format(FMT_STRING("Cannot read the {}, expected 3 numbers"), a_ParamName));
		}
		return a_StartParam + 3;
	}
	else
	{
		// Assume the Vector3i version:
		if (!a_LuaState.GetStackValues(a_StartParam, a_Coords))
		{
			return a_LuaState.ApiParamError(fmt::format(FMT_STRING("Cannot read the {}, expected a Vector3i instance"), a_ParamName));
		}
		return a_StartParam + 1;
	}
}





/** Binding for the cBlockArea::Create() functions. Supports two overloads and one default parameter. */
static int tolua_cBlockArea_Create(lua_State * a_LuaState)
{
	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}
	cBlockArea * self = nullptr;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read self");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	int dataTypes = cBlockArea::baBlocks | cBlockArea::baBlockEntities;
	Vector3i size;
	auto dataTypesIdx = readVector3iOverloadParams(L, 2, size, "size");
	L.GetStackValue(dataTypesIdx, dataTypes);
	if (!cBlockArea::IsValidDataTypeCombination(dataTypes))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("Invalid combination of baDataTypes specified (0x{:02x})"), dataTypes));
	}

	// Create the area:
	if ((size.x <= 0) || (size.y <= 0) || (size.z <= 0))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("Invalid sizes, must be greater than zero, got {}"), size));
	}
	ASSERT(self != nullptr);
	self->Create(size, dataTypes);
	return 0;
}





/** Bindings for the cBlockArea:FillRelCuboid() functions. Supports coord overloads and one default parameter. */
static int tolua_cBlockArea_FillRelCuboid(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	// Get the common params:
	cBlockArea * self = nullptr;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read self");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	// Check and get the overloaded params:
	cCuboid bounds;
	auto nextIdx = readCuboidOverloadParams(L, 2, bounds);
	bounds.Sort();
	if (!(self->IsValidRelCoords(bounds.p1) && self->IsValidRelCoords(bounds.p2)))
	{
		return L.ApiParamError(fmt::format(
			FMT_STRING("The bounds ({0} - {1}) are out of range ({2} - {3})"),
			bounds.p1,
			bounds.p2,
			Vector3i(0, 0, 0),
			(self->GetSize() - Vector3i{1, 1, 1})
		));
	}
	int DataTypes = cBlockArea::baBlocks | cBlockArea::baBlockEntities;
	unsigned char BlockType;
	unsigned char BlockMeta = 0, BlockLight = 0, BlockSkyLight = 0x0f;
	if (!L.GetStackValues(nextIdx, DataTypes, BlockType))
	{
		return L.ApiParamError("Cannot read the datatypes or block type params");
	}
	L.GetStackValues(nextIdx + 2, BlockMeta, BlockLight, BlockSkyLight);  // These values are optional
	if (!cBlockArea::IsValidDataTypeCombination(DataTypes))
	{
		return L.ApiParamError(fmt::format("Invalid baDataTypes combination (%d)", DataTypes));
	}

	// Do the actual Fill:
	self->FillRelCuboid(bounds, DataTypes, PaletteUpgrade::FromBlock(BlockType, BlockMeta), BlockLight, BlockSkyLight);
	return 0;
}





static int tolua_cBlockArea_GetBlockTypeMeta(lua_State * a_LuaState)
{
	// function cBlockArea::GetBlockTypeMeta()

	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read 'self'");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	Vector3i Coords;
	readVector3iOverloadParams(L, 2, Coords, "Coords");
	if (!self->IsValidCoords(Coords))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("Coords ({0}) out of range ({1} - {2})"),
			Coords, self->GetOrigin(), self->GetOrigin() + self->GetSize() - Vector3i{1, 1, 1}
		));
	}

	auto Block = self->GetBlock({Coords.x, Coords.y, Coords.z});
	auto NumericBlock = PaletteUpgrade::ToBlock(Block);
	L.Push(NumericBlock.first, NumericBlock.second);
	return 2;
}





static int tolua_cBlockArea_GetCoordRange(lua_State * a_LuaState)
{
	// function cBlockArea::GetCoordRange()
	// Returns all three sizes of the area, each minus one, so that they represent the maximum coord value
	// Exported manually because there's no direct C++ equivalent,
	// plus tolua would generate extra input params for the outputs

	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' parameter");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	L.Push(self->GetSizeX() - 1, self->GetSizeY() - 1, self->GetSizeZ() - 1);
	return 3;
}





static int tolua_cBlockArea_GetNonAirCropRelCoords(lua_State * a_LuaState)
{
	// function cBlockArea::GetNonAirCropRelCoords()
	// Exported manually because tolua would generate extra input params for the outputs

	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self = nullptr;
	unsigned char IgnoreBlockType = 0;
	if (!L.GetStackValues(1, self, IgnoreBlockType))
	{
		return L.ApiParamError("Cannot read params");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}
	if (!self->HasBlocks())
	{
		return L.ApiParamError("The area doesn't contain baBlocks datatype");
	}

	auto IngoredBlock = PaletteUpgrade::FromBlock(IgnoreBlockType, 0);

	// Calculate the crop Coords:
	int minRelX, minRelY, minRelZ, maxRelX, maxRelY, maxRelZ;
	self->GetNonAirCropRelCoords(minRelX, minRelY, minRelZ, maxRelX, maxRelY, maxRelZ, IngoredBlock.Type());

	// Push the six crop Coords:
	L.Push(minRelX, minRelY, minRelZ, maxRelX, maxRelY, maxRelZ);
	return 6;
}





static int tolua_cBlockArea_GetOrigin(lua_State * a_LuaState)
{
	// function cBlockArea::GetOrigin()
	// Returns all three Coords of the origin point
	// Exported manually because there's no direct C++ equivalent,
	// plus tolua would generate extra input params for the outputs

	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' parameter");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	// Push the three origin Coords:
	L.Push(self->GetOriginX(), self->GetOriginY(), self->GetOriginZ());
	return 3;
}





static int tolua_cBlockArea_GetRelBlockTypeMeta(lua_State * a_LuaState)
{
	// function cBlockArea::GetRelBlockTypeMeta()
	// Exported manually because tolua generates extra input params for the outputs

	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' parameter");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}
	if (!self->HasBlocks())
	{
		return L.ApiParamError("The area doesn't contain baBlocks datatype");
	}

	Vector3i Coords;
	readVector3iOverloadParams(L, 2, Coords, "Coords");
	if (!self->IsValidRelCoords(Coords))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("The coords ({0}) are out of range (max {1})"),
			Coords, (self->GetSize() - Vector3i{1, 1, 1})
		));
	}

	auto Block = self->GetRelBlock(Coords);

	auto NumericBlock = PaletteUpgrade::ToBlock(Block);

	L.Push(NumericBlock.first, NumericBlock.second);
	return 2;
}





static int tolua_cBlockArea_GetSize(lua_State * a_LuaState)
{
	// function cBlockArea::GetSize()
	// Returns all three sizes of the area
	// Exported manually because there's no direct C++ equivalent,
	// plus tolua would generate extra input params for the outputs

	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' parameter");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	L.Push(self->GetSizeX(), self->GetSizeY(), self->GetSizeZ());
	return 3;
}





static int tolua_cBlockArea_LoadFromSchematicFile(lua_State * a_LuaState)
{
	// function cBlockArea::LoadFromSchematicFile
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamSelf("cBlockArea") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}
	cBlockArea * self;
	AString fileName;
	if (!L.GetStackValues(1, self, fileName))
	{
		return L.ApiParamError("Cannot read the parameters");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	try
	{
		cSchematicFileSerializer::LoadFromSchematicFile(*self, fileName);
		L.Push(true);
	}
	catch (const std::exception & Oops)
	{
		LOGWARNING(Oops.what());
		L.LogStackTrace();
		L.Push(false);
	}
	return 1;
}





static int tolua_cBlockArea_LoadFromSchematicString(lua_State * a_LuaState)
{
	// function cBlockArea::LoadFromSchematicString
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamSelf("cBlockArea") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}
	cBlockArea * self;
	ContiguousByteBuffer data;
	if (!L.GetStackValues(1, self, data))
	{
		return L.ApiParamError("Cannot read the parameters");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	try
	{
		cSchematicFileSerializer::LoadFromSchematicString(*self, data);
		L.Push(true);
	}
	catch (const std::exception & Oops)
	{
		LOGWARNING(Oops.what());
		L.LogStackTrace();
		L.Push(false);
	}
	return 1;
}





/** Bindings for the cBlockArea:Read() functions. Supports three overloads and one default parameter. */
static int tolua_cBlockArea_Read(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamSelf("cBlockArea") ||
		!L.CheckParamUserType(2, "cWorld")
	)
	{
		return 0;
	}

	// Get the common params:
	cBlockArea * self = nullptr;
	cWorld * world = nullptr;
	if (!L.GetStackValues(1, self, world))
	{
		return L.ApiParamError("Cannot read self or world");
	}
	if (world == nullptr)
	{
		return L.ApiParamError("Invalid world instance. The world must be not nil");
	}

	// Check and get the overloaded params:
	cCuboid bounds;
	int dataTypes = cBlockArea::baBlocks | cBlockArea::baBlockEntities;
	auto dataTypesIdx = readCuboidOverloadParams(L, 3, bounds);
	L.GetStackValues(dataTypesIdx, dataTypes);
	if (!cBlockArea::IsValidDataTypeCombination(dataTypes))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("Invalid baDataTypes combination (0x{:02x})"), dataTypes));
	}

	// Check the coords:
	if (!cChunkDef::IsValidHeight(bounds.p1) || !cChunkDef::IsValidHeight(bounds.p2))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("Coordinates {0} - {1} exceed world bounds"), bounds.p1, bounds.p2));
	}

	bounds.Sort();

	// Do the actual read:
	L.Push(self->Read(*world, bounds, dataTypes));
	return 1;
}





/** Bindings for the cBlockArea:RelLine() functions. Supports two overloads and one default parameter.
Also supports "bastard overloads" (Vector3i, x, y, z), but we don't advertise those. */
static int tolua_cBlockArea_RelLine(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	// Get the common params:
	cBlockArea * self = nullptr;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read self");
	}

	// Check and get the overloaded params:
	Vector3i p1;
	auto idx = readVector3iOverloadParams(L, 2, p1, "start Coords");
	Vector3i p2;
	idx = readVector3iOverloadParams(L, idx, p2, "end Coords");
	int dataTypes = cBlockArea::baBlocks | cBlockArea::baBlockEntities;
	unsigned char BlockType;
	unsigned char BlockMeta, BlockLight, BlockSkyLight;
	L.GetStackValues(idx, dataTypes, BlockType, BlockMeta, BlockLight, BlockSkyLight);
	if (!cBlockArea::IsValidDataTypeCombination(dataTypes))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("Invalid baDataTypes combination (0x{:02x})"), dataTypes));
	}
	if ((self->GetDataTypes() & dataTypes) != dataTypes)
	{
		return L.ApiParamError(fmt::format(
			FMT_STRING("Requested datatypes not present in the cBlockArea. Got only 0x{:02x}, requested 0x{:02x}"),
			self->GetDataTypes(), dataTypes
		));
	}

	// Draw the line:
	self->RelLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, dataTypes, PaletteUpgrade::FromBlock(BlockType, BlockMeta), BlockLight, BlockSkyLight);
	return 0;
}





static int tolua_cBlockArea_SaveToSchematicFile(lua_State * a_LuaState)
{
	// function cBlockArea::SaveToSchematicFile
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamSelf("cBlockArea") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}
	cBlockArea * self;
	AString fileName;
	if (!L.GetStackValues(1, self, fileName))
	{
		return L.ApiParamError("Cannot read the parameters");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	try
	{
		cSchematicFileSerializer::SaveToSchematicFile(*self, fileName);
		L.Push(true);
	}
	catch (const std::exception & Oops)
	{
		LOGWARNING(Oops.what());
		L.LogStackTrace();
		L.Push(false);
	}
	return 1;
}





static int tolua_cBlockArea_SaveToSchematicString(lua_State * a_LuaState)
{
	// function cBlockArea::SaveToSchematicString
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamSelf("cBlockArea") ||
		!L.CheckParamEnd(2)
	)
	{
		return 0;
	}
	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' parameter");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	try
	{
		L.Push(cSchematicFileSerializer::SaveToSchematicString(*self).GetView());
		return 1;
	}
	catch (const std::exception & Oops)
	{
		LOGWARNING(Oops.what());
		L.LogStackTrace();
		return 0;
	}
}





/** Bindings for the cBlockArea:Write() functions. Supports two overloads and one default parameter. */
static int tolua_cBlockArea_Write(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamSelf("cBlockArea") ||
		!L.CheckParamUserType(2, "cWorld")
	)
	{
		return 0;
	}

	// Get the common params:
	cBlockArea * self = nullptr;
	cWorld * world = nullptr;
	if (!L.GetStackValues(1, self, world))
	{
		return L.ApiParamError("Cannot read self or world");
	}
	if (world == nullptr)
	{
		return L.ApiParamError("Invalid world instance. The world must be not nil");
	}

	// Check and get the overloaded params:
	Vector3i Coords;
	int dataTypes = 0;
	auto dataTypesIdx = readVector3iOverloadParams(L, 3, Coords, "Coords");
	auto HasDataTypes = L.GetStackValues(dataTypesIdx, dataTypes);

	// Check the dataType parameter validity:
	if (HasDataTypes)
	{
		if (!cBlockArea::IsValidDataTypeCombination(dataTypes))
		{
			return L.ApiParamError(fmt::format(FMT_STRING("Invalid datatype combination (0x{:02x})"), dataTypes));
		}
		if ((self->GetDataTypes() & dataTypes) != dataTypes)
		{
			return L.ApiParamError(fmt::format(
				FMT_STRING("Requesting datatypes not present in the cBlockArea. Got only 0x{:02x}, requested 0x{:02x}"),
				self->GetDataTypes(), dataTypes
			));
		}
	}

	// Check and adjust the coord params:
	// TODO: Should we report this as a failure? Because the result is definitely not what the plugin assumed
	//   ... Or should we silently clone-crop-write the cBlockArea so that the API call does what would be naturally expected?
	//   ... Or should we change the cBlockArea::Write() to allow out-of-range Y Coords and do the cropping there?
	//   ... NOTE: We already support auto-crop in cBlockArea::Merge() itself
	if (Coords.y < 0)
	{
		LOGWARNING("cBlockArea:Write(): MinBlockY less than zero, adjusting to zero");
		L.LogStackTrace();
		Coords.y = 0;
	}
	else if (Coords.y > cChunkDef::Height - self->GetSizeY())
	{
		LOGWARNING("cBlockArea:Write(): MinBlockY + m_SizeY more than chunk height, adjusting to chunk height");
		L.LogStackTrace();
		Coords.y = cChunkDef::Height - self->GetSizeY();
	}

	// Do the actual write:
	if (HasDataTypes)
	{
		L.Push(self->Write(*world, Coords, dataTypes));
	}
	else
	{
		L.Push(self->Write(*world, Coords));
	}
	return 1;
}





/** Templated bindings for the GetBlock___() functions.
DataType is either BLOCKTYPE or NIBBLETYPE.
DataTypeFlag is the ba___ constant used for the datatype being queried.
Fn is the getter function.
Also supports the Vector3i overloads (TODO: document these (?)). */
template <
	typename DataType,
	int DataTypeFlag,
	DataType (cBlockArea::*Fn)(Vector3i) const
>
static int GetBlock(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	// Read the common params:
	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' param");
	}

	// Check the datatype's presence:
	if ((self->GetDataTypes() & DataTypeFlag) == 0)
	{
		return L.ApiParamError(fmt::format(FMT_STRING("The area doesn't contain the datatype (0x{:02x})"), DataTypeFlag));
	}

	// Read the overloaded params:
	Vector3i Coords;
	readVector3iOverloadParams(L, 2, Coords, "Coords");
	if (!self->IsValidCoords(Coords))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("The coords ({0}) are out of range ({1} - {2})"),
			Coords, self->GetOrigin(), self->GetOrigin() + self->GetSize() - Vector3i{1, 1, 1}
		));
	}

	// Get the block info:
	L.Push((self->*Fn)(Coords));
	return 1;
}





/** Templated bindings for the GetRelBlock___() functions.
DataType is either BLOCKTYPE or NIBBLETYPE.
DataTypeFlag is the ba___ constant used for the datatype being queried.
Fn is the getter function.
Also supports the Vector3i overloads (TODO: document these (?)). */
template <
	typename DataType,
	int DataTypeFlag,
	DataType (cBlockArea::*Fn)(Vector3i) const
>
static int GetRelBlock(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	// Read the common params:
	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' param");
	}

	// Check the datatype's presence:
	if ((self->GetDataTypes() & DataTypeFlag) == 0)
	{
		return L.ApiParamError(fmt::format(FMT_STRING("The area doesn't contain the datatype (0x{:02x})"), DataTypeFlag));
	}

	// Read the overloaded params:
	Vector3i Coords;
	readVector3iOverloadParams(L, 2, Coords, "Coords");
	if (!self->IsValidRelCoords(Coords))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("The coords ({0}) are out of range ({1})"),
			Coords, (self->GetSize() - Vector3i(1, 1, 1))
		));
	}

	// Get the block info:
	L.Push((self->*Fn)(Coords));
	return 1;
}





/** Templated bindings for the SetBlock___() functions.
Also supports the Vector3i overloads (TODO: document these (?)). */
template <
	typename DataType,
	int DataTypeFlag,
	void (cBlockArea::*Fn)(Vector3i, DataType)
>
static int SetBlock(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	// Read the common params:
	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' param");
	}

	// Check the datatype's presence:
	if ((self->GetDataTypes() & DataTypeFlag) == 0)
	{
		return L.ApiParamError("The area doesn't contain the datatype baBlocks");
	}

	// Read the overloaded params:
	Vector3i Coords;
	auto idx = readVector3iOverloadParams(L, 2, Coords, "Coords");
	if (!self->IsValidCoords(Coords))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("The coords ({0}) are out of range ({1} - {2})"),
			Coords, self->GetOrigin(), self->GetOrigin() + self->GetSize() - Vector3i{1, 1, 1}
		));
	}
	unsigned char Data;
	L.GetStackValues(idx, Data);

	(self->*Fn)(Coords, Data);
	return 0;
}





/** Templated bindings for the SetRelBlock___() functions.
DataType is either BLOCKTYPE or NIBBLETYPE.
DataTypeFlag is the ba___ constant used for the datatypebeing manipulated.
Fn is the setter function.
Also supports the Vector3i overloads (TODO: document these (?)). */
template <
	typename DataType,
	int DataTypeFlag,
	void (cBlockArea::*Fn)(Vector3i, DataType)
>
static int SetRelBlock(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	// Read the common params:
	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' param");
	}

	// Check the datatype's presence:
	if ((self->GetDataTypes() & DataTypeFlag) == 0)
	{
		return L.ApiParamError("The area doesn't contain the datatype baBlocks");
	}

	// Read the overloaded params:
	Vector3i Coords;
	auto idx = readVector3iOverloadParams(L, 2, Coords, "Coords");
	if (!self->IsValidRelCoords(Coords))
	{
		return L.ApiParamError(fmt::format(
			FMT_STRING("The coords ({0}) are out of range ({1})"),
			Coords, (self->GetSize() - Vector3i(1, 1, 1))
		));
	}
	unsigned char Data;
	L.GetStackValues(idx, Data);

	(self->*Fn)(Coords, Data);

	return 0;
}





static int tolua_cBlockArea_SetBlockTypeMeta(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	// Read the common params:
	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' param");
	}

	// Check if block types and metas are present:
	if (!self->HasBlocks())
	{
		return L.ApiParamError("The area doesn't contain the datatype baBlocks");
	}

	// Read the overloaded params:
	Vector3i Coords;
	auto idx = readVector3iOverloadParams(L, 2, Coords, "Coords");
	if (!self->IsValidCoords(Coords))
	{
		return L.ApiParamError(fmt::format(FMT_STRING("The coords ({0}) are out of range ({1} - {2})"),
			Coords, self->GetOrigin(), self->GetOrigin() + self->GetSize() - Vector3i{1, 1, 1}
		));
	}

	unsigned char Block;
	unsigned char Meta;
	if (!L.GetStackValues(idx, Block, Meta))
	{
		return L.ApiParamError("Bad number for block type or meta type");
	}

	// Set block type and meta:
	self->SetBlock(Coords, PaletteUpgrade::FromBlock(Block, Meta));
	return 0;
}





static int tolua_cBlockArea_SetRelBlockTypeMeta(lua_State * a_LuaState)
{
	// Check the common params:
	cLuaState L(a_LuaState);
	if (!L.CheckParamSelf("cBlockArea"))
	{
		return 0;
	}

	// Read the common params:
	cBlockArea * self;
	if (!L.GetStackValues(1, self))
	{
		return L.ApiParamError("Cannot read the 'self' param");
	}

	// Check if block types and metas are present:
	if (!self->HasBlocks())
	{
		return L.ApiParamError(fmt::format("The area doesn't contain the baBlocks (0x%02x)", self->GetDataTypes()));
	}

	// Read the overloaded params:
	Vector3i Coords;
	auto idx = readVector3iOverloadParams(L, 2, Coords, "Coords");
	if (!self->IsValidRelCoords(Coords))
	{
		return L.ApiParamError(fmt::format(
			FMT_STRING("The coords ({0}) are out of range ({1})"),
			Coords, (self->GetSize() - Vector3i(1, 1, 1))
		));
	}

	unsigned char Block;
	unsigned char Meta;
	if (!L.GetStackValues(idx, Block, Meta))
	{
		return L.ApiParamError("Bad number for block type or meta type");
	}

	// Set block type and meta:
	self->SetRelBlock(Coords, PaletteUpgrade::FromBlock(Block, Meta));
	return 0;
}





void cManualBindings::BindBlockArea(lua_State * a_LuaState)
{
	tolua_beginmodule(a_LuaState, nullptr);
		tolua_beginmodule(a_LuaState, "cBlockArea");
			tolua_function(a_LuaState, "Create",                  tolua_cBlockArea_Create);
			tolua_function(a_LuaState, "DoWithBlockEntityAt",     DoWithXYZ<cBlockArea, cBlockEntity, &cBlockArea::DoWithBlockEntityAt,    &cBlockArea::IsValidCoords>);
			tolua_function(a_LuaState, "DoWithBlockEntityRelAt",  DoWithXYZ<cBlockArea, cBlockEntity, &cBlockArea::DoWithBlockEntityRelAt, &cBlockArea::IsValidRelCoords>);
			tolua_function(a_LuaState, "FillRelCuboid",           tolua_cBlockArea_FillRelCuboid);
			tolua_function(a_LuaState, "ForEachBlockEntity",      ForEach<  cBlockArea, cBlockEntity, &cBlockArea::ForEachBlockEntity>);
			tolua_function(a_LuaState, "GetBlockLight",           GetBlock<LIGHTTYPE, cBlockArea::baLight,  &cBlockArea::GetBlockLight>);
			tolua_function(a_LuaState, "GetBlockMeta",            GetBlock<unsigned char, cBlockArea::baBlocks, &cBlockArea::GetBlockMeta>);
			tolua_function(a_LuaState, "GetBlockSkyLight",        GetBlock<LIGHTTYPE, cBlockArea::baSkyLight, &cBlockArea::GetRelBlockSkyLight>);
			tolua_function(a_LuaState, "GetBlockType",            GetBlock<unsigned char, cBlockArea::baBlocks, &cBlockArea::GetBlockType>);
			tolua_function(a_LuaState, "GetBlock",                GetBlock<BlockState, cBlockArea::baBlocks, &cBlockArea::GetBlock>);
			tolua_function(a_LuaState, "GetBlockTypeMeta",        tolua_cBlockArea_GetBlockTypeMeta);
			tolua_function(a_LuaState, "GetCoordRange",           tolua_cBlockArea_GetCoordRange);
			tolua_function(a_LuaState, "GetNonAirCropRelCoords",  tolua_cBlockArea_GetNonAirCropRelCoords);
			tolua_function(a_LuaState, "GetOrigin",               tolua_cBlockArea_GetOrigin);
			tolua_function(a_LuaState, "GetRelBlockLight",        GetRelBlock<LIGHTTYPE, cBlockArea::baLight,    &cBlockArea::GetRelBlockLight>);
			tolua_function(a_LuaState, "GetRelBlockSkyLight",     GetRelBlock<LIGHTTYPE, cBlockArea::baSkyLight, &cBlockArea::GetRelBlockSkyLight>);
			tolua_function(a_LuaState, "GetRelBlockMeta",         GetRelBlock<unsigned char, cBlockArea::baBlocks, &cBlockArea::GetRelBlockMeta>);
			tolua_function(a_LuaState, "GetRelBlockType",         GetRelBlock<unsigned char, cBlockArea::baBlocks, &cBlockArea::GetRelBlockType>);
			tolua_function(a_LuaState, "GetRelBlockTypeMeta",     tolua_cBlockArea_GetRelBlockTypeMeta);
			tolua_function(a_LuaState, "GetSize",                 tolua_cBlockArea_GetSize);
			tolua_function(a_LuaState, "LoadFromSchematicFile",   tolua_cBlockArea_LoadFromSchematicFile);
			tolua_function(a_LuaState, "LoadFromSchematicString", tolua_cBlockArea_LoadFromSchematicString);
			tolua_function(a_LuaState, "Read",                    tolua_cBlockArea_Read);
			tolua_function(a_LuaState, "RelLine",                 tolua_cBlockArea_RelLine);
			tolua_function(a_LuaState, "SaveToSchematicFile",     tolua_cBlockArea_SaveToSchematicFile);
			tolua_function(a_LuaState, "SaveToSchematicString",   tolua_cBlockArea_SaveToSchematicString);
			tolua_function(a_LuaState, "SetBlock",                SetBlock<BlockState, cBlockArea::baBlocks, &cBlockArea::SetBlock>);
			tolua_function(a_LuaState, "SetBlockType",            SetBlock<unsigned char, cBlockArea::baBlocks, &cBlockArea::SetBlockType>);
			tolua_function(a_LuaState, "SetBlockMeta",            SetBlock<unsigned char, cBlockArea::baBlocks, &cBlockArea::SetBlockMeta>);
			tolua_function(a_LuaState, "SetBlockLight",           SetBlock<LIGHTTYPE, cBlockArea::baLight,    &cBlockArea::SetBlockLight>);
			tolua_function(a_LuaState, "SetBlockSkyLight",        SetBlock<LIGHTTYPE, cBlockArea::baSkyLight, &cBlockArea::SetBlockSkyLight>);
			tolua_function(a_LuaState, "SetBlockTypeMeta",        tolua_cBlockArea_SetBlockTypeMeta);
			tolua_function(a_LuaState, "SetRelBlockType",         SetRelBlock<unsigned char, cBlockArea::baBlocks, &cBlockArea::SetRelBlockType>);
			tolua_function(a_LuaState, "SetRelBlockMeta",         SetRelBlock<unsigned char, cBlockArea::baBlocks, &cBlockArea::SetRelBlockMeta>);
			tolua_function(a_LuaState, "SetRelBlockLight",        SetRelBlock<LIGHTTYPE, cBlockArea::baLight,    &cBlockArea::SetRelBlockLight>);
			tolua_function(a_LuaState, "SetRelBlockSkyLight",     SetRelBlock<LIGHTTYPE, cBlockArea::baSkyLight, &cBlockArea::SetRelBlockSkyLight>);
			tolua_function(a_LuaState, "SetRelBlockTypeMeta",     tolua_cBlockArea_SetRelBlockTypeMeta);
			tolua_function(a_LuaState, "Write",                   tolua_cBlockArea_Write);
		tolua_endmodule(a_LuaState);
	tolua_endmodule(a_LuaState);
}




