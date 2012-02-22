#pragma once

class cWorld;
class cRedstone
{
public:

	cRedstone( cWorld* a_World );

	static char RepeaterRotationToMetaData( float a_Rotation )
	{
			a_Rotation += 90 + 45; // So its not aligned with axis
			if( a_Rotation > 360.f ) a_Rotation -= 360.f;
			if( a_Rotation >= 0.f && a_Rotation < 90.f )
					return 0x1;
			else if( a_Rotation >= 180 && a_Rotation < 270 )
					return 0x3;
			else if( a_Rotation >= 90 && a_Rotation < 180 )
					return 0x2;
			else
					return 0x0;
	}


	void CalculateRedstone( int, int, int );
	void ChangeRedstone( int, int, int, bool );
	bool IsBlockPowered( int, int, int );

	cWorld* m_World;

	char m_Metadata;
	char m_Direction;
	
	static bool s_UseRedstone;
};
