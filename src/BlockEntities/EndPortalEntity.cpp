
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EndPortalEntity.h"
#include "../ClientHandle.h"





cEndPortalEntity::cEndPortalEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) :
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World)
{
	ASSERT(a_BlockType == E_BLOCK_END_PORTAL);
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
