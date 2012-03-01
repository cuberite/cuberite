#pragma once

#include "Vector3i.h"

class cTorch												//tolua_export
{															//tolua_export
public:

	static char DirectionToMetaData( char a_Direction )		//tolua_export
	{														//tolua_export
		switch( a_Direction )
		{
		case 0x0:
			return 0x0;
		case 0x1:
			return 0x5; //standing on floor
		case 0x2:
			return 0x4; // south
		case 0x3:
			return 0x3; // north
		case 0x4:
			return 0x2; // west
		case 0x5:
			return 0x1; // east
		default:
			break;
		};
		return 0x0;
	}														//tolua_export

	static char MetaDataToDirection( char a_MetaData )		//tolua_export
	{														//tolua_export
		switch( a_MetaData )
		{
		case 0x0:
			return 0x0;
		case 0x1:
			return 0x5;
		case 0x2:
			return 0x4;
		case 0x3:
			return 0x3;
		case 0x4:
			return 0x2;
		case 0x5:
			return 0x1;
		default:
			break;
		};
		return 0x0;
	}														//tolua_export

	static bool IsAttachedTo( const Vector3i & a_TorchPos, char a_TorchMeta, const Vector3i & a_BlockPos )
	{
		switch( a_TorchMeta )
		{
		case 0x0:
		case 0x5:	// On floor
			if( (a_TorchPos - a_BlockPos).Equals( Vector3i(0, 1, 0 ) ) )
			{
				return true;
			}
			break;
		case 0x4: // South -Z
			if( (a_TorchPos - a_BlockPos).Equals( Vector3i(0, 0, -1 ) ) )
			{
				return true;
			}
			break;
		case 0x3: // North +Z
			if( (a_TorchPos - a_BlockPos).Equals( Vector3i(0, 0, 1 ) ) )
			{
				return true;
			}
			break;
		case 0x2: // West -X
			if( (a_TorchPos - a_BlockPos).Equals( Vector3i(-1, 0, 0 ) ) )
			{
				return true;
			}
			break;
		case 0x1: // East +X
			if( (a_TorchPos - a_BlockPos).Equals( Vector3i( 1, 0, 0 ) ) )
			{
				return true;
			}
			break;
		default:
			break;
		};
		return false;
	}

};															//tolua_export