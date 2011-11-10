#pragma once

class cDoors												//tolua_export
{															//tolua_export
public:
	static char RotationToMetaData( float a_Rotation )		//tolua_export
	{														//tolua_export
		a_Rotation += 90 + 45; // So its not aligned with axis
		if( a_Rotation > 360.f ) a_Rotation -= 360.f;
		if( a_Rotation >= 0.f && a_Rotation < 90.f )
			return 0x0;
		else if( a_Rotation >= 180 && a_Rotation < 270 )
			return 0x2;
		else if( a_Rotation >= 90 && a_Rotation < 180 )
			return 0x1;
		else
			return 0x3;
	}														//tolua_export

	static char ChangeStateMetaData( char MetaData )		//tolua_export
	{														//tolua_export
		if ( (int)MetaData == 0 ) { //todo, condense this code. lol
			return 0x4;
		} else if ( (int)MetaData == 1 ) {
			return 0x5;
		} else if ( (int)MetaData == 2 ) {
			return 0x6;
		} else if ( (int)MetaData == 3 ) {
			return 0x7;
		} else if ( (int)MetaData == 4 ) {
			return 0x0;
		} else if ( (int)MetaData == 5 ) {
			return 0x1;
		} else if ( (int)MetaData == 6 ) {
			return 0x2;
		} else if ( (int)MetaData == 7 ) {
			return 0x3;
		} else if ( (int)MetaData == 8 ) {
			return 0xC;
		} else if ( (int)MetaData == 9 ) {
			return 0xD;
		} else if ( (int)MetaData == 10 ) {
			return 0xE;
		} else if ( (int)MetaData == 11 ) {
			return 0xF;
		} else if ( (int)MetaData == 12 ) {
			return 0x8;
		} else if ( (int)MetaData == 13 ) {
			return 0x9;
		} else if ( (int)MetaData == 14 ) {
			return 0xA;
		} else if ( (int)MetaData == 15 ) {
			return 0xB;
		}
	}												//tolua_export

};															//tolua_export