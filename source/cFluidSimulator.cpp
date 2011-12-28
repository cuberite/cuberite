#include "cFluidSimulator.h"
#include "cWorld.h"
#include "Vector3i.h"
#include "BlockID.h"
#include "Defines.h"
#include <vector>

class cFluidSimulator::FluidData
{
public:
	FluidData( cWorld* a_World, cFluidSimulator *a_Simulator )
		: m_ActiveFluid( new std::vector< Vector3i >() )
		, m_Simulator (a_Simulator)
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
		bool bFluidFound = false;
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
			else if( m_Simulator->IsAllowedBlock(Block2) && m_Simulator->IsPassableForFluid(Block1) )
			{
				bFluidFound = true;
			}
		}

		if( Points.size() == 0 && !bFluidFound )
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
				if( m_Simulator->IsPassableForFluid(Block) )
					Points.push_back( LevelPoints[i] );
			}
		}
		return Points;
	}

	std::vector< Vector3i >* m_ActiveFluid;
	std::vector< Vector3i >* m_Buffer;
	cWorld* m_World;
	cFluidSimulator *m_Simulator;
};

cFluidSimulator::cFluidSimulator( cWorld* a_World )
	: cSimulator(a_World)
	, m_Data(0)
{
	m_Data = new FluidData(a_World, this);
}

cFluidSimulator::~cFluidSimulator()
{
	delete m_Data;
}

void cFluidSimulator::AddBlock( int a_X, int a_Y, int a_Z )
{
	if(!IsAllowedBlock(m_World->GetBlock(a_X, a_Y, a_Z)))		//This should save very much time because it doesn´t have to iterate through all blocks
		return;

	// Check for duplicates
	std::vector< Vector3i > & ActiveFluid = *m_Data->m_ActiveFluid;
	for( std::vector< Vector3i >::iterator itr = ActiveFluid.begin(); itr != ActiveFluid.end(); ++itr )
	{
		Vector3i & Pos = *itr;
		if( Pos.x == a_X && Pos.y == a_Y && Pos.z == a_Z )
			return;
	}

	ActiveFluid.push_back( Vector3i( a_X, a_Y, a_Z ) );
}

char cFluidSimulator::GetHighestLevelAround( int a_X, int a_Y, int a_Z )
{
	char Max = m_MaxHeight + 1;

#define __HIGHLEVEL_CHECK__( x, y, z ) \
	if( IsAllowedBlock( m_World->GetBlock( x, y, z ) ) ) \
	{ \
		char Meta; \
		if( (Meta = m_World->GetBlockMeta( x, y, z ) ) < Max ) Max = Meta; \
		else if( Meta == m_MaxHeight + 1 ) Max = 0; \
		if( Max == 0 ) return 0; \
	}

	__HIGHLEVEL_CHECK__( a_X-1, a_Y, a_Z );
	__HIGHLEVEL_CHECK__( a_X+1, a_Y, a_Z );
	__HIGHLEVEL_CHECK__( a_X, a_Y, a_Z-1 );
	__HIGHLEVEL_CHECK__( a_X, a_Y, a_Z+1 );

	return Max;
}

