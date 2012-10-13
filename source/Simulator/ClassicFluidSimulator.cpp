
// ClassicFluidSimulator.cpp

// Implements the cClassicFluidSimulator class representing the original MCServer's fluid simulator

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ClassicFluidSimulator.h"
#include "../World.h"
#include "../BlockID.h"
#include "../Defines.h"
#include "../Item.h"
#include "../Blocks/BlockHandler.h"






// #define DEBUG_FLUID

#ifdef DEBUG_FLUID
	#define LOG_FLUID(...) LOGD( __VA_ARGS__ )
#else
	#define LOG_FLUID(...)
#endif





class cClassicFluidSimulator::FluidData
{
public:
	FluidData(cWorld * a_World, cClassicFluidSimulator * a_Simulator )
		: m_ActiveFluid( new std::set < Vector3i >() )
		, m_Simulator (a_Simulator)
		, m_Buffer( new std::set< Vector3i >() )
		, m_World( a_World )
	{
	}


	~FluidData()
	{
		delete m_Buffer;
		delete m_ActiveFluid;
	}


	void UpdateWave(Vector3i a_LeftCorner, Vector3i a_CurBlock)
	{
		Vector3i LevelPoints [] = {
			Vector3i(a_CurBlock.x - 1, a_CurBlock.y, a_CurBlock.z),
			Vector3i(a_CurBlock.x + 1, a_CurBlock.y, a_CurBlock.z),
			Vector3i(a_CurBlock.x,     a_CurBlock.y, a_CurBlock.z - 1),
			Vector3i(a_CurBlock.x,     a_CurBlock.y, a_CurBlock.z + 1),
		};


		for (int i = 0; i < 4; i++)
		{
			Vector3i cur = LevelPoints[i];
			switch (m_Relief[cur.x][cur.z])
			{
				case E_HOLE:
				{
					m_StartSide[cur.x][cur.z] = m_StartSide[a_CurBlock.x][a_CurBlock.z];
					m_CurResult|=m_StartSide[cur.x][cur.z];
					m_NearestHole = m_WayLength[a_CurBlock.x][a_CurBlock.z] + 1;
					LOG_FLUID("Hole found: %d	\t curResult: %d", int(m_StartSide[cur.x][cur.z]), int(m_CurResult) );
					LOG_FLUID("Coordinates: (%d, %d)", cur.x, cur.z);
					break;
				}

				case E_BLOCK:
				{
					break;
				}

				case E_PLAIN:
				{
					if (m_WayLength[cur.x][cur.z] > m_WayLength[a_CurBlock.x][a_CurBlock.z] + 1)
					{
						m_WayLength[cur.x][cur.z] = m_WayLength[a_CurBlock.x][a_CurBlock.z] + 1;
						m_StartSide[cur.x][cur.z] = m_StartSide[a_CurBlock.x][a_CurBlock.z];
						m_WaveQueue.push(cur);
					}
					else if(m_WayLength[cur.x][cur.z] == m_WayLength[a_CurBlock.x][a_CurBlock.z] + 1)
					{
						m_StartSide[cur.x][cur.z] |= m_StartSide[a_CurBlock.x][a_CurBlock.z];
					}
					LOG_FLUID("Plain step: (%d, %d) from %d", cur.x, cur.z, m_StartSide[cur.x][cur.z]);
					break;
				}
			}
		}
	}


