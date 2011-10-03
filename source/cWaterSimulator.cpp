#include "cWaterSimulator.h"
#include "cWorld.h"
#include "Vector3i.h"
#include "BlockID.h"
#include <vector>

class cWaterSimulator::WaterData
{
public:
	WaterData( cWorld* a_World )
		: m_ActiveWater( new std::vector< Vector3i >() )
		, m_Buffer( new std::vector< Vector3i >() )
		, m_World( a_World )
	{}

	std::vector< Vector3i > GetLowestPoints( int a_X, int a_Y, int a_Z )
	{
		std::vector< Vector3i > Points;
		if( m_World->GetBlock(a_X, a_Y-1, a_Z) == E_BLOCK_AIR )
		{
			Points.push_back( Vector3i( a_X, a_Y-1, a_Z ) );
			return Points;
		}

		Vector3i LowerPoints [] = {
			Vector3i( a_X-1, a_Y-1, a_Z ),
			Vector3i( a_X+1, a_Y-1, a_Z ),
			Vector3i( a_X, a_Y-1, a_Z-1 ),
			Vector3i( a_X, a_Y-1, a_Z+1 ),
		};
		bool bWaterFound = false;
		for( int i = 0; i < 4; ++i )
		{
			char Block1 = m_World->GetBlock( LowerPoints[i].x, LowerPoints[i].y, LowerPoints[i].z );
			char Block2 = m_World->GetBlock( LowerPoints[i].x, a_Y, LowerPoints[i].z );
			if( Block1 == E_BLOCK_AIR && Block2 == E_BLOCK_AIR )
			{
				Points.push_back( LowerPoints[i] );
				LowerPoints[i].y = a_Y;
				Points.push_back( LowerPoints[i] );
			}
			else if( (Block2 == E_BLOCK_WATER || Block2 == E_BLOCK_STATIONARY_WATER ) && ( Block1 == E_BLOCK_AIR || Block1 == E_BLOCK_WATER || Block1 == E_BLOCK_STATIONARY_WATER ) )
			{
				bWaterFound = true;
			}
		}

		if( Points.size() == 0 && !bWaterFound )
		{
			Vector3i LevelPoints [] = {
				Vector3i( a_X-1, a_Y, a_Z ),
				Vector3i( a_X+1, a_Y, a_Z ),
				Vector3i( a_X, a_Y, a_Z-1 ),
				Vector3i( a_X, a_Y, a_Z+1 ),
			};
			for( int i = 0; i < 4; ++i )
			{
				char Block = m_World->GetBlock( LevelPoints[i].x, a_Y, LevelPoints[i].z );
				if( Block == E_BLOCK_AIR || Block == E_BLOCK_WATER || Block == E_BLOCK_STATIONARY_WATER )
					Points.push_back( LevelPoints[i] );
			}
		}
		return Points;
	}

	std::vector< Vector3i >* m_ActiveWater;
	std::vector< Vector3i >* m_Buffer;
	cWorld* m_World;
};

cWaterSimulator::cWaterSimulator( cWorld* a_World )
	: m_World( a_World )
	, m_Data( new WaterData( a_World ) )
{
}

cWaterSimulator::~cWaterSimulator()
{
}

void cWaterSimulator::WakeUp( int a_X, int a_Y, int a_Z )
{
	AddBlock( a_X, a_Y, a_Z );
	AddBlock( a_X-1, a_Y, a_Z );
	AddBlock( a_X+1, a_Y, a_Z );
	AddBlock( a_X, a_Y-1, a_Z );
	AddBlock( a_X, a_Y+1, a_Z );
	AddBlock( a_X, a_Y, a_Z-1 );
	AddBlock( a_X, a_Y, a_Z+1 );
}

void cWaterSimulator::AddBlock( int a_X, int a_Y, int a_Z )
{
	// Check for duplicates
	std::vector< Vector3i > & ActiveWater = *m_Data->m_ActiveWater;
	for( std::vector< Vector3i >::iterator itr = ActiveWater.begin(); itr != ActiveWater.end(); ++itr )
	{
		Vector3i & pos = *itr;
		if( pos.x == a_X && pos.y == a_Y && pos.z == a_Z )
			return;
	}

	ActiveWater.push_back( Vector3i( a_X, a_Y, a_Z ) );
}

