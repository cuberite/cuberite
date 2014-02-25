
#pragma once

#include "BlockEntity.h"
#include "../BlockEntities/MobHeadEntity.h"





class cBlockMobHeadHandler :
	public cBlockEntityHandler
{
public:
	cBlockMobHeadHandler(BLOCKTYPE a_BlockType)
		: cBlockEntityHandler(a_BlockType)
	{
	}
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_HEAD, 1, 0));
	}
	
	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, 
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) override
	{
		class cCallback : public cMobHeadBlockCallback
		{
			cPlayer * m_Player;
			NIBBLETYPE m_OldBlockMeta;
			NIBBLETYPE m_NewBlockMeta;
			
			virtual bool Item (cMobHeadEntity * a_MobHeadEntity)
			{
				int Rotation = 0;
				if (m_NewBlockMeta == 1)
				{
					Rotation = (int) floor(m_Player->GetYaw() * 16.0F / 360.0F + 0.5) & 0xF;
				}
				
				a_MobHeadEntity->SetType(static_cast<eMobHeadType>(m_OldBlockMeta));
				a_MobHeadEntity->SetRotation(static_cast<eMobHeadRotation>(Rotation));
				return false;
			}
		
		public:
			cCallback (cPlayer * a_Player, NIBBLETYPE a_OldBlockMeta, NIBBLETYPE a_NewBlockMeta) :
				m_Player(a_Player),
				m_OldBlockMeta(a_OldBlockMeta),
				m_NewBlockMeta(a_NewBlockMeta)
			{}
		};
		cCallback Callback(a_Player, a_BlockMeta, static_cast<NIBBLETYPE>(a_BlockFace));
		
		a_BlockMeta = a_BlockFace;
		cWorld * World = (cWorld *) &a_WorldInterface;
		World->DoWithMobHeadBlockAt(a_BlockX, a_BlockY, a_BlockZ, Callback);
		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, a_BlockMeta);
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
		case 0x02: return 0x04;  // North -> East
		case 0x04: return 0x03;  // East  -> South
		case 0x03: return 0x05;  // South -> West
		case 0x05: return 0x02;  // West  -> North
		}
		// Not reachable, but to avoid a compiler warning:
		return a_Meta;
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
		case 0x02: return 0x05;  // North -> West
		case 0x05: return 0x03;  // West  -> South
		case 0x03: return 0x04;  // South -> East
		case 0x04: return 0x02;  // East  -> North
		}
		// Not reachable, but to avoid a compiler warning:
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
		case 0x02: return 0x03;  // North -> South
		case 0x03: return 0x02;  // South -> North
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Mirrors over YZ-plane (East/West).
		switch (a_Meta)
		{
		case 0x05: return 0x04;  // West -> East
		case 0x04: return 0x05;  // East -> West
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}
} ;




