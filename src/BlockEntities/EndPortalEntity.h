
#pragma once

#include "BlockEntity.h"





class cEndPortalEntity :
	public cBlockEntity
{
	using Super = cBlockEntity;

public:

	cEndPortalEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

private:

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;
};
