
#pragma once

#include "BlockEntity.h"
#include "../World.h"





enum ENUM_RAIL_DIRECTIONS
{
	E_RAIL_NORTH_SOUTH = 0,
	E_RAIL_EAST_WEST = 1,
	E_RAIL_ASCEND_EAST = 2,
	E_RAIL_ASCEND_WEST = 3,
	E_RAIL_ASCEND_NORTH = 4,
	E_RAIL_ASCEND_SOUTH = 5,
	E_RAIL_CURVED_SOUTH_EAST = 6,
	E_RAIL_CURVED_SOUTH_WEST = 7,
	E_RAIL_CURVED_NORTH_WEST = 8,
	E_RAIL_CURVED_NORTH_EAST = 9
};

enum ENUM_PURE
{
	E_PURE_UPDOWN = 0,
	E_PURE_DOWN = 1,
	E_PURE_NONE = 2
};





class cBlockRailHandler :
	public cBlockHandler
{
public:
	cBlockRailHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, FindMeta(a_World, a_BlockX, a_BlockY, a_BlockZ));
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
		NeighborChanged(a_World, a_BlockX - 1, a_BlockY + 1, a_BlockZ);
		NeighborChanged(a_World, a_BlockX + 1, a_BlockY + 1, a_BlockZ);
		NeighborChanged(a_World, a_BlockX, a_BlockY + 1, a_BlockZ - 1);
		NeighborChanged(a_World, a_BlockX, a_BlockY + 1, a_BlockZ + 1);
		NeighborChanged(a_World, a_BlockX - 1, a_BlockY - 1, a_BlockZ);
		NeighborChanged(a_World, a_BlockX + 1, a_BlockY - 1, a_BlockZ);
		NeighborChanged(a_World, a_BlockX, a_BlockY - 1, a_BlockZ - 1);
		NeighborChanged(a_World, a_BlockX, a_BlockY - 1, a_BlockZ + 1);
	}

	virtual void OnNeighborChanged(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		if(IsUnstable(a_World, a_BlockX, a_BlockY, a_BlockZ))
			a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, FindMeta(a_World, a_BlockX, a_BlockY, a_BlockZ));
	}

	virtual bool CanBeAt(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		if(!g_BlockIsSolid[a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ)])
			return false;
		char Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		switch(Meta)
		{
			case E_RAIL_ASCEND_EAST:
			{
				if(!g_BlockIsSolid[a_World->GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ)])
					return false;
				break;
			}
			case E_RAIL_ASCEND_WEST:
			{
				if(!g_BlockIsSolid[a_World->GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ)])
					return false;
				break;
			}
			case E_RAIL_ASCEND_NORTH:
			{
				if(!g_BlockIsSolid[a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ - 1)])
					return false;
				break;
			}
			case E_RAIL_ASCEND_SOUTH:
			{
				if(!g_BlockIsSolid[a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ + 1)])
					return false;
				break;
			}
		}
		return true;
	}

	char FindMeta(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		char Meta = 0;
		char RailsCnt = 0;
		bool Neighbors[8]; // 0 - EAST, 1 - WEST, 2 - NORTH, 3 - SOUTH, 4 - EAST UP, 5 - WEST UP, 6 - NORTH UP, 7 - SOUTH UP
		memset(Neighbors, false, sizeof(Neighbors));
		if(IsUnstable(a_World, a_BlockX + 1, a_BlockY, a_BlockZ) || !IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_EAST, E_PURE_DOWN))
			Neighbors[0] = true;
		if(IsUnstable(a_World, a_BlockX - 1, a_BlockY, a_BlockZ) || !IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_WEST, E_PURE_DOWN))
			Neighbors[1] = true;
		if(IsUnstable(a_World, a_BlockX, a_BlockY, a_BlockZ - 1) || !IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_NORTH, E_PURE_DOWN))
			Neighbors[2] = true;
		if(IsUnstable(a_World, a_BlockX, a_BlockY, a_BlockZ + 1) || !IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_SOUTH, E_PURE_DOWN))
			Neighbors[3] = true;
		if(IsUnstable(a_World, a_BlockX + 1, a_BlockY + 1, a_BlockZ) || !IsNotConnected(a_World, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_EAST, E_PURE_NONE))
			Neighbors[4] = true;
		if(IsUnstable(a_World, a_BlockX - 1, a_BlockY + 1, a_BlockZ) || !IsNotConnected(a_World, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_WEST, E_PURE_NONE))
			Neighbors[5] = true;
		if(IsUnstable(a_World, a_BlockX, a_BlockY + 1, a_BlockZ - 1) || !IsNotConnected(a_World, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_NORTH, E_PURE_NONE))
			Neighbors[6] = true;
		if(IsUnstable(a_World, a_BlockX, a_BlockY + 1, a_BlockZ + 1) || !IsNotConnected(a_World, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_SOUTH, E_PURE_NONE))
			Neighbors[7] = true;
		if(IsUnstable(a_World, a_BlockX + 1, a_BlockY - 1, a_BlockZ) || !IsNotConnected(a_World, a_BlockX, a_BlockY - 1, a_BlockZ, BLOCK_FACE_EAST))
			Neighbors[0] = true;
		if(IsUnstable(a_World, a_BlockX - 1, a_BlockY - 1, a_BlockZ) || !IsNotConnected(a_World, a_BlockX, a_BlockY - 1, a_BlockZ, BLOCK_FACE_WEST))
			Neighbors[1] = true;
		if(IsUnstable(a_World, a_BlockX, a_BlockY - 1, a_BlockZ - 1) || !IsNotConnected(a_World, a_BlockX, a_BlockY - 1, a_BlockZ, BLOCK_FACE_NORTH))
			Neighbors[2] = true;
		if(IsUnstable(a_World, a_BlockX, a_BlockY - 1, a_BlockZ + 1) || !IsNotConnected(a_World, a_BlockX, a_BlockY - 1, a_BlockZ, BLOCK_FACE_SOUTH))
			Neighbors[3] = true;
		for(int i = 0; i < 8; i++)
		{
			if(Neighbors[i])
			{
				RailsCnt++;
			}
		}
		if(RailsCnt == 1)
		{
			if(Neighbors[7]) Meta = E_RAIL_ASCEND_SOUTH;
			else if(Neighbors[6]) Meta = E_RAIL_ASCEND_NORTH;
			else if(Neighbors[5]) Meta = E_RAIL_ASCEND_WEST;
			else if(Neighbors[4]) Meta = E_RAIL_ASCEND_EAST;
			else if(Neighbors[0] || Neighbors[1]) Meta = E_RAIL_EAST_WEST;
			else if(Neighbors[2] || Neighbors[3]) Meta = E_RAIL_NORTH_SOUTH;
			return Meta;
		}
		for(int i = 0; i < 4; i++)
		{
			if(Neighbors[i+4])
			{
				Neighbors[i] = true;
			}
		}
		if(RailsCnt > 1)
		{
			if(Neighbors[3] && Neighbors[0]) Meta = E_RAIL_CURVED_SOUTH_EAST;
			else if(Neighbors[3] && Neighbors[1]) Meta = E_RAIL_CURVED_SOUTH_WEST;
			else if(Neighbors[2] && Neighbors[0]) Meta = E_RAIL_CURVED_NORTH_EAST;
			else if(Neighbors[2] && Neighbors[1]) Meta = E_RAIL_CURVED_NORTH_WEST;
			else if(Neighbors[7] && Neighbors[2]) Meta = E_RAIL_ASCEND_SOUTH;
			else if(Neighbors[3] && Neighbors[6]) Meta = E_RAIL_ASCEND_NORTH;
			else if(Neighbors[5] && Neighbors[0]) Meta = E_RAIL_ASCEND_WEST;
			else if(Neighbors[4] && Neighbors[1]) Meta = E_RAIL_ASCEND_EAST;
			else if(Neighbors[0] && Neighbors[1]) Meta = E_RAIL_EAST_WEST;
			else if(Neighbors[2] && Neighbors[3]) Meta = E_RAIL_NORTH_SOUTH;
		}
		return Meta;
	}

	bool IsUnstable(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		if(a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) != E_BLOCK_RAIL) return false;
		char Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		switch(Meta)
		{
			case E_RAIL_NORTH_SOUTH:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_NORTH, E_PURE_DOWN) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_SOUTH, E_PURE_DOWN)) return true;
				break;
			}
			case E_RAIL_EAST_WEST:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_EAST, E_PURE_DOWN) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_WEST, E_PURE_DOWN)) return true;
				break;
			}
			case E_RAIL_ASCEND_EAST:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_EAST) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_WEST)) return true;
				break;
			}
			case E_RAIL_ASCEND_WEST:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_EAST) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_WEST)) return true;
				break;
			}
			case E_RAIL_ASCEND_NORTH:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_NORTH) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_SOUTH)) return true;
				break;
			}
			case E_RAIL_ASCEND_SOUTH:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_NORTH) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_SOUTH)) return true;
				break;
			}
			case E_RAIL_CURVED_SOUTH_EAST:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_SOUTH) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_EAST)) return true;
				break;
			}
			case E_RAIL_CURVED_SOUTH_WEST:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_SOUTH) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_WEST)) return true;
				break;
			}
			case E_RAIL_CURVED_NORTH_WEST:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_NORTH) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_WEST)) return true;
				break;
			}
			case E_RAIL_CURVED_NORTH_EAST:
			{
				if(IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_NORTH) || 
				   IsNotConnected(a_World, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_EAST)) return true;
				break;
			}
		}
		return false;
	}

	bool IsNotConnected(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Direction, char a_Pure = 0)
	{
		AddDirection(a_BlockX, a_BlockY, a_BlockZ, a_Direction, false);
		char Meta;
		if(a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) != E_BLOCK_RAIL)
		{
			if((a_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ) != E_BLOCK_RAIL) || (a_Pure != E_PURE_UPDOWN))
			{
				if((a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ) != E_BLOCK_RAIL) || (a_Pure == E_PURE_NONE))
				{
					return true;
				}
				else
				{
					Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ);
				}
			}
			else
			{
				Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY + 1, a_BlockZ);
			}
		}
		else
		{
			Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		}
		switch(a_Direction)
		{
			case BLOCK_FACE_NORTH:
			{
				if(Meta == E_RAIL_NORTH_SOUTH ||
				   Meta == E_RAIL_ASCEND_NORTH ||
				   Meta == E_RAIL_ASCEND_SOUTH ||
				   Meta == E_RAIL_CURVED_SOUTH_EAST ||
				   Meta == E_RAIL_CURVED_SOUTH_WEST)
				   return false;
				break;
			}
			case BLOCK_FACE_SOUTH:
			{
				if(Meta == E_RAIL_NORTH_SOUTH ||
				   Meta == E_RAIL_ASCEND_NORTH ||
				   Meta == E_RAIL_ASCEND_SOUTH ||
				   Meta == E_RAIL_CURVED_NORTH_EAST ||
				   Meta == E_RAIL_CURVED_NORTH_WEST)
				   return false;
				break;
			}
			case BLOCK_FACE_EAST:
			{
				if(Meta == E_RAIL_EAST_WEST ||
				   Meta == E_RAIL_ASCEND_EAST ||
				   Meta == E_RAIL_ASCEND_WEST ||
				   Meta == E_RAIL_CURVED_SOUTH_WEST ||
				   Meta == E_RAIL_CURVED_NORTH_WEST)
				   return false;
				break;
			}
			case BLOCK_FACE_WEST:
			{
				if(Meta == E_RAIL_EAST_WEST ||
				   Meta == E_RAIL_ASCEND_EAST ||
				   Meta == E_RAIL_ASCEND_WEST ||
				   Meta == E_RAIL_CURVED_SOUTH_EAST ||
				   Meta == E_RAIL_CURVED_NORTH_EAST)
				   return false;
				break;
			}
		}
		return true;
	}
} ;




