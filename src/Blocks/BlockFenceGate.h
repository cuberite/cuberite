
#pragma once

#include "BlockHandler.h"
#include "MetaRotator.h"
#include "../EffectID.h"




class cBlockFenceGateHandler :
	public cMetaRotator<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01, true>
{
public:
	cBlockFenceGateHandler(BLOCKTYPE a_BlockType) :
		cMetaRotator<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01, true>(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.Add(m_BlockType, 1, 0);  // Reset meta to zero
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = PlayerYawToMetaData(a_Player.GetYaw());
		return true;
	}

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE OldMetaData = a_ChunkInterface.GetBlockMeta({a_BlockX, a_BlockY, a_BlockZ});
		NIBBLETYPE NewMetaData = PlayerYawToMetaData(a_Player.GetYaw());
		OldMetaData ^= 4;  // Toggle the gate

		if ((OldMetaData & 1) == (NewMetaData & 1))
		{
			// Standing in front of the gate - apply new direction
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, (OldMetaData & 4) | (NewMetaData & 3));
		}
		else
		{
			// Standing aside - use last direction
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, OldMetaData);
		}
		a_Player.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_FENCE_GATE_OPEN, a_BlockX, a_BlockY, a_BlockZ, 0, a_Player.GetClientHandle());
		return true;
	}

	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
	}

	virtual bool IsUseable(void) override
	{
		return true;
	}

	/** Converts the player's yaw to placed gate's blockmeta */
	inline static NIBBLETYPE PlayerYawToMetaData(double a_Yaw)
	{
		ASSERT((a_Yaw >= -180) && (a_Yaw < 180));

		a_Yaw += 360 + 45;
		if (a_Yaw > 360)
		{
			a_Yaw -= 360;
		}
		if ((a_Yaw >= 0) && (a_Yaw < 90))
		{
			return 0x0;
		}
		else if ((a_Yaw >= 180) && (a_Yaw < 270))
		{
			return 0x2;
		}
		else if ((a_Yaw >= 90) && (a_Yaw < 180))
		{
			return 0x1;
		}
		else
		{
			return 0x3;
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		switch (m_BlockType)
		{
			case E_BLOCK_OAK_FENCE_GATE: return 13;
			case E_BLOCK_SPRUCE_FENCE_GATE: return 34;
			case E_BLOCK_BIRCH_FENCE_GATE: return 2;
			case E_BLOCK_JUNGLE_FENCE_GATE: return 10;
			case E_BLOCK_DARK_OAK_FENCE_GATE: return 26;
			case E_BLOCK_ACACIA_FENCE_GATE: return 15;
			default:
			{
				ASSERT(!"Unhandled blocktype in fence gate handler!");
				return 0;
			}
		}
	}
} ;




