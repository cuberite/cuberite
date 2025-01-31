
#pragma once

#include "BlockEntity.h"
#include "../UI/WindowOwner.h"





// tolua_begin
class cEnderChestEntity :
	public cBlockEntity,
	public cBlockEntityWindowOwner
{
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	cEnderChestEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World);

	// cBlockEntity overrides:
	virtual void OnRemoveFromWorld() override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

	static void LoadFromJson(const Json::Value & a_Value, cItemGrid & a_Grid);
	static void SaveToJson(Json::Value & a_Value, const cItemGrid & a_Grid);

	/** Opens a new enderchest window for this enderchest */
	void OpenNewWindow(void);
} ;  // tolua_export
