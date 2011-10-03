#pragma once

class cSign													//tolua_export
{															//tolua_export
public:
	static char RotationToMetaData( float a_Rotation )		//tolua_export
	{														//tolua_export
		a_Rotation += 180 + (180/16); // So its not aligned with axis
		if( a_Rotation > 360.f ) a_Rotation -= 360.f;
		
		a_Rotation = (a_Rotation/360) * 16;
		
		return ((char)a_Rotation) % 16;
	}														//tolua_export
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
	}	
};															//tolua_export