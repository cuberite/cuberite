
#pragma once

#include "BlockEntity.h"
#include "UI/WindowOwner.h"





namespace Json
{
	class Value;
};

class cClientHandle;
class cServer;
class cItem;
class cNBTData;





class cChestEntity :              // tolua_export
	public cBlockEntity,            // tolua_export
	public cBlockEntityWindowOwner  // tolua_export
{  // tolua_export
public:
	cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	virtual ~cChestEntity();
	virtual void Destroy();

	static const char * GetClassStatic() { return "cChestEntity"; }

	const cItem * GetSlot(int a_Slot) const;                   // tolua_export
	void          SetSlot(int a_Slot, const cItem & a_Item );  // tolua_export

	bool LoadFromJson( const Json::Value& a_Value );
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value& a_Value ) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player);  // tolua_export
	
	/// Opens a new chest window for this chests. Scans for neighbors to open a double chest window, if appropriate.
	void OpenNewWindow(void);

	cItem * GetContents(void) { return m_Content; }

	static const int c_ChestWidth = 9;
	static const int c_ChestHeight = 3;
	
private:

	cItem * m_Content;
};	// tolua_export




