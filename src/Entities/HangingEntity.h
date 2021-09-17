
#pragma once

#include "Entity.h"





// tolua_begin
class cHangingEntity :
	public cEntity
{
	// tolua_end

	using Super = cEntity;

public:  // tolua_export

	CLASS_PROTODEF(cHangingEntity)

	cHangingEntity(eEntityType a_EntityType, eBlockFace a_BlockFace, Vector3d a_Pos);

	// tolua_begin

	/** Returns the direction in which the entity is facing. */
	eBlockFace GetFacing() const { return cHangingEntity::ProtocolFaceToBlockFace(m_Facing); }

	/** Set the direction in which the entity is facing. */
	void SetFacing(eBlockFace a_Facing)
	{
		m_Facing = cHangingEntity::BlockFaceToProtocolFace(a_Facing);
	}

	inline static bool ValidSupportBlock(BLOCKTYPE BlockType)
	{
		switch (BlockType)
		{
			case E_BLOCK_AIR:
			case E_BLOCK_RAIL:
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_DETECTOR_RAIL:
			case E_BLOCK_POWERED_RAIL:
			case E_BLOCK_LEVER:
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_WOODEN_BUTTON:
			case E_BLOCK_TRIPWIRE:
			case E_BLOCK_TRIPWIRE_HOOK:
			case E_BLOCK_REDSTONE_WIRE:
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_TORCH:
			case E_BLOCK_SAPLING:
			case E_BLOCK_DEAD_BUSH:
			case E_BLOCK_TALL_GRASS:
			case E_BLOCK_BROWN_MUSHROOM:
			case E_BLOCK_RED_MUSHROOM:
			case E_BLOCK_CARPET:
			case E_BLOCK_HEAD:
			case E_BLOCK_FLOWER_POT:
			case E_BLOCK_FLOWER:
			case E_BLOCK_BIG_FLOWER:
			case E_BLOCK_YELLOW_FLOWER:
			case E_BLOCK_BREWING_STAND:
			case E_BLOCK_END_ROD:
			case E_BLOCK_LADDER:
			case E_BLOCK_END_PORTAL:
			case E_BLOCK_NETHER_PORTAL:
				return false;
			default:
			{
				return true;
			}
		}
	}

	// tolua_end

	/** Returns the direction in which the entity is facing. */
	Byte GetProtocolFacing() const { return m_Facing; }

	/** Set the direction in which the entity is facing. */
	void SetProtocolFacing(Byte a_Facing)
	{
		ASSERT(a_Facing <= 3);
		m_Facing = a_Facing;
	}

protected:

	Byte m_Facing;

	Byte m_TickCounter = 0;

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;


	/** Converts protocol hanging item facing to eBlockFace values */
	inline static eBlockFace ProtocolFaceToBlockFace(Byte a_ProtocolFace)
	{
		// The client uses different values for item frame directions and block faces. Our constants are for the block faces, so we convert them here to item frame faces
		switch (a_ProtocolFace)
		{
			case 0: return BLOCK_FACE_ZP;
			case 2: return BLOCK_FACE_ZM;
			case 1: return BLOCK_FACE_XM;
			case 3: return BLOCK_FACE_XP;
			default:
			{
				LOGINFO("Invalid facing (%d) in a cHangingEntity, adjusting to BLOCK_FACE_XP.", a_ProtocolFace);
				ASSERT(!"Tried to convert a bad facing!");

				return cHangingEntity::ProtocolFaceToBlockFace(3);
			}
		}
	}


	/** Converts eBlockFace values to protocol hanging item faces */
	inline static Byte BlockFaceToProtocolFace(eBlockFace a_BlockFace)
	{
		// The client uses different values for item frame directions and block faces. Our constants are for the block faces, so we convert them here to item frame faces
		switch (a_BlockFace)
		{
			case BLOCK_FACE_ZP: return 0;
			case BLOCK_FACE_ZM: return 2;
			case BLOCK_FACE_XM: return 1;
			case BLOCK_FACE_XP: return 3;
			case BLOCK_FACE_YP:
			case BLOCK_FACE_YM:
			case BLOCK_FACE_NONE:
			{
				// Uncomment when entities are initialised with their real data, instead of dummy values:
				// LOGINFO("Invalid facing (%d) in a cHangingEntity, adjusting to BLOCK_FACE_XP.", a_BlockFace);
				// ASSERT(!"Tried to convert a bad facing!");

				return cHangingEntity::BlockFaceToProtocolFace(BLOCK_FACE_XP);
			}
		}
		UNREACHABLE("Unsupported block face");
	}
};  // tolua_export




