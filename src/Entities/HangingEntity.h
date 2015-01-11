
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
	eBlockFace GetFacing() const { return m_Facing; }
	
	/** Set the direction in which the entity is facing. */
	void SetFacing(eBlockFace a_Facing);
	
	/** Returns the X coord of the block in which the entity resides. */
	int GetBlockX() const { return POSX_TOINT; }
	
	/** Returns the Y coord of the block in which the entity resides. */
	int GetBlockY() const { return POSY_TOINT; }
	
	/** Returns the Z coord of the block in which the entity resides. */
	int GetBlockZ() const { return POSZ_TOINT; }

	// tolua_end

private:

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override {}

	eBlockFace m_Facing;

};  // tolua_export




