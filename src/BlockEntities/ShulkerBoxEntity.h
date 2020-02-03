
#pragma once

#include "BlockEntityWithItems.h"





class cClientHandle;





// tolua_begin
class cShulkerBoxEntity :
	public cBlockEntityWithItems
{
	// tolua_end

	using super = cBlockEntityWithItems;

	// tolua_begin

public:

	enum
	{
		ContentsHeight = 3,
		ContentsWidth  = 9,
	} ;

	// tolua_end

	BLOCKENTITY_PROTODEF(cShulkerBoxEntity)

	/** Constructor used for normal operation */
	cShulkerBoxEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	virtual ~cShulkerBoxEntity() override;

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool UsedBy(cPlayer * a_Player) override;

	/** Opens a new shulker box window. */
	void OpenNewWindow();

	/** Forces any players to close the owned window. */
	void DestroyWindow();

	/** Returns true if the shulker box should not be accessible by players. */
	bool IsBlocked();

	/** Gets the number of players who currently have this shulker box open */
	int GetNumberOfPlayers(void) const { return m_NumActivePlayers; }

	/** Sets the number of players who currently have this shulker box open */
	void SetNumberOfPlayers(int a_NumActivePlayers) { m_NumActivePlayers = a_NumActivePlayers; }


private:

	/** Number of players who currently have this shulker box open */
	int m_NumActivePlayers;

} ;  // tolua_export




