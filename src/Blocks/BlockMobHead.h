
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
		class cCallback : public cMobHeadCallback
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
				a_MobHeadEntity->GetWorld()->BroadcastBlockEntity(a_MobHeadEntity->GetPosX(), a_MobHeadEntity->GetPosY(), a_MobHeadEntity->GetPosZ(), m_Player->GetClientHandle());
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
		World->DoWithMobHeadAt(a_BlockX, a_BlockY, a_BlockZ, Callback);
		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, a_BlockMeta);
	}
} ;




