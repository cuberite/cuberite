#pragma once
#include <cstdio>
class cWorld;
class cPiston
{
public:

cPiston( cWorld* a_World );

static char RotationPitchToMetaData( float a_Rotation, float a_Pitch )
{
	//if pitch higher than this value set up
	//else if pitch lower than this value set down,
	//else just do rotation:
	std::printf("pre:a_Rotation %f \n",a_Rotation);
        std::printf("a_Pitch %f \n",a_Pitch);

	if (a_Pitch >= 50.f ){
		return 0x1;
	} else if ( a_Pitch <= -50.f ) {
		return 0x0;
	} else {

	        a_Rotation += 90 + 45; // So its not aligned with axis
		std::printf("a_Rotation %f \n",a_Rotation);

	        if( a_Rotation > 360.f ) a_Rotation -= 360.f;
	        if( a_Rotation >= 0.f && a_Rotation < 90.f )
	                return 0x4;
	        else if( a_Rotation >= 180 && a_Rotation < 270 )
	                return 0x5;
	        else if( a_Rotation >= 90 && a_Rotation < 180 )
	                return 0x2;
	        else
	                return 0x3;
	}
}


void ExtendPiston( int, int, int );
void RetractPiston( int, int, int );

cWorld* m_World;

};
