
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

	bool TrySpawnWither(cChunkInterface & a_ChunkInterface, cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		if (a_BlockY < 2)
		{
			return false;
		}

		class cCallback : public cMobHeadCallback
		{
			bool m_IsWither;
			
			virtual bool Item (cMobHeadEntity * a_MobHeadEntity)
			{
				m_IsWither = (a_MobHeadEntity->GetType() == SKULL_TYPE_WITHER);

				return false;
			}
		
		public:
			cCallback () : m_IsWither(false) {}

			bool IsWither(void) const { return m_IsWither; }

			void Reset(void) { m_IsWither = false; }

		} CallbackA, CallbackB;

		class cPlayerCallback : public cPlayerListCallback
		{
			Vector3f m_Pos;

			virtual bool Item(cPlayer * a_Player)
			{
				// TODO 2014-05-21 xdot: Vanilla minecraft uses an AABB check instead of a radius one
				double Dist = (a_Player->GetPosition() - m_Pos).Length();
				if (Dist < 50.0)
				{
					// If player is close, award achievement
					a_Player->AwardAchievement(achSpawnWither);
				}
				return false;
			}

		public:
			cPlayerCallback(const Vector3f & a_Pos) : m_Pos(a_Pos) {}

		} PlayerCallback(Vector3f(a_BlockX, a_BlockY, a_BlockZ));

		a_World->DoWithMobHeadAt(a_BlockX, a_BlockY, a_BlockZ, CallbackA);

		if (!CallbackA.IsWither())
		{
			return false;
		}

		CallbackA.Reset();
		
		BLOCKTYPE BlockY1 = a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);
		BLOCKTYPE BlockY2 = a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 2, a_BlockZ);

		if ((BlockY1 != E_BLOCK_SOULSAND) || (BlockY2 != E_BLOCK_SOULSAND))
		{
			return false;
		}
		
		a_World->DoWithMobHeadAt(a_BlockX - 1, a_BlockY, a_BlockZ, CallbackA);
		a_World->DoWithMobHeadAt(a_BlockX + 1, a_BlockY, a_BlockZ, CallbackB);

		BLOCKTYPE Block1 = a_ChunkInterface.GetBlock(a_BlockX - 1, a_BlockY - 1, a_BlockZ);
		BLOCKTYPE Block2 = a_ChunkInterface.GetBlock(a_BlockX + 1, a_BlockY - 1, a_BlockZ);

		if ((Block1 == E_BLOCK_SOULSAND) && (Block2 == E_BLOCK_SOULSAND) && CallbackA.IsWither() && CallbackB.IsWither())
		{
			a_ChunkInterface.FastSetBlock(a_BlockX,     a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX + 1, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX - 1, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX,     a_BlockY - 2, a_BlockZ, E_BLOCK_AIR, 0);

			// Block entities
			a_World->SetBlock(a_BlockX + 1, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			a_World->SetBlock(a_BlockX,     a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			a_World->SetBlock(a_BlockX - 1, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);

			// Spawn the wither:
			a_World->SpawnMob(a_BlockX + 0.5, a_BlockY - 2, a_BlockZ + 0.5, cMonster::mtWither);

			// Award Achievement
			a_World->ForEachPlayer(PlayerCallback);

			return true;
		}

		CallbackA.Reset();
		CallbackB.Reset();
		
		a_World->DoWithMobHeadAt(a_BlockX, a_BlockY, a_BlockZ - 1, CallbackA);
		a_World->DoWithMobHeadAt(a_BlockX, a_BlockY, a_BlockZ + 1, CallbackB);

		Block1 = a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ - 1);
		Block2 = a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ + 1);

		if ((Block1 == E_BLOCK_SOULSAND) && (Block2 == E_BLOCK_SOULSAND) && CallbackA.IsWither() && CallbackB.IsWither())
		{
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ,     E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ + 1, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ - 1, E_BLOCK_AIR, 0);
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 2, a_BlockZ,     E_BLOCK_AIR, 0);

			// Block entities
			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ + 1, E_BLOCK_AIR, 0);
			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ,     E_BLOCK_AIR, 0);
			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ - 1, E_BLOCK_AIR, 0);

			// Spawn the wither:
			a_World->SpawnMob(a_BlockX + 0.5, a_BlockY - 2, a_BlockZ + 0.5, cMonster::mtWither);

			// Award Achievement
			a_World->ForEachPlayer(PlayerCallback);

			return true;
		}

		return false;
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
			cCallback (cPlayer * a_CBPlayer, NIBBLETYPE a_OldBlockMeta, NIBBLETYPE a_NewBlockMeta) :
				m_Player(a_CBPlayer),
				m_OldBlockMeta(a_OldBlockMeta),
				m_NewBlockMeta(a_NewBlockMeta)
			{}
		};
		cCallback Callback(a_Player, a_BlockMeta, static_cast<NIBBLETYPE>(a_BlockFace));
		
		a_BlockMeta = (NIBBLETYPE)a_BlockFace;
		cWorld * World = (cWorld *) &a_WorldInterface;
		World->DoWithMobHeadAt(a_BlockX, a_BlockY, a_BlockZ, Callback);
		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, a_BlockMeta);

		if (a_BlockMeta == SKULL_TYPE_WITHER)
		{
			static const Vector3i Coords[] =
			{
				Vector3i( 0, 0,  0),
				Vector3i( 1, 0,  0),
				Vector3i(-1, 0,  0),
				Vector3i( 0, 0,  1),
				Vector3i( 0, 0, -1),
			};
			for (size_t i = 0; i < ARRAYCOUNT(Coords); ++i)
			{
				if (TrySpawnWither(a_ChunkInterface, World, a_BlockX + Coords[i].x, a_BlockY, a_BlockZ + Coords[i].z))
				{
					break;
				}
			}  // for i - Coords[]
		}
	}
} ;




