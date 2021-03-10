
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EndPortalEntity.h"
#include "../ClientHandle.h"





cEndPortalEntity::cEndPortalEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World) :
	Super(a_Block, a_Pos, a_World)
{
	ASSERT(a_Block.Type() == BlockType::EndPortal);
}





void cEndPortalEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
}





bool cEndPortalEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	return true;
}





void cEndPortalEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}
