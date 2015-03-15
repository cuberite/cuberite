
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "HangingEntity.h"
#include "ClientHandle.h"
#include "Player.h"





cHangingEntity::cHangingEntity(eEntityType a_EntityType, eBlockFace a_Facing, double a_X, double a_Y, double a_Z) :
	cEntity(a_EntityType, a_X, a_Y, a_Z, 0.8, 0.8),
	m_Facing(cHangingEntity::BlockFaceToProtocolFace(a_Facing))
{
	SetMaxHealth(1);
	SetHealth(1);
}





void cHangingEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	SetYaw(GetProtocolFacing() * 90);
}