	std::vector< Vector3i > GetLowestPoints(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		std::vector< Vector3i > Points;  // result

		Vector3i CornerGlobal(a_BlockX - AREA_WIDTH / 2, a_BlockY, a_BlockZ - AREA_WIDTH / 2);

		// TODO: Rewrite without relief, get blocks directly in algorithm
		for (int x = 0; x < AREA_WIDTH; x++) 
		{
			for (int z = 0; z < AREA_WIDTH; z++)
			{
				char UpperBlock = m_World->GetBlock(CornerGlobal.x + x, CornerGlobal.y,     CornerGlobal.z + z);
				char DownBlock  = m_World->GetBlock(CornerGlobal.x + x, CornerGlobal.y - 1, CornerGlobal.z + z);

				if (m_Simulator->IsSolidBlock(UpperBlock) || (m_Simulator->IsStationaryFluidBlock(UpperBlock)))
				{
					m_Relief[x][z] = E_BLOCK;
				}
				else if (m_Simulator->IsSolidBlock(DownBlock))
				{
					m_Relief[x][z] = E_PLAIN;
				}
				else
				{
					m_Relief[x][z] = E_HOLE;
				}
				m_WayLength[x][z] = 255;
				m_StartSide[x][z] = E_SIDE_NONE;
			}
			LOG_FLUID("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", m_Relief[x][0], m_Relief[x][1], m_Relief[x][2], m_Relief[x][3], m_Relief[x][4], m_Relief[x][5], m_Relief[x][6], m_Relief[x][7], m_Relief[x][8], m_Relief[x][9], m_Relief[x][10]);
		}

		m_NearestHole = 5;
		m_CurResult = 0;
		while (!m_WaveQueue.empty()) m_WaveQueue.pop();  // _X 2012_10_13: WTF? why not use m_WaveQueue.clear() ?

		int left = AREA_WIDTH / 2 - 1;
		int right = AREA_WIDTH / 2 + 1;
		int center = AREA_WIDTH / 2;

		Vector3i r(right,  0, center);  // right block
		Vector3i l(left,   0, center);  // left block
		Vector3i f(center, 0, right);   // front block
		Vector3i b(center, 0, left);    // back block
		Vector3i c(center, 0, center);  // center block

		m_WayLength[c.x][c.z] = 0;

		Vector3i Nearest[] = {r, l, f, b};
		unsigned char Sides[] = {E_SIDE_RIGHT, E_SIDE_LEFT, E_SIDE_FRONT, E_SIDE_BACK};

		for (int i = 0; i < 4; i++)
		{
			Vector3i cur = Nearest[i];
			switch (m_Relief[cur.x][cur.z])
			{
				case E_HOLE:
				{
					m_StartSide[cur.x][cur.z] = Sides[i];
					m_CurResult |= m_StartSide[cur.x][cur.z];
					m_NearestHole = 1;
					LOG_FLUID("Hole found: %d	\t curResult: %d", int(Sides[i]), int(m_CurResult) );
					break;
				}

				case E_BLOCK:
				{
					break;
				}

				case E_PLAIN:
				{
					m_WaveQueue.push(cur);
					m_StartSide[cur.x][cur.z] = Sides[i];
					m_WayLength[cur.x][cur.z] = 1;
					LOG_FLUID("Plain found: %d", int(Sides[i]));
					break;
				}
			}
		}

		Vector3i curBlock;
		
		bool bContinue = !m_WaveQueue.empty();

		if (!m_WaveQueue.empty())
		{
			curBlock = m_WaveQueue.front();
			bContinue = (m_WayLength[curBlock.x][curBlock.z] < m_NearestHole);
		}

		while (bContinue)
		{
			LOG_FLUID("while iteration" );
			curBlock = m_WaveQueue.front();
			UpdateWave(CornerGlobal, curBlock);
			m_WaveQueue.pop();

			bContinue = ( (!m_WaveQueue.empty()) && (m_WayLength[m_WaveQueue.front().x][m_WaveQueue.front().z] < m_NearestHole) );
		}

		if (m_CurResult & E_SIDE_LEFT)  Points.push_back(Vector3i(a_BlockX - 1, a_BlockY, a_BlockZ));
		if (m_CurResult & E_SIDE_RIGHT) Points.push_back(Vector3i(a_BlockX + 1, a_BlockY, a_BlockZ));
		if (m_CurResult & E_SIDE_FRONT) Points.push_back(Vector3i(a_BlockX,     a_BlockY, a_BlockZ + 1));
		if (m_CurResult & E_SIDE_BACK)  Points.push_back(Vector3i(a_BlockX,     a_BlockY, a_BlockZ - 1));

		if (Points.empty())
		{
			Vector3i LevelPoints [] = {
				Vector3i(a_BlockX - 1, a_BlockY, a_BlockZ),
				Vector3i(a_BlockX + 1, a_BlockY, a_BlockZ),
				Vector3i(a_BlockX,     a_BlockY, a_BlockZ - 1),
				Vector3i(a_BlockX,     a_BlockY, a_BlockZ + 1),
			};
			for (int i = 0; i < 4; ++i)
			{
				char Block = m_World->GetBlock(LevelPoints[i].x, a_BlockY, LevelPoints[i].z);
				if (m_Simulator->IsPassableForFluid(Block))
				{
					Points.push_back(LevelPoints[i]);
				}
			}
		}

		return Points;
	}

	std::set< Vector3i > * m_ActiveFluid;
	std::set< Vector3i > * m_Buffer;
	cWorld * m_World;
	cClassicFluidSimulator * m_Simulator;

