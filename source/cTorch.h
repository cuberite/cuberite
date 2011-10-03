#pragma once

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

};															//tolua_export