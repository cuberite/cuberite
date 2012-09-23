#pragma once

#include "Vector3i.h"

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

	static bool IsRepeaterPointingTo( const Vector3i & a_RepeaterPos, char a_MetaData, const Vector3i & a_BlockPos )
	{
		switch( a_MetaData & 0x3 )
		{
		case 0x0:
			if( (a_RepeaterPos - a_BlockPos).Equals( Vector3i( 0, 0, 1 ) ) )
			{
				return true;
			}
			break;
		case 0x1:
			if( (a_RepeaterPos - a_BlockPos).Equals( Vector3i(-1, 0, 0 ) ) )
			{
				return true;
			}
			break;
		case 0x2:
			if( (a_RepeaterPos - a_BlockPos).Equals( Vector3i( 0, 0,-1 ) ) )
			{
				return true;
			}
			break;
		case 0x3:
			if( (a_RepeaterPos - a_BlockPos).Equals( Vector3i( 1, 0, 0 ) ) )
			{
				return true;
			}
			break;
		default:
			break;
		}
		return false;
	}

	static bool IsRepeaterPointingAway( const Vector3i & a_RepeaterPos, char a_MetaData, const Vector3i & a_BlockPos )
	{
		switch( a_MetaData & 0x3 )
		{
		case 0x0:
			if( (a_RepeaterPos - a_BlockPos).Equals( Vector3i( 0, 0,-1 ) ) )
			{
				return true;
			}
			break;
		case 0x1:
			if( (a_RepeaterPos - a_BlockPos).Equals( Vector3i( 1, 0, 0 ) ) )
			{
				return true;
			}
			break;
		case 0x2:
			if( (a_RepeaterPos - a_BlockPos).Equals( Vector3i( 0, 0, 1 ) ) )
			{
				return true;
			}
			break;
		case 0x3:
			if( (a_RepeaterPos - a_BlockPos).Equals( Vector3i(-1, 0, 0 ) ) )
			{
				return true;
			}
			break;
		default:
			break;
		}
		return false;
	}

	static Vector3i GetRepeaterDirection( char a_MetaData )
	{
		switch( a_MetaData & 0x3 )
		{
		case 0x0:
			return Vector3i( 0, 0,-1 );
		case 0x1:
			return Vector3i( 1, 0, 0 );
		case 0x2:
			return Vector3i( 0, 0, 1 );
		case 0x3:
			return Vector3i(-1, 0, 0 );
		default:
			break;
		}
		return Vector3i();
	}

	void CalculateRedstone( int, int, int );
	void ChangeRedstone( int, int, int, bool );
	bool IsBlockPowered( int, int, int );

	cWorld* m_World;

	char m_Metadata;
	char m_Direction;
	
	static bool s_UseRedstone;
};