	const static int AREA_WIDTH = 11; 

	const static unsigned char E_SIDE_RIGHT = 0x10;
	const static unsigned char E_SIDE_LEFT = 0x20;
	const static unsigned char E_SIDE_FRONT = 0x40;
	const static unsigned char E_SIDE_BACK = 0x80;
	const static unsigned char E_SIDE_NONE = 0x00;

	enum eRelief
	{
		E_HOLE = 0,
		E_PLAIN = 1,
		E_BLOCK = 2
	};

	eRelief m_Relief[AREA_WIDTH][AREA_WIDTH];
	unsigned char m_WayLength[AREA_WIDTH][AREA_WIDTH];
	unsigned char m_StartSide[AREA_WIDTH][AREA_WIDTH];

	std::queue<Vector3i> m_WaveQueue;

	int m_NearestHole;
	unsigned char m_CurResult;
};





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cClassicFluidSimulator:

cClassicFluidSimulator::cClassicFluidSimulator(cWorld * a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, NIBBLETYPE a_MaxHeight, NIBBLETYPE a_Falloff) :
	cFluidSimulator(a_World, a_Fluid, a_StationaryFluid),
	m_Data(NULL),
	m_MaxHeight(a_MaxHeight),
	m_Falloff(a_Falloff)
{
	m_Data = new FluidData(a_World, this);
}





cClassicFluidSimulator::~cClassicFluidSimulator()
{
	delete m_Data;
}





void cClassicFluidSimulator::AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	BLOCKTYPE BlockType = m_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	if (!IsAllowedBlock(BlockType))		// This should save very much time because it doesn´t have to iterate through all blocks
	{
		return;
	}

	std::set< Vector3i > & ActiveFluid = *m_Data->m_ActiveFluid;
	ActiveFluid.insert(Vector3i(a_BlockX, a_BlockY, a_BlockZ));
}





bool cClassicFluidSimulator::IsAllowedBlock(BLOCKTYPE a_BlockType)
{
	return ((a_BlockType == m_FluidBlock) || (a_BlockType == m_StationaryFluidBlock));
}





NIBBLETYPE cClassicFluidSimulator::GetHighestLevelAround(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE Max = m_MaxHeight + m_Falloff;

#define __HIGHLEVEL_CHECK__( x, y, z ) \
	if (IsAllowedBlock(m_World->GetBlock( x, y, z ) ) ) \
	{ \
		NIBBLETYPE Meta; \
		if ((Meta = m_World->GetBlockMeta( x, y, z ) ) < Max ) Max = Meta; \
		else if (Meta == m_MaxHeight + m_Falloff) Max = 0; \
		if (Max == 0) return 0; \
	}

	__HIGHLEVEL_CHECK__(a_BlockX - 1, a_BlockY, a_BlockZ );
	__HIGHLEVEL_CHECK__(a_BlockX + 1, a_BlockY, a_BlockZ );
	__HIGHLEVEL_CHECK__(a_BlockX,     a_BlockY, a_BlockZ - 1);
	__HIGHLEVEL_CHECK__(a_BlockX,     a_BlockY, a_BlockZ + 1);

	return Max;
}





