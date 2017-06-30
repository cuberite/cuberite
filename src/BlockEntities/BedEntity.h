
// BedEntity.h

#pragma once

#include "BlockEntity.h"





// tolua_begin

class cBedEntity :
	public cBlockEntity
{
	typedef cBlockEntity Super;
public:
	// tolua_end

	BLOCKENTITY_PROTODEF(cBedEntity)

	cBedEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	// tolua_begin

	/** Returns the color of the bed */
	short GetColor(void) const { return m_Color; }

	/** Set the color of the bed. */
	void SetColor(short a_Color) { m_Color = a_Color; }

	/** Returns true if this bed block has the meta for head set */
	bool IsHead(void) { return ((m_BlockMeta & 0x08) == 0x08); }

	// tolua_end

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override { return false; }
	virtual void SendTo(cClientHandle & a_Client) override;

private:
	short m_Color;
};  // tolua_export

