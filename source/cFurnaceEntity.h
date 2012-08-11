
#pragma once

#include "cBlockEntity.h"
#include "cWindowOwner.h"
#include "cItem.h"





namespace Json
{
	class Value;
}

class cClientHandle;
class cServer;





class cFurnaceEntity :
	public cBlockEntity,
	public cBlockEntityWindowOwner
{
public:
	cFurnaceEntity(int a_X, int a_Y, int a_Z, cWorld * a_World);
	virtual ~cFurnaceEntity();
	virtual void Destroy();

	bool LoadFromFile(cFile & a_File);  // deprecated format

	bool LoadFromJson(const Json::Value& a_Value );
	virtual void SaveToJson(Json::Value& a_Value ) override;

	// Returns true if there's any change, forcing the chunk to go dirty.
	bool Tick( float a_Dt );
	
	virtual void UsedBy( cPlayer * a_Player ) override;

	bool StartCooking();
	
	/// Restarts cooking. Used after the furnace is loaded from storage to set up the internal variables so that cooking continues, if it was active. Returns true if cooking.
	bool ContinueCooking(void);

	void ResetCookTimer();
	
	const cItem & GetSlot(int i) const { return m_Items[i]; }
	
	void SetSlot(int a_Slot, const cItem & a_Item);
	
	float GetTimeCooked(void) const {return m_TimeCooked; }
	float GetTimeToBurn(void) const {return m_BurnTime - m_TimeBurned; }
	
	void SetBurnTimes(float a_BurnTime, float a_TimeBurned) {m_BurnTime = a_BurnTime; m_TimeBurned = 0; }
	void SetCookTimes(float a_CookTime, float a_TimeCooked) {m_CookTime = a_CookTime; m_TimeCooked = a_TimeCooked; }
	
private:

	cItem * m_Items;
	cItem * m_CookingItem;
	
	// All timers are in 1 ms
	float   m_CookTime;    // Amount of time needed to fully cook current item
	float   m_TimeCooked;  // Amount of time that the current item has been cooking
	float   m_BurnTime;    // Amount of time that the current fuel can burn (in total); zero if no fuel burning
	float   m_TimeBurned;  // Amount of time that the current fuel has been burning
};




