#pragma once

class cLadder												//tolua_export
{															//tolua_export
public:

	static char DirectionToMetaData( char a_Direction )		//tolua_export
	{														//tolua_export
		switch( a_Direction )
		{
		case 0x2:
			return 0x2;
		case 0x3:
			return 0x3;
		case 0x4:
			return 0x4;
		case 0x5:
			return 0x5;
		default:
			break;
		};
		return 0x2;
	}														//tolua_export

	static char MetaDataToDirection( char a_MetaData )		//tolua_export
	{														//tolua_export
		switch( a_MetaData )
		{
		case 0x2:
			return 0x2;
		case 0x3:
			return 0x3;
		case 0x4:
			return 0x4;
		case 0x5:
			return 0x5;
		default:
			break;
		};
		return 0x2;
	}														//tolua_export

};															//tolua_export