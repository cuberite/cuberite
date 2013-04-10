
#pragma once

#include "BlockEntity.h"
#include "UI/WindowOwner.h"
#include "ItemGrid.h"





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
	virtual void Destroy(void);

	bool LoadFromJson(const Json::Value & a_Value);
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value & a_Value) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool Tick(float a_Dt) override;
	virtual void UsedBy(cPlayer * a_Player) override;
	
	const cItem & GetSlot(int a_SlotNum) const { return m_Contents.GetSlot(a_SlotNum); }
	
	void SetSlot(int a_SlotNum, const cItem & a_Item) { m_Contents.SetSlot(a_SlotNum, a_Item); }

	/// Sets the dispenser to dispense an item in the next tick
	void Activate(void);
	
	const cItemGrid & GetContents(void) const { return m_Contents; }
	cItemGrid & GetContents(void) { return m_Contents; }
	
private:
	cItemGrid m_Contents;
	bool      m_ShouldDispense;  ///< If true, the dispenser will dispense an item in the next tick

	void Dispense(void);
};