void cClassicFluidSimulator::Simulate(float a_Dt)
{
	m_Timer += a_Dt;

	if (m_Data->m_ActiveFluid->empty()) 	// Nothing to do if there is no active fluid ;) saves very little time ;D
	{
		return;
	}

	std::swap( m_Data->m_ActiveFluid, m_Data->m_Buffer );	// Swap so blocks can be added to empty ActiveFluid array
	m_Data->m_ActiveFluid->clear();

	std::set< Vector3i > & FluidBlocks = *m_Data->m_Buffer;
	for( std::set< Vector3i >::iterator itr = FluidBlocks.begin(); itr != FluidBlocks.end(); ++itr )
	{
		const Vector3i & pos = *itr;

		if(UniqueSituation(pos))
		{
			continue;
		}
		
		char BlockID = m_World->GetBlock( pos.x, pos.y, pos.z );
		if( IsAllowedBlock( BlockID ) ) // only care about own fluid
		{
			bool bIsFed = false;
			NIBBLETYPE Meta = m_World->GetBlockMeta( pos.x, pos.y, pos.z );
			NIBBLETYPE Feed = Meta;
			if (BlockID == m_StationaryFluidBlock) Meta = 0;
			if (Meta == 8 ) // Falling fluid
			{
				if (IsAllowedBlock( m_World->GetBlock(pos.x, pos.y+1, pos.z) ) )	// Block above is fluid
				{
					bIsFed = true;
					Meta = 0;	// Make it a full block
				}
			}
			else if (Meta < m_Falloff) // It's a full block, so it's always fed
			{
				bIsFed = true;
			}
			else
			{
				if ((Feed = GetHighestLevelAround( pos.x, pos.y, pos.z )) < Meta)
					bIsFed = true;
			}


			if( bIsFed )
			{
				char DownID = m_World->GetBlock(pos.x, pos.y - 1, pos.z);
				bool bWashedAwayItem = CanWashAway(DownID);
				if ((IsPassableForFluid(DownID) || bWashedAwayItem) && !IsStationaryFluidBlock(DownID) ) // free for fluid 
				{
					if (bWashedAwayItem)
					{
						cBlockHandler * Handler = BlockHandler(DownID);
						if (Handler->DoesDropOnUnsuitable())
						{
							Handler->DropBlock(m_World, pos.x, pos.y - 1, pos.z);
						}
					}
					if (pos.y > 0)
					{
						m_World->FastSetBlock( pos.x, pos.y-1, pos.z, m_FluidBlock, 8 ); // falling
						AddBlock( pos.x, pos.y-1, pos.z );
						ApplyUniqueToNearest(pos - Vector3i(0, 1, 0));
					}
				}
				if (IsSolidBlock(DownID)||( BlockID == m_StationaryFluidBlock)) // Not falling
				{
					if (Feed + m_Falloff < Meta)
					{
						m_World->FastSetBlock( pos.x, pos.y, pos.z, m_FluidBlock, Feed + m_Falloff);
						AddBlock( pos.x, pos.y, pos.z );
						ApplyUniqueToNearest(pos);
					}
					else if ((Meta < m_MaxHeight ) || (BlockID == m_StationaryFluidBlock)) // max is the lowest, so it cannot spread					
					{
						std::vector< Vector3i > Points = m_Data->GetLowestPoints( pos.x, pos.y, pos.z );
						for( std::vector< Vector3i >::iterator itr = Points.begin(); itr != Points.end(); ++itr )
						{
							Vector3i & p = *itr;
							char BlockID = m_World->GetBlock( p.x, p.y, p.z );
							bool bWashedAwayItem = CanWashAway( BlockID );

							if (!IsPassableForFluid(BlockID)) continue;

							if (!IsAllowedBlock(BlockID))
							{
								if (bWashedAwayItem)
								{
									cBlockHandler * Handler = BlockHandler(DownID);
									if (Handler->DoesDropOnUnsuitable())
									{
										Handler->DropBlock(m_World, p.x, p.y, p.z);
									}
								}

								if (p.y == pos.y)
								{
									m_World->FastSetBlock(p.x, p.y, p.z, m_FluidBlock, Meta + m_Falloff);
								}
								else
								{
									m_World->FastSetBlock(p.x, p.y, p.z, m_FluidBlock, 8);
								}
								AddBlock( p.x, p.y, p.z );
								ApplyUniqueToNearest(p);
							}
							else // it's fluid
							{
								char PointMeta = m_World->GetBlockMeta( p.x, p.y, p.z );
								if( PointMeta > Meta + m_Falloff)
								{
									AddBlock( p.x, p.y, p.z );
									ApplyUniqueToNearest(p);
								}
							}
						}
					}
				}
			}
			else// not fed
			{
				m_World->FastSetBlock( pos.x, pos.y, pos.z, E_BLOCK_AIR, 0 );
				WakeUp( pos.x, pos.y, pos.z );
			}
		}
	}
}





bool cClassicFluidSimulator::IsPassableForFluid(BLOCKTYPE a_BlockType)
{
	return a_BlockType == E_BLOCK_AIR
		|| a_BlockType == E_BLOCK_FIRE
		|| IsAllowedBlock(a_BlockType)
		|| CanWashAway(a_BlockType);
}





