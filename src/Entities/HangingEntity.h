
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


	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override
	{
		UNUSED(a_Dt);
		UNUSED(a_Chunk);
	}


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




