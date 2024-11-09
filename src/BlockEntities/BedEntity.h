
// BedEntity.h

#pragma once

#include "BlockEntity.h"
#include "../BlockType.h"





// tolua_begin

class cBedEntity :
	public cBlockEntity
{
	// tolua_end
	using Super = cBlockEntity;

public:  // tolua_export

	cBedEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World, short a_Color = E_META_WOOL_RED);

	// tolua_begin

	/** Returns the color of the bed */
	short GetColor(void) const { return m_Color; }

	/** Set the color of the bed. */
	void SetColor(short a_Color);

	/** Returns true if this is the pillow block, it has then the meta 8. */
	bool IsPillowBlock(void) { return ((m_BlockMeta & 0x08) == 0x08); }

	// tolua_end

	// cBlockEntity overrides:
	virtual cItems ConvertToPickups() const override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override { return false; }
	virtual void SendTo(cClientHandle & a_Client) override;

private:

	short m_Color;
};  // tolua_export