void cFluidSimulator::Simulate( float a_Dt )
{
	m_Timer += a_Dt;

	if(m_Data->m_ActiveFluid->empty())	//Nothing to do if there is no active fluid ;) saves very little time ;D
		return;

	std::swap( m_Data->m_ActiveFluid, m_Data->m_Buffer );	// Swap so blocks can be added to empty ActiveFluid array
	m_Data->m_ActiveFluid->clear();

	std::vector< Vector3i > & FluidBlocks = *m_Data->m_Buffer;
	for( std::vector< Vector3i >::iterator itr = FluidBlocks.begin(); itr != FluidBlocks.end(); ++itr )
	{
		
		Vector3i & pos = *itr;
		char BlockID = m_World->GetBlock( pos.x, pos.y, pos.z );
		if( IsAllowedBlock( BlockID ) ) // only care about own fluid
		{
			bool bIsFed = false;
			char Meta = m_World->GetBlockMeta( pos.x, pos.y, pos.z );
			char Feed = Meta;
			if( Meta & 8 ) // Falling fluid
			{
				if( IsAllowedBlock( m_World->GetBlock(pos.x, pos.y+1, pos.z) ) )	// Block above is fluid
				{
					bIsFed = true;
					Meta = 0;	// Make it a full block
				}
			}
			else if( Meta < m_FlowReduction ) // It's a full block, so it's always fed
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
				if( IsPassableForFluid(DownID) ) // free for fluid 
				{
					m_World->FastSetBlock( pos.x, pos.y-1, pos.z, m_FluidBlock, 8 ); // falling
					AddBlock( pos.x, pos.y-1, pos.z );
				}
				else // Not falling
				{
					if( Feed + m_FlowReduction < Meta )
					{
						m_World->FastSetBlock( pos.x, pos.y, pos.z, m_FluidBlock, Feed + m_FlowReduction );
						AddBlock( pos.x, pos.y, pos.z );
					}
					else if( Meta < m_MaxHeight ) // max is the lowest, so it cannot spread
					{
						std::vector< Vector3i > Points = m_Data->GetLowestPoints( pos.x, pos.y, pos.z );
						for( std::vector< Vector3i >::iterator itr = Points.begin(); itr != Points.end(); ++itr )
						{
							Vector3i & p = *itr;
							char BlockID = m_World->GetBlock( p.x, p.y, p.z );
							if( !IsAllowedBlock( BlockID ) )
							{
								if( p.y == pos.y )
									m_World->FastSetBlock(p.x, p.y, p.z, m_FluidBlock, Meta + m_FlowReduction);
								else
									m_World->FastSetBlock(p.x, p.y, p.z, m_FluidBlock, 8);
								AddBlock( p.x, p.y, p.z );
							}
							else // it's fluid
							{
								char PointMeta = m_World->GetBlockMeta( p.x, p.y, p.z );
								if( PointMeta > Meta + m_FlowReduction )
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

//TODO Not working very well yet :s
Direction cFluidSimulator::GetFlowingDirection(int a_X, int a_Y, int a_Z, bool a_Over)
{
	char BlockID = m_World->GetBlock(a_X, a_Y, a_Z);
	if(!IsAllowedBlock(BlockID))	//No Fluid -> No Flowing direction :D
		return NONE;
	

	/*
	Disabled because of causing problems and beeing useless atm
	char BlockBelow = m_World->GetBlock(a_X, a_Y - 1, a_Z);		//If there is nothing or fluid below it -> dominating flow is down :D
	if(BlockBelow == E_BLOCK_AIR || IsAllowedBlock(BlockBelow))
		return Y_MINUS;
	*/

	char LowestPoint = m_World->GetBlockMeta(a_X, a_Y, a_Z);	//Current Block Meta so only lower points will be counted
	int X = 0, Y = 0, Z = 0;									//Lowest Pos will be stored here

	if(IsAllowedBlock(m_World->GetBlock(a_X, a_Y + 1, a_Z)) && a_Over)		//check for upper block to flow because this also affects the flowing direction
	{
		return GetFlowingDirection(a_X, a_Y + 1, a_Z, false);
	}

	std::vector< Vector3i * > Points;

	//add blocks around the checking pos
	Points.push_back(new Vector3i(a_X - 1, a_Y, a_Z));
	Points.push_back(new Vector3i(a_X + 1, a_Y, a_Z));
	Points.push_back(new Vector3i(a_X, a_Y, a_Z + 1));
	Points.push_back(new Vector3i(a_X, a_Y, a_Z - 1));

	for(std::vector<Vector3i *>::iterator it = Points.begin(); it < Points.end(); it++)
	{
		Vector3i *Pos = (*it);
		char BlockID = m_World->GetBlock(Pos->x, Pos->y, Pos->z);
		if(IsAllowedBlock(BlockID))
		{
			char Meta = m_World->GetBlockMeta(Pos->x, Pos->y, Pos->z);

			if(Meta > LowestPoint)
			{
				LowestPoint = Meta;
				X = Pos->x;
				Y = Pos->y;
				Z = Pos->z;
			}
		}else if(BlockID == E_BLOCK_AIR)
		{
			LowestPoint = 9;		//This always dominates
			X = Pos->x;
			Y = Pos->y;
			Z = Pos->z;
		
		}
		delete Pos;
	}


	if(LowestPoint == m_World->GetBlockMeta(a_X, a_Y, a_Z))
		return NONE;

	if(a_X - X > 0)
	{
		return X_MINUS;
	}

	if(a_X - X < 0)
	{
		return X_PLUS;
	}

	if(a_Z - Z > 0)
	{
		return Z_MINUS;
	}

	if(a_Z - Z < 0)
	{
		return Z_PLUS;
	}

	return NONE;


}