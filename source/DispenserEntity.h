
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

	bool LoadFromFile(cFile & a_File);  // deprecated format

	bool LoadFromJson(const Json::Value& a_Value );
	virtual void SaveToJson(Json::Value& a_Value ) override;

	virtual void SendTo(cClientHandle & a_Client) override;
	
	// Returns true if there's any change, forcing the chunk to go dirty.
	bool Tick( float a_Dt );
	
	virtual void UsedBy( cPlayer * a_Player ) override;
	
	const cItem * GetSlot(int i) const { return &(m_Items[i]); }
	
	void SetSlot(int a_Slot, const cItem & a_Item);
	
private:
	cItem * m_Items;
};




