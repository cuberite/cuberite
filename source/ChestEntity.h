
#pragma once

#include "BlockEntityWithItems.h"
#include "UI/WindowOwner.h"





namespace Json
{
	class Value;
};

class cClientHandle;
class cServer;
class cNBTData;





// tolua_begin
class cChestEntity :
	public cBlockEntityWithItems,
	public cBlockEntityWindowOwner
{
	typedef cBlockEntityWithItems super;
	
public:
	enum {
		ContentsHeight = 3,
		ContentsWidth  = 9,
	} ;
	
	
	/// Constructor used while generating a chunk; sets m_World to NULL
	cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	// tolua_end
	
	/// Constructor used for normal operation
	cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	
	virtual ~cChestEntity();

	static const char * GetClassStatic(void) { return "cChestEntity"; }

	bool LoadFromJson(const Json::Value& a_Value);
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value & a_Value ) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player);
	
	/// Opens a new chest window for this chests. Scans for neighbors to open a double chest window, if appropriate.
	void OpenNewWindow(void);
} ;  // tolua_export




