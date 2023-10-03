#pragma once

#include "BlockEntityWithItems.h"
#include "../Simulator/RedstoneSimulator.h"





class cClientHandle;





// tolua_begin
class cShulkerBoxEntity :
	public cBlockEntityWithItems
{
	// tolua_end

	using Super = cBlockEntityWithItems;

	// tolua_begin

public:

	enum
	{
		ContentsHeight = 3,
		ContentsWidth  = 9,
	} ;

	// tolua_end

	/** Constructor used for normal operation */
	cShulkerBoxEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	// cBlockEntity overrides:
	virtual cItems ConvertToPickups() const override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual void OnRemoveFromWorld() override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool UsedBy(cPlayer * a_Player) override;


	/** Opens a new shulker box window */
	void OpenNewWindow();

	/** Returns true if the chest should not be accessible by players. */
	bool IsBlocked();

	/** Gets the number of players who currently have this shulker box open */
	int GetNumberOfPlayers(void) const { return m_NumActivePlayers; }

	/** Sets the number of players who currently have this shulker box open */
	void SetNumberOfPlayers(int a_NumActivePlayers) { m_NumActivePlayers = a_NumActivePlayers; }

	/** Loads shulker box from item */
	void Load(const cItem & a_Item);

	static bool IsShulkerBox(short m_ItemType);

	/** Custom name of a shulker box */
	AString m_CustomName;

private:

	/** Number of players who currently have this shulker box open */
	int m_NumActivePlayers;

	/** cItemGrid::cListener overrides: */
	virtual void OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum) override;

	/** Returns direction for obstruction check */
	static Vector3i BlockMetaToObstructionCheckDirection(NIBBLETYPE a_BlockMeta);

} ;  // tolua_export
