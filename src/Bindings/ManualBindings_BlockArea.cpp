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
			return a_LuaState.ApiParamError("Cannot read the %s, expected 3 numbers", a_ParamName);
		}
		return a_StartParam + 3;
	}
	else
	{
		// Assume the Vector3i version:
		if (!a_LuaState.GetStackValues(a_StartParam, a_Coords))
		{
			return a_LuaState.ApiParamError("Cannot read the %s, expected a Vector3i instance", a_ParamName);
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

	int dataTypes = cBlockArea::baTypes | cBlockArea::baMetas | cBlockArea::baBlockEntities;
	Vector3i size;
	auto dataTypesIdx = readVector3iOverloadParams(L, 2, size, "size");
	L.GetStackValue(dataTypesIdx, dataTypes);
	if (!cBlockArea::IsValidDataTypeCombination(dataTypes))
	{
		return L.ApiParamError("Invalid combination of baDataTypes specified (%d)", dataTypes);
	}

	// Create the area:
	if ((size.x <= 0) || (size.y <= 0) || (size.z <= 0))
	{
		return L.FApiParamError("Invalid sizes, must be greater than zero, got {0}", size);
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
		return L.FApiParamError(
			"The bounds ({0} - {1}) are out of range ({2} - {3})",
			bounds.p1,
			bounds.p2,
			Vector3i(0, 0, 0),
			(self->GetSize() - Vector3i{1, 1, 1})
		);
	}
	int dataTypes = cBlockArea::baTypes | cBlockArea::baMetas | cBlockArea::baBlockEntities;
	BLOCKTYPE blockType;
	NIBBLETYPE blockMeta = 0, blockLight = 0, blockSkyLight = 0x0f;
	if (!L.GetStackValues(nextIdx, dataTypes, blockType))
	{
		return L.ApiParamError("Cannot read the datatypes or block type params");
	}
	L.GetStackValues(nextIdx + 2, blockMeta, blockLight, blockSkyLight);  // These values are optional
	if (!cBlockArea::IsValidDataTypeCombination(dataTypes))
	{
		return L.ApiParamError("Invalid baDataTypes combination (%d)", dataTypes);
	}

	// Do the actual Fill:
	self->FillRelCuboid(bounds, dataTypes, blockType, blockMeta, blockLight, blockSkyLight);
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

	Vector3i coords;
	readVector3iOverloadParams(L, 2, coords, "coords");
	if (!self->IsValidCoords(coords))
	{
		return L.FApiParamError("Coords ({0}) out of range ({1} - {2})",
			coords, self->GetOrigin(), self->GetOrigin() + self->GetSize() - Vector3i{1, 1, 1}
		);
	}
	BLOCKTYPE blockType;
	NIBBLETYPE blockMeta;
	self->GetBlockTypeMeta(coords.x, coords.y, coords.z, blockType, blockMeta);
	L.Push(blockType, blockMeta);
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
	BLOCKTYPE ignoreBlockType = E_BLOCK_AIR;
	if (!L.GetStackValues(1, self, ignoreBlockType))
	{
		return L.ApiParamError("Cannot read params");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}
	if (!self->HasBlockTypes())
	{
		return L.ApiParamError("The area doesn't contain baTypes datatype");
	}

	// Calculate the crop coords:
	int minRelX, minRelY, minRelZ, maxRelX, maxRelY, maxRelZ;
	self->GetNonAirCropRelCoords(minRelX, minRelY, minRelZ, maxRelX, maxRelY, maxRelZ, ignoreBlockType);

	// Push the six crop coords:
	L.Push(minRelX, minRelY, minRelZ, maxRelX, maxRelY, maxRelZ);
	return 6;
}





static int tolua_cBlockArea_GetOrigin(lua_State * a_LuaState)
{
	// function cBlockArea::GetOrigin()
	// Returns all three coords of the origin point
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

	// Push the three origin coords:
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
	if (!self->HasBlockTypes())
	{
		return L.ApiParamError("The area doesn't contain baTypes datatype");
	}
	if (!self->HasBlockMetas())
	{
		return L.ApiParamError("The area doesn't contain baMetas datatype");
	}

	Vector3i coords;
	readVector3iOverloadParams(L, 2, coords, "coords");
	if (!self->IsValidRelCoords(coords))
	{
		return L.FApiParamError("The coords ({0}) are out of range (max {1})",
			coords, (self->GetSize() - Vector3i{1, 1, 1})
		);
	}
	BLOCKTYPE blockType;
	NIBBLETYPE blockMeta;
	self->GetRelBlockTypeMeta(coords.x, coords.y, coords.z, blockType, blockMeta);
	L.Push(blockType, blockMeta);
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

	L.Push(cSchematicFileSerializer::LoadFromSchematicFile(*self, fileName));
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
	AString data;
	if (!L.GetStackValues(1, self, data))
	{
		return L.ApiParamError("Cannot read the parameters");
	}
	if (self == nullptr)
	{
		return L.ApiParamError("Invalid 'self', must not be nil");
	}

	L.Push(cSchematicFileSerializer::LoadFromSchematicString(*self, data));
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
	int dataTypes = cBlockArea::baTypes | cBlockArea::baMetas | cBlockArea::baBlockEntities;
	auto dataTypesIdx = readCuboidOverloadParams(L, 3, bounds);
	L.GetStackValues(dataTypesIdx, dataTypes);
	if (!cBlockArea::IsValidDataTypeCombination(dataTypes))
	{
		return L.ApiParamError("Invalid baDataTypes combination (%d)", dataTypes);
	}

	// Check the coords, shift if needed:
	bounds.Sort();
	if (bounds.p1.y < 0)
	{
		FLOGWARNING("cBlockArea:Read(): MinBlockY less than zero, adjusting to zero. Coords: {0} - {1}",
			bounds.p1, bounds.p2
		);
		L.LogStackTrace();
		bounds.p1.y = 0;
	}
	else if (bounds.p1.y >= cChunkDef::Height)
	{
		FLOGWARNING("cBlockArea:Read(): MinBlockY more than chunk height, adjusting to chunk height. Coords: {0} - {1}",
			bounds.p1, bounds.p2
		);
		L.LogStackTrace();
		bounds.p1.y = cChunkDef::Height - 1;
	}
	if (bounds.p2.y < 0)
	{
		FLOGWARNING("cBlockArea:Read(): MaxBlockY less than zero, adjusting to zero. Coords: {0} - {1}",
			bounds.p1, bounds.p2
		);
		L.LogStackTrace();
		bounds.p2.y = 0;
	}
	else if (bounds.p2.y > cChunkDef::Height)
	{
		FLOGWARNING("cBlockArea:Read(): MaxBlockY more than chunk height, adjusting to chunk height. Coords: {0} - {1}",
			bounds.p1, bounds.p2
		);
		L.LogStackTrace();
		bounds.p2.y = cChunkDef::Height;
	}

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
	auto idx = readVector3iOverloadParams(L, 2, p1, "start coords");
	Vector3i p2;
	idx = readVector3iOverloadParams(L, idx, p2, "end coords");
	int dataTypes = cBlockArea::baTypes | cBlockArea::baMetas | cBlockArea::baBlockEntities;
	BLOCKTYPE blockType;
	NIBBLETYPE blockMeta, blockLight, blockSkyLight;
	L.GetStackValues(idx, dataTypes, blockType, blockMeta, blockLight, blockSkyLight);
	if (!cBlockArea::IsValidDataTypeCombination(dataTypes))
	{
		return L.ApiParamError("Invalid baDataTypes combination (%d)", dataTypes);
	}
	if ((self->GetDataTypes() & dataTypes) != dataTypes)
	{
		return L.ApiParamError("Requested datatypes not present in the cBlockArea. Got only 0x%02x, requested 0x%02x",
			self->GetDataTypes(), dataTypes
		);
	}

	// Draw the line:
	self->RelLine(p1, p2, dataTypes, blockType, blockMeta, blockLight, blockSkyLight);
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

	L.Push(cSchematicFileSerializer::SaveToSchematicFile(*self, fileName));
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

	AString data;
	if (cSchematicFileSerializer::SaveToSchematicString(*self, data))
	{
		L.Push(data);
		return 1;
	}
	return 0;
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
	Vector3i coords;
	int dataTypes = 0;
	auto dataTypesIdx = readVector3iOverloadParams(L, 3, coords, "coords");
	auto HasDataTypes = L.GetStackValues(dataTypesIdx, dataTypes);

	// Check the dataType parameter validity:
	if (HasDataTypes)
	{
		if (!cBlockArea::IsValidDataTypeCombination(dataTypes))
		{
			return L.ApiParamError("Invalid datatype combination (%d)", dataTypes);
		}
		if ((self->GetDataTypes() & dataTypes) != dataTypes)
		{
			return L.ApiParamError("Requesting datatypes not present in the cBlockArea. Got only 0x%02x, requested 0x%02x",
				self->GetDataTypes(), dataTypes
			);
		}
	}

	// Check and adjust the coord params:
	// TODO: Should we report this as a failure? Because the result is definitely not what the plugin assumed
	//   ... Or should we silently clone-crop-write the cBlockArea so that the API call does what would be naturally expected?
	//   ... Or should we change the cBlockArea::Write() to allow out-of-range Y coords and do the cropping there?
	//   ... NOTE: We already support auto-crop in cBlockArea::Merge() itself
	if (coords.y < 0)
	{
		LOGWARNING("cBlockArea:Write(): MinBlockY less than zero, adjusting to zero");
		L.LogStackTrace();
		coords.y = 0;
	}
	else if (coords.y > cChunkDef::Height - self->GetSizeY())
	{
		LOGWARNING("cBlockArea:Write(): MinBlockY + m_SizeY more than chunk height, adjusting to chunk height");
		L.LogStackTrace();
		coords.y = cChunkDef::Height - self->GetSizeY();
	}

	// Do the actual write:
	if (HasDataTypes)
	{
		L.Push(self->Write(*world, coords, dataTypes));
	}
	else
	{
		L.Push(self->Write(*world, coords));
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
	DataType (cBlockArea::*Fn)(int, int, int) const
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
		return L.ApiParamError("The area doesn't contain the datatype (%d)", DataTypeFlag);
	}

	// Read the overloaded params:
	Vector3i coords;
	readVector3iOverloadParams(L, 2, coords, "coords");
	if (!self->IsValidCoords(coords))
	{
		return L.FApiParamError("The coords ({0}) are out of range ({1} - {2})",
			coords, self->GetOrigin(), self->GetOrigin() + self->GetSize() - Vector3i{1, 1, 1}
		);
	}

	// Get the block info:
	L.Push((self->*Fn)(coords.x, coords.y, coords.z));
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
	DataType (cBlockArea::*Fn)(int, int, int) const
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
		return L.ApiParamError("The area doesn't contain the datatype (%d)", DataTypeFlag);
	}

	// Read the overloaded params:
	Vector3i coords;
	readVector3iOverloadParams(L, 2, coords, "coords");
	if (!self->IsValidRelCoords(coords))
	{
		return L.FApiParamError("The coords ({0}) are out of range ({1})",
			coords, (self->GetSize() - Vector3i(1, 1, 1))
		);
	}

	// Get the block info:
	L.Push((self->*Fn)(coords.x, coords.y, coords.z));
	return 1;
}





