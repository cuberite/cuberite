
#pragma once

#include "BlockEntity.h"
#include "UI/WindowOwner.h"
#include "Item.h"





namespace Json
{
	class Value;
}

class cClientHandle;
class cServer;





class cDispenserEntity :
	public cBlockEntity,
	public cBlockEntityWindowOwner
{
public:
	cDispenserEntity(int a_X, int a_Y, int a_Z, cWorld * a_World);
	virtual ~cDispenserEntity();
	virtual void Destroy();

	bool LoadFromJson(const Json::Value & a_Value);
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value & a_Value) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool Tick(float a_Dt) override;
	virtual void UsedBy(cPlayer * a_Player) override;
	
	const cItem * GetSlot(int i) const { return &(m_Items[i]); }
	
	void SetSlot(int a_Slot, const cItem & a_Item);

	void Activate();
	
private:
	cItem * m_Items;
	bool m_CanDispense;

	void Dispense();
};




