
#pragma once

#include "BlockEntity.h"





class cEndPortalEntity :
	public cBlockEntity
{
	using Super = cBlockEntity;

public:

	cEndPortalEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World);

private:

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;
};
