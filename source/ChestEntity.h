
#pragma once

#include "BlockEntity.h"
#include "UI/WindowOwner.h"
#include "ItemGrid.h"





namespace Json
{
	class Value;
};

class cClientHandle;
class cServer;
class cNBTData;





// tolua_begin
class cChestEntity :
	public cBlockEntity,
	public cBlockEntityWindowOwner
{
	typedef cBlockEntity super;
	
public:
	cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ);  // Used while generating
	// tolua_end
	
	cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	
	virtual ~cChestEntity();
	virtual void Destroy();

	static const char * GetClassStatic() { return "cChestEntity"; }

	// tolua_begin
	const cItem & GetSlot(int a_Slot) const { return m_Contents.GetSlot(a_Slot); }
	void          SetSlot(int a_Slot, const cItem & a_Item ) { m_Contents.SetSlot(a_Slot, a_Item); }
	// tolua_end
	
	bool LoadFromJson( const Json::Value& a_Value );
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value& a_Value ) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player);  // tolua_export
	
	/// Opens a new chest window for this chests. Scans for neighbors to open a double chest window, if appropriate.
	void OpenNewWindow(void);

	const cItemGrid & GetContents(void) const { return m_Contents; }
	
	cItemGrid & GetContents(void) { return m_Contents; }  // tolua_export

	static const int c_ChestWidth = 9;
	static const int c_ChestHeight = 3;
	
private:

	cItemGrid m_Contents;
} ;  // tolua_export




