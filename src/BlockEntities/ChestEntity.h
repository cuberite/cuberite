
#pragma once

#include "BlockEntityWithItems.h"





namespace Json
{
	class Value;
};

class cClientHandle;
class cServer;
class cNBTData;





// tolua_begin
class cChestEntity :
	public cBlockEntityWithItems
{
	typedef cBlockEntityWithItems super;
	
public:
	enum
	{
		ContentsHeight = 3,
		ContentsWidth  = 9,
	} ;
	
	// tolua_end
	
	/** Constructor used for normal operation */
	cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World, BLOCKTYPE a_Type);
	
	virtual ~cChestEntity();

	static const char * GetClassStatic(void) { return "cChestEntity"; }

	bool LoadFromJson(const Json::Value & a_Value);
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value & a_Value) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player) override;
	
	/** Opens a new chest window for this chest.
	Scans for neighbors to open a double chest window, if appropriate. */
	void OpenNewWindow(void);

	/** Gets the number of players who currently have this chest open */
	int GetNumberOfPlayers(void) const { return m_NumActivePlayers; }

	/** Sets the number of players who currently have this chest open */
	void SetNumberOfPlayers(int a_NumActivePlayers) { m_NumActivePlayers = a_NumActivePlayers; }

private:

	/** Number of players who currently have this chest open */
	int m_NumActivePlayers;
} ;  // tolua_export




