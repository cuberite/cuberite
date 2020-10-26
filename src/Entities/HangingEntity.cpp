
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "HangingEntity.h"
#include "Player.h"
#include "../ClientHandle.h"





cHangingEntity::cHangingEntity(eEntityType a_EntityType, eBlockFace a_Facing, Vector3d a_Pos) :
	Super(a_EntityType, a_Pos, 0.8, 0.8),
	m_Facing(cHangingEntity::BlockFaceToProtocolFace(a_Facing))
{
	SetMaxHealth(1);
	SetHealth(1);
}





void cHangingEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	SetYaw(GetProtocolFacing() * 90);
}




