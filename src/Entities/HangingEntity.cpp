
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "HangingEntity.h"
#include "ClientHandle.h"
#include "Player.h"





cHangingEntity::cHangingEntity(eEntityType a_EntityType, eBlockFace a_BlockFace, CreateEntityInfo a_Info)
	: cEntity(a_Info, a_EntityType, 0.8, 0.8)
	, m_BlockFace(a_BlockFace)
{
	SetMaxHealth(1);
	SetHealth(1);
}





void cHangingEntity::SetDirection(eBlockFace a_BlockFace)
{
	if ((a_BlockFace < 2) || (a_BlockFace > 5))
	{
		ASSERT(!"Tried to set a bad direction!");
		return;
	}

	m_BlockFace = a_BlockFace;
}





void cHangingEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	int Dir = 0;

	// The client uses different values for item frame directions and block faces. Our constants are for the block faces, so we convert them here to item frame faces
	switch (m_BlockFace)
	{
		case BLOCK_FACE_ZP: Dir = 0; break;
		case BLOCK_FACE_ZM: Dir = 2; break;
		case BLOCK_FACE_XM: Dir = 1; break;
		case BLOCK_FACE_XP: Dir = 3; break;
		default:
		{
			LOGINFO("Invalid face (%d) in a cHangingEntity at {%d, %d, %d}, adjusting to BLOCK_FACE_XP.",
				m_BlockFace, (int)GetPosX(), (int)GetPosY(), (int)GetPosZ()
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




