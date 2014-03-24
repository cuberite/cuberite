/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "HangingEntity.h"
#include "ClientHandle.h"
#include "Player.h"





cHangingEntity::cHangingEntity(eEntityType a_EntityType, eBlockFace a_BlockFace, double a_X, double a_Y, double a_Z)
	:	cEntity(a_EntityType, a_X, a_Y, a_Z, 0.8, 0.8)
	, m_BlockFace(a_BlockFace)
{
	SetMaxHealth(1);
	SetHealth(1);
}





void cHangingEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	int Dir = 0;
			
	// The client uses different values for item frame directions and block faces. Our constants are for the block faces, so we convert them here to item frame faces
	switch (m_BlockFace)
	{
		case BLOCK_FACE_ZP: break; // Initialised to zero
		case BLOCK_FACE_ZM: Dir = 2; break;
		case BLOCK_FACE_XM: Dir = 1; break;
		case BLOCK_FACE_XP: Dir = 3; break;
		default: ASSERT(!"Unhandled block face when trying to spawn item frame!"); return;
	}

	if ((Dir == 0) || (Dir == 2)) // Probably a client bug, but two directions are flipped and contrary to the norm, so we do -180
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




