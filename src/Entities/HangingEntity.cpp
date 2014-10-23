
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "HangingEntity.h"
#include "ClientHandle.h"
#include "Player.h"





cHangingEntity::cHangingEntity(eEntityType a_EntityType, eBlockFace a_Facing, double a_X, double a_Y, double a_Z) :
	cEntity(a_EntityType, a_X, a_Y, a_Z, 0.8, 0.8),
	m_Facing(a_Facing)
{
	SetMaxHealth(1);
	SetHealth(1);
}





void cHangingEntity::SetFacing(eBlockFace a_Facing)
{
	// Y-based faces are not allowed:
	switch (a_Facing)
	{
		case BLOCK_FACE_NONE:
		case BLOCK_FACE_YM:
		case BLOCK_FACE_YP:
		{
			LOGWARNING("%s: Invalid facing: %d. Ignoring.", __FUNCTION__, a_Facing);
			ASSERT(!"Tried to set a bad facing!");
			return;
		}
		default: break;
	}

	m_Facing = a_Facing;
}





void cHangingEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	int Dir = 0;

	// The client uses different values for item frame directions and block faces. Our constants are for the block faces, so we convert them here to item frame faces
	switch (m_Facing)
	{
		case BLOCK_FACE_ZP: Dir = 0; break;
		case BLOCK_FACE_ZM: Dir = 2; break;
		case BLOCK_FACE_XM: Dir = 1; break;
		case BLOCK_FACE_XP: Dir = 3; break;
		default:
		{
			LOGINFO("Invalid facing (%d) in a cHangingEntity at {%d, %d, %d}, adjusting to BLOCK_FACE_XP.",
				m_Facing, (int)GetPosX(), (int)GetPosY(), (int)GetPosZ()
			);
			Dir = 3;
		}
	}

	if ((Dir == 0) || (Dir == 2))  // Probably a client bug, but two directions are flipped and contrary to the norm, so we do -180
	{
		SetYaw((Dir * 90) - 180);
	}
	else
	{
		SetYaw(Dir * 90);
	}

	a_ClientHandle.SendSpawnObject(*this, 71, Dir, (Byte)GetYaw(), (Byte)GetPitch());
	a_ClientHandle.SendEntityMetadata(*this);
}