/** Templated bindings for the SetBlock___() functions.
DataType is either BLOCKTYPE or NIBBLETYPE.
DataTypeFlag is the ba___ constant used for the datatypebeing manipulated.
Fn is the setter function.
Also supports the Vector3i overloads (TODO: document these (?)). */
template <
	typename DataType,
	int DataTypeFlag,
	void (cBlockArea::*Fn)(int, int, int, DataType)
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
		return L.ApiParamError("The area doesn't contain the datatype (%d)", DataTypeFlag);
	}

	// Read the overloaded params:
	Vector3i coords;
	auto idx = readVector3iOverloadParams(L, 2, coords, "coords");
	if (!self->IsValidCoords(coords))
	{
		return L.FApiParamError("The coords ({0}) are out of range ({1} - {2})",
			coords, self->GetOrigin(), self->GetOrigin() + self->GetSize() - Vector3i{1, 1, 1}
		);
	}
	DataType data;
	L.GetStackValues(idx, data);

	// Set the block info:
	(self->*Fn)(coords.x, coords.y, coords.z, data);
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
	void (cBlockArea::*Fn)(int, int, int, DataType)
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
		return L.ApiParamError("The area doesn't contain the datatype (%d)", DataTypeFlag);
	}

	// Read the overloaded params:
	Vector3i coords;
	auto idx = readVector3iOverloadParams(L, 2, coords, "coords");
	if (!self->IsValidRelCoords(coords))
	{
		return L.FApiParamError("The coords ({0}) are out of range ({1})",
			coords, (self->GetSize() - Vector3i(1, 1, 1))
		);
	}
	DataType data;
	L.GetStackValues(idx, data);

	// Set the block info:
	(self->*Fn)(coords.x, coords.y, coords.z, data);
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
	if (!self->HasBlockTypes() || !self->HasBlockMetas())
	{
		return L.ApiParamError("The area doesn't contain the datatypes baTypes and baMetas");
	}

	// Read the overloaded params:
	Vector3i coords;
	auto idx = readVector3iOverloadParams(L, 2, coords, "coords");
	if (!self->IsValidCoords(coords))
	{
		return L.FApiParamError("The coords ({0}) are out of range ({1} - {2})",
			coords, self->GetOrigin(), self->GetOrigin() + self->GetSize() - Vector3i{1, 1, 1}
		);
	}

	BLOCKTYPE block;
	NIBBLETYPE meta;
	if (!L.GetStackValues(idx, block, meta))
	{
		return L.ApiParamError("Bad number for block type or meta type");
	}

	// Set block type and meta:
	self->SetBlockTypeMeta(coords.x, coords.y, coords.z, block, meta);
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
	if (!self->HasBlockTypes() || !self->HasBlockMetas())
	{
		return L.ApiParamError("The area doesn't contain the baTypes or baMetas datatypes (0x%02x)", self->GetDataTypes());
	}

	// Read the overloaded params:
	Vector3i coords;
	auto idx = readVector3iOverloadParams(L, 2, coords, "coords");
	if (!self->IsValidRelCoords(coords))
	{
		return L.FApiParamError("The coords ({0}) are out of range ({1})",
			coords, (self->GetSize() - Vector3i(1, 1, 1))
		);
	}

	BLOCKTYPE block;
	NIBBLETYPE meta;
	if (!L.GetStackValues(idx, block, meta))
	{
		return L.ApiParamError("Bad number for block type or meta type");
	}

	// Set block type and meta:
	self->SetRelBlockTypeMeta(coords.x, coords.y, coords.z, block, meta);
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
			tolua_function(a_LuaState, "GetBlockLight",           GetBlock<NIBBLETYPE, cBlockArea::baLight,    &cBlockArea::GetRelBlockLight>);
			tolua_function(a_LuaState, "GetBlockMeta",            GetBlock<NIBBLETYPE, cBlockArea::baMetas,    &cBlockArea::GetRelBlockMeta>);
			tolua_function(a_LuaState, "GetBlockSkyLight",        GetBlock<NIBBLETYPE, cBlockArea::baSkyLight, &cBlockArea::GetRelBlockSkyLight>);
			tolua_function(a_LuaState, "GetBlockType",            GetBlock<BLOCKTYPE,  cBlockArea::baTypes,    &cBlockArea::GetRelBlockType>);
			tolua_function(a_LuaState, "GetBlockTypeMeta",        tolua_cBlockArea_GetBlockTypeMeta);
			tolua_function(a_LuaState, "GetCoordRange",           tolua_cBlockArea_GetCoordRange);
			tolua_function(a_LuaState, "GetNonAirCropRelCoords",  tolua_cBlockArea_GetNonAirCropRelCoords);
			tolua_function(a_LuaState, "GetOrigin",               tolua_cBlockArea_GetOrigin);
			tolua_function(a_LuaState, "GetRelBlockLight",        GetRelBlock<NIBBLETYPE, cBlockArea::baLight,    &cBlockArea::GetRelBlockLight>);
			tolua_function(a_LuaState, "GetRelBlockMeta",         GetRelBlock<NIBBLETYPE, cBlockArea::baMetas,    &cBlockArea::GetRelBlockMeta>);
			tolua_function(a_LuaState, "GetRelBlockSkyLight",     GetRelBlock<NIBBLETYPE, cBlockArea::baSkyLight, &cBlockArea::GetRelBlockSkyLight>);
			tolua_function(a_LuaState, "GetRelBlockType",         GetRelBlock<BLOCKTYPE,  cBlockArea::baTypes,    &cBlockArea::GetRelBlockType>);
			tolua_function(a_LuaState, "GetRelBlockTypeMeta",     tolua_cBlockArea_GetRelBlockTypeMeta);
			tolua_function(a_LuaState, "GetSize",                 tolua_cBlockArea_GetSize);
			tolua_function(a_LuaState, "LoadFromSchematicFile",   tolua_cBlockArea_LoadFromSchematicFile);
			tolua_function(a_LuaState, "LoadFromSchematicString", tolua_cBlockArea_LoadFromSchematicString);
			tolua_function(a_LuaState, "Read",                    tolua_cBlockArea_Read);
			tolua_function(a_LuaState, "RelLine",                 tolua_cBlockArea_RelLine);
			tolua_function(a_LuaState, "SaveToSchematicFile",     tolua_cBlockArea_SaveToSchematicFile);
			tolua_function(a_LuaState, "SaveToSchematicString",   tolua_cBlockArea_SaveToSchematicString);
			tolua_function(a_LuaState, "SetBlockType",            SetBlock<BLOCKTYPE,  cBlockArea::baTypes,    &cBlockArea::SetRelBlockType>);
			tolua_function(a_LuaState, "SetBlockMeta",            SetBlock<NIBBLETYPE, cBlockArea::baMetas,    &cBlockArea::SetRelBlockMeta>);
			tolua_function(a_LuaState, "SetBlockLight",           SetBlock<NIBBLETYPE, cBlockArea::baLight,    &cBlockArea::SetRelBlockLight>);
			tolua_function(a_LuaState, "SetBlockSkyLight",        SetBlock<NIBBLETYPE, cBlockArea::baSkyLight, &cBlockArea::SetRelBlockSkyLight>);
			tolua_function(a_LuaState, "SetBlockTypeMeta",        tolua_cBlockArea_SetBlockTypeMeta);
			tolua_function(a_LuaState, "SetRelBlockType",         SetRelBlock<BLOCKTYPE,  cBlockArea::baTypes,    &cBlockArea::SetRelBlockType>);
			tolua_function(a_LuaState, "SetRelBlockMeta",         SetRelBlock<NIBBLETYPE, cBlockArea::baMetas,    &cBlockArea::SetRelBlockMeta>);
			tolua_function(a_LuaState, "SetRelBlockLight",        SetRelBlock<NIBBLETYPE, cBlockArea::baLight,    &cBlockArea::SetRelBlockLight>);
			tolua_function(a_LuaState, "SetRelBlockSkyLight",     SetRelBlock<NIBBLETYPE, cBlockArea::baSkyLight, &cBlockArea::SetRelBlockSkyLight>);
			tolua_function(a_LuaState, "SetRelBlockTypeMeta",     tolua_cBlockArea_SetRelBlockTypeMeta);
			tolua_function(a_LuaState, "Write",                   tolua_cBlockArea_Write);
		tolua_endmodule(a_LuaState);
	tolua_endmodule(a_LuaState);
}