bool cClassicFluidSimulator::CanWashAway(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_YELLOW_FLOWER:
		case E_BLOCK_RED_ROSE:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_CACTUS:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool cClassicFluidSimulator::IsSolidBlock( BLOCKTYPE a_BlockType )
{
	return !(a_BlockType == E_BLOCK_AIR
		|| a_BlockType == E_BLOCK_FIRE
		|| IsBlockLava(a_BlockType)
		|| IsBlockWater(a_BlockType)
		|| CanWashAway(a_BlockType));
}





// TODO Not working very well yet :s
Direction cClassicFluidSimulator::GetFlowingDirection(int a_X, int a_Y, int a_Z, bool a_Over)
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

	Points.reserve(4);	//Already allocate 4 places :D

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





bool cClassicFluidSimulator::UniqueSituation(Vector3i a_Pos)
{
	bool result = false;

	char BlockId = m_World->GetBlock( a_Pos.x, a_Pos.y, a_Pos.z );
	char Meta = m_World->GetBlockMeta( a_Pos.x, a_Pos.y, a_Pos.z );

	if(IsBlockWater(BlockId))
	{

		char UpperBlock = m_World->GetBlock( a_Pos.x, a_Pos.y + 1, a_Pos.z );
		if(IsBlockLava(UpperBlock))
		{
			m_World->SetBlock(a_Pos.x, a_Pos.y, a_Pos.z, E_BLOCK_STONE, 0);
		}

	
		if(BlockId != E_BLOCK_STATIONARY_WATER)
		{
			char DownBlockId = m_World->GetBlock( a_Pos.x, a_Pos.y-1, a_Pos.z );
			if(IsSolidBlock(DownBlockId))
			{
				Vector3i LevelPoints [] = {
						Vector3i( a_Pos.x-1, a_Pos.y, a_Pos.z ),
						Vector3i( a_Pos.x+1, a_Pos.y, a_Pos.z ),
						Vector3i( a_Pos.x, a_Pos.y, a_Pos.z-1 ),
						Vector3i( a_Pos.x, a_Pos.y, a_Pos.z+1 ),
				};
				int SourceBlocksCount = 0;
				for(int i=0; i<4; i++)
				{
					if (m_World->GetBlock(LevelPoints[i].x, LevelPoints[i].y, LevelPoints[i].z)==E_BLOCK_STATIONARY_WATER)
					{
						SourceBlocksCount++;
					}
				}
				if(SourceBlocksCount>=2)
				{
					m_World->SetBlock(a_Pos.x, a_Pos.y, a_Pos.z, E_BLOCK_STATIONARY_WATER, 0);
				}
			}

		}
	}

	if(IsBlockLava(BlockId))
	{
		bool bWater = false;

		char UpperBlock = m_World->GetBlock( a_Pos.x, a_Pos.y + 1, a_Pos.z );
		if (IsBlockWater(UpperBlock))
		{
			bWater = true;
		}
		else
		{
			Vector3i LevelPoints [] = {
					Vector3i( a_Pos.x-1, a_Pos.y, a_Pos.z ),
					Vector3i( a_Pos.x+1, a_Pos.y, a_Pos.z ),
					Vector3i( a_Pos.x, a_Pos.y, a_Pos.z-1 ),
					Vector3i( a_Pos.x, a_Pos.y, a_Pos.z+1 ),
			};

			for(int i=0; i<4; i++)
			{
				if (IsBlockWater(m_World->GetBlock(LevelPoints[i].x, LevelPoints[i].y, LevelPoints[i].z)))
				{
					bWater = true;
				}
			}
		}


		if (bWater)
		{
			if (BlockId == E_BLOCK_STATIONARY_LAVA)
			{
				m_World->SetBlock(a_Pos.x, a_Pos.y, a_Pos.z, E_BLOCK_OBSIDIAN, 0);
			}
			else if (Meta<m_MaxHeight)
			{
				m_World->SetBlock(a_Pos.x, a_Pos.y, a_Pos.z, E_BLOCK_COBBLESTONE, 0);
			}
		}
	}

	return result;
}





void cClassicFluidSimulator::ApplyUniqueToNearest(Vector3i a_Pos)
{
	Vector3i NearPoints [] = {
		Vector3i( a_Pos.x-1, a_Pos.y, a_Pos.z ),
		Vector3i( a_Pos.x+1, a_Pos.y, a_Pos.z ),
		Vector3i( a_Pos.x, a_Pos.y, a_Pos.z-1 ),
		Vector3i( a_Pos.x, a_Pos.y, a_Pos.z+1 ),
		Vector3i( a_Pos.x, a_Pos.y-1, a_Pos.z )
	};

	for (int i = 0; i < ARRAYCOUNT(NearPoints); i++)
	{
		UniqueSituation(NearPoints[i]);
	}
}




