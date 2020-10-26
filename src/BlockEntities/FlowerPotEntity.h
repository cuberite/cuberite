// FlowerPotEntity.h

// Declares the cFlowerPotEntity class representing a single sign in the world





#pragma once

#include "BlockEntity.h"
#include "../Item.h"





// tolua_begin
class cFlowerPotEntity :
	public cBlockEntity
{
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	/** Creates a new flowerpot entity at the specified block coords. a_World may be nullptr */
	cFlowerPotEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);


	// tolua_begin

	/** Is a flower in the pot? */
	bool IsItemInPot(void) { return !m_Item.IsEmpty(); }

	/** Get the item in the flower pot */
	cItem GetItem(void) const { return m_Item; }

	/** Set the item in the flower pot */
	void SetItem(const cItem & a_Item) { m_Item = a_Item; }

	// tolua_end

	// cBlockEntity overrides:
	virtual void Destroy(void) override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

	static bool IsFlower(short m_ItemType, short m_ItemData);

private:

	cItem m_Item;
} ;  // tolua_export
