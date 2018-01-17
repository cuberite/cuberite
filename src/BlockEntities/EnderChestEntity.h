
#pragma once

#include "BlockEntity.h"
#include "UI/WindowOwner.h"





// tolua_begin
class cEnderChestEntity :
	public cBlockEntity,
	public cBlockEntityWindowOwner
{
	typedef cBlockEntity Super;

public:
	// tolua_end

	BLOCKENTITY_PROTODEF(cEnderChestEntity)

	cEnderChestEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	virtual ~cEnderChestEntity() override;

	// cBlockEntity overrides:
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

	static void LoadFromJson(const Json::Value & a_Value, cItemGrid & a_Grid);
	static void SaveToJson(Json::Value & a_Value, const cItemGrid & a_Grid);

	/** Opens a new enderchest window for this enderchest */
	void OpenNewWindow(void);
} ;  // tolua_export




