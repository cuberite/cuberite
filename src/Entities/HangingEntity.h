
#pragma once

#include "Entity.h"





// tolua_begin
class cHangingEntity :
	public cEntity
{
	typedef cEntity super;

public:

	// tolua_end

	CLASS_PROTODEF(cHangingEntity)

	cHangingEntity(eEntityType a_EntityType, eBlockFace a_BlockFace, double a_X, double a_Y, double a_Z);
	
	// tolua_begin

	/** Returns the direction in which the entity is facing. */
	eBlockFace GetFacing() const { return cHangingEntity::ProtocolFaceToBlockFace(m_Facing); }

	/** Set the direction in which the entity is facing. */
	void SetFacing(eBlockFace a_Facing) { m_Facing = cHangingEntity::BlockFaceToProtocolFace(a_Facing); }

	// tolua_end

	/** Returns the direction in which the entity is facing. */
	Byte GetProtocolFacing() const { return m_Facing; }

	/** Set the direction in which the entity is facing. */
	void SetProtocolFacing(Byte a_Facing)
	{
		ASSERT((a_Facing <= 3) && (a_Facing >= 0));
		m_Facing = a_Facing;
	}

protected:

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override
	{
		UNUSED(a_Dt);
		UNUSED(a_Chunk);
	}

	/** Converts protocol hanging item facing to eBlockFace values */
	inline static eBlockFace ProtocolFaceToBlockFace(Byte a_ProtocolFace)
	{
		eBlockFace Dir;

		// The client uses different values for item frame directions and block faces. Our constants are for the block faces, so we convert them here to item frame faces
		switch (a_ProtocolFace)
		{
			case 0: Dir = BLOCK_FACE_ZP; break;
			case 2: Dir = BLOCK_FACE_ZM; break;
			case 1: Dir = BLOCK_FACE_XM; break;
			case 3: Dir = BLOCK_FACE_XP; break;
			default:
			{
				LOGINFO("Invalid facing (%d) in a cHangingEntity, adjusting to BLOCK_FACE_XP.", a_ProtocolFace);
				ASSERT(!"Tried to convert a bad facing!");

				Dir = cHangingEntity::ProtocolFaceToBlockFace(3);
			}
		}

		return Dir;
	}

	/** Converts eBlockFace values to protocol hanging item faces */
	inline static Byte BlockFaceToProtocolFace(eBlockFace a_BlockFace)
	{
		Byte Dir;

		// The client uses different values for item frame directions and block faces. Our constants are for the block faces, so we convert them here to item frame faces
		switch (a_BlockFace)
		{
			case BLOCK_FACE_ZP: Dir = 0; break;
			case BLOCK_FACE_ZM: Dir = 2; break;
			case BLOCK_FACE_XM: Dir = 1; break;
			case BLOCK_FACE_XP: Dir = 3; break;
			default:
			{
				// Uncomment when entities are initialised with their real data, instead of dummy values:
				// LOGINFO("Invalid facing (%d) in a cHangingEntity, adjusting to BLOCK_FACE_XP.", a_BlockFace);
				// ASSERT(!"Tried to convert a bad facing!");

				Dir = cHangingEntity::BlockFaceToProtocolFace(BLOCK_FACE_XP);
			}
		}

		return Dir;
	}

	Byte m_Facing;

};  // tolua_export




