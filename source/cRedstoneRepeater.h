#pragma once

class cRedstoneRepeater												//tolua_export
{															//tolua_export
public:
	static char RotationToMetaData( float a_Rotation )		//tolua_export
	{														//tolua_export
printf("a_Rotation: %f\n",a_Rotation);
		a_Rotation += 90 + 45; // So its not aligned with axis
		if( a_Rotation > 360.f ) a_Rotation -= 360.f;
		if( a_Rotation >= 0.f && a_Rotation < 90.f ){
			return 0x1;}
		else if( a_Rotation >= 180 && a_Rotation < 270 ){
			return 0x3;}
		else if( a_Rotation >= 90 && a_Rotation < 180 ){
			return 0x2;}
		else
			return 0x0;
	}														//tolua_export
};															//tolua_export
