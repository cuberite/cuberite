
#pragma once

#include "BlockEntity.h"





class cEnchantingTableEntity :
	public cBlockEntity
{
	using Super = cBlockEntity;

public:

	cEnchantingTableEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World, AString a_CustomName = "");

	const AString & GetCustomName() const { return m_CustomName; }
	void SetCustomName(const AString & a_CustomName) { m_CustomName = a_CustomName; }

private:

	// cBlockEntity overrides:
	virtual cItems ConvertToPickups() const override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

	AString m_CustomName;
};
