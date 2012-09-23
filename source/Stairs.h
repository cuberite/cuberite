#pragma once

class cStairs												//tolua_export
{															//tolua_export
public:
	static char RotationToMetaData( float a_Rotation, int a_Direction )		//tolua_export
	{														//tolua_export
    a_Rotation += 90 + 45; // So its not aligned with axis
    char result = 0x0;
    if( a_Direction == 0)
    {
      
      result = 0x4;
    }

		if( a_Rotation > 360.f ) a_Rotation -= 360.f;
		if( a_Rotation >= 0.f && a_Rotation < 90.f )
			return result;
		else if( a_Rotation >= 180 && a_Rotation < 270 )
			result += 0x1;
		else if( a_Rotation >= 90 && a_Rotation < 180 )
			result += 0x2;
		else
			result += 0x3;

    return result;
	}														//tolua_export
};															//tolua_export