char cWaterSimulator::GetHighestLevelAround( int a_X, int a_Y, int a_Z )
{
	char Max = 8;
#define __HIGHLEVEL_CHECK__( x, y, z ) \
	if( IsWaterBlock( m_World->GetBlock( x, y, z ) ) ) \
	{ \
		char Meta; \
		if( (Meta = m_World->GetBlockMeta( x, y, z ) ) < Max ) Max = Meta; \
		else if( Meta == 8 ) Max = 0; \
		if( Max == 0 ) return 0; \
	}

	__HIGHLEVEL_CHECK__( a_X-1, a_Y, a_Z );
	__HIGHLEVEL_CHECK__( a_X+1, a_Y, a_Z );
	__HIGHLEVEL_CHECK__( a_X, a_Y, a_Z-1 );
	__HIGHLEVEL_CHECK__( a_X, a_Y, a_Z+1 );

	return Max;
}

void cWaterSimulator::Simulate( float a_Dt )
{
	m_Timer += a_Dt;

	std::swap( m_Data->m_ActiveWater, m_Data->m_Buffer );	// Swap so blocks can be added to empty ActiveWater array
	m_Data->m_ActiveWater->clear();

	std::vector< Vector3i > & WaterBlocks = *m_Data->m_Buffer;
	for( std::vector< Vector3i >::iterator itr = WaterBlocks.begin(); itr != WaterBlocks.end(); ++itr )
	{
		Vector3i & pos = *itr;
		char BlockID = m_World->GetBlock( pos.x, pos.y, pos.z );
		if( IsWaterBlock( BlockID ) ) // only care about water
		{
			bool bIsFed = false;
			char Meta = m_World->GetBlockMeta( pos.x, pos.y, pos.z );
			char Feed = Meta;
			if( Meta == 8 ) // Falling water
			{
				if( IsWaterBlock( m_World->GetBlock(pos.x, pos.y+1, pos.z) ) )	// Block above is water
				{
					bIsFed = true;
					Meta = 0;	// Make it a full block
				}
			}
			else if( Meta == 0 ) // It's a full block, so it's always fed
			{
				bIsFed = true;
			}
			else
			{
				if( (Feed = GetHighestLevelAround( pos.x, pos.y, pos.z )) < Meta )
					bIsFed = true;
			}


			if( bIsFed )
			{
				char DownID = m_World->GetBlock( pos.x, pos.y-1, pos.z );
				if( DownID == E_BLOCK_AIR || IsWaterBlock( DownID ) ) // free for water
				{
					m_World->FastSetBlock( pos.x, pos.y-1, pos.z, E_BLOCK_WATER, 8 ); // falling
					AddBlock( pos.x, pos.y-1, pos.z );
				}
				else // Not falling water
				{
					if( Feed+1 < Meta )
					{
						m_World->FastSetBlock( pos.x, pos.y, pos.z, E_BLOCK_WATER, Feed+1 );
						AddBlock( pos.x, pos.y, pos.z );
					}
					else if( Meta < 7 ) // 7 is only 1 unit high, so it cannot spread, lower than 7 can though.
					{
						std::vector< Vector3i > Points = m_Data->GetLowestPoints( pos.x, pos.y, pos.z );
						for( std::vector< Vector3i >::iterator itr = Points.begin(); itr != Points.end(); ++itr )
						{
							Vector3i & p = *itr;
							char BlockID = m_World->GetBlock( p.x, p.y, p.z );
							if( !IsWaterBlock( BlockID ) )
							{
								if( p.y == pos.y )
									m_World->FastSetBlock(p.x, p.y, p.z, E_BLOCK_WATER, Meta+1);
								else
									m_World->FastSetBlock(p.x, p.y, p.z, E_BLOCK_WATER, 8);
								AddBlock( p.x, p.y, p.z );
							}
							else // it's water
							{
								char PointMeta = m_World->GetBlockMeta( p.x, p.y, p.z );
								if( PointMeta > Meta+1 )
								{
									AddBlock( p.x, p.y, p.z );
								}
							}
						}
					}
				}
			}
			else // not fed
			{
				m_World->FastSetBlock( pos.x, pos.y, pos.z, E_BLOCK_AIR, 0 );
				WakeUp( pos.x, pos.y, pos.z );
			}
		}
	}
}

bool cWaterSimulator::IsWaterBlock( char a_BlockID )
{
	return a_BlockID == E_BLOCK_WATER || a_BlockID == E_BLOCK_STATIONARY_WATER;
}