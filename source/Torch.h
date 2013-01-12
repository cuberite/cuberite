
#pragma once
#include "Vector3i.h"
#include "Defines.h"





class cTorch												// tolua_export
{															// tolua_export
public:

	static char DirectionToMetaData( char a_Direction )		// tolua_export
	{														// tolua_export
		switch (a_Direction)
		{
			case BLOCK_FACE_BOTTOM: ASSERT(!"Shouldn't be getting this face"); return 0;
			case BLOCK_FACE_TOP:    return E_META_TORCH_FLOOR;
			case BLOCK_FACE_EAST:   return E_META_TORCH_EAST;
			case BLOCK_FACE_WEST:   return E_META_TORCH_WEST;
			case BLOCK_FACE_NORTH:  return E_META_TORCH_NORTH;
			case BLOCK_FACE_SOUTH:  return E_META_TORCH_SOUTH;
			default:
			{
				ASSERT(!"Unhandled torch direction!");
				break;
			}
		};
		return 0x0;
	}														// tolua_export
	

	static char MetaDataToDirection(char a_MetaData)		// tolua_export
	{														// tolua_export
		switch (a_MetaData)
		{
			case 0:                  return BLOCK_FACE_TOP;  // by default, the torches stand on the ground
			case E_META_TORCH_FLOOR: return BLOCK_FACE_TOP;
			case E_META_TORCH_EAST:  return BLOCK_FACE_EAST;
			case E_META_TORCH_WEST:  return BLOCK_FACE_WEST;
			case E_META_TORCH_NORTH: return BLOCK_FACE_NORTH;
			case E_META_TORCH_SOUTH: return BLOCK_FACE_SOUTH;
			default:
			{
				ASSERT(!"Unhandled torch metadata");
				break;
			}
		}
		return 0;
	}														// tolua_export


	static bool IsAttachedTo(const Vector3i & a_TorchPos, char a_TorchMeta, const Vector3i & a_BlockPos)
	{
		switch (a_TorchMeta)
		{
			case 0x0:
			case E_META_TORCH_FLOOR: return ((a_TorchPos - a_BlockPos).Equals(Vector3i(0, 1, 0)));
			case E_META_TORCH_EAST:  return ((a_TorchPos - a_BlockPos).Equals(Vector3i(0, 0, -1)));
			case E_META_TORCH_WEST:  return ((a_TorchPos - a_BlockPos).Equals(Vector3i(0, 0, 1)));
			case E_META_TORCH_NORTH: return ((a_TorchPos - a_BlockPos).Equals(Vector3i(-1, 0, 0)));
			case E_META_TORCH_SOUTH: return ((a_TorchPos - a_BlockPos).Equals(Vector3i(1, 0, 0)));
			default:
			{
				ASSERT(!"Unhandled torch meta!");
				break;
			}
		}
		return false;
	}

} ;															// tolua_export




