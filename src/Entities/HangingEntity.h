
#pragma once

#include "Entity.h"





// tolua_begin
class cHangingEntity :
	public cEntity
{
	// tolua_end
	typedef cEntity super;

public:

	CLASS_PROTODEF(cHangingEntity)

	cHangingEntity(eEntityType a_EntityType, eBlockFace a_BlockFace, double a_X, double a_Y, double a_Z);
	
	/** Returns the orientation from the hanging entity */
	eBlockFace GetDirection() const { return m_BlockFace; }  // tolua_export
	
	/** Set the orientation from the hanging entity */
	void SetDirection(eBlockFace a_BlockFace) { m_BlockFace = a_BlockFace; }  // tolua_export
	
	/** Returns the X coord. */
	int GetTileX() const { return POSX_TOINT; }  // tolua_export
	
	/** Returns the Y coord. */
	int GetTileY() const { return POSY_TOINT; }  // tolua_export
	
	/** Returns the Z coord. */
	int GetTileZ() const { return POSZ_TOINT; }  // tolua_export

private:

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override {}

	eBlockFace m_BlockFace;

};  // tolua_export




