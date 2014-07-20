
#pragma once

#include "BlockEntity.h"
#include "UI/WindowOwner.h"
#include "json/json.h"





// tolua_begin
class cEnderChestEntity :
	public cBlockEntity,
	public cBlockEntityWindowOwner
{
	typedef cBlockEntity super;
	
public:
	// tolua_end
	
	cEnderChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	virtual ~cEnderChestEntity();

	static const char * GetClassStatic(void) { return "cEnderChestEntity"; }
	
	// cBlockEntity overrides:
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SaveToJson(Json::Value & a_Value) override { UNUSED(a_Value); }
	virtual void SendTo(cClientHandle & a_Client) override { UNUSED(a_Client); }

	static void LoadFromJson(const Json::Value & a_Value, cItemGrid & a_Grid);
	static void SaveToJson(Json::Value & a_Value, const cItemGrid & a_Grid);
	
	/** Opens a new enderchest window for this enderchest */
	void OpenNewWindow(void);
} ;  // tolua_export




