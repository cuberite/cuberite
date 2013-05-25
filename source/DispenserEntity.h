
#pragma once

#include "BlockEntityWithItems.h"
#include "UI/WindowOwner.h"





namespace Json
{
	class Value;
}

class cClientHandle;
class cServer;





// tolua_begin
class cDispenserEntity :
	public cBlockEntityWithItems,
	public cBlockEntityWindowOwner
{
	typedef cBlockEntityWithItems super;
	
public:
	enum {
		ContentsHeight = 3,
		ContentsWidth  = 3,
	} ;

	/// Constructor used while generating a chunk; sets m_World to NULL
	cDispenserEntity(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	// tolua_end
	
	/// Constructor used for normal operation
	cDispenserEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	virtual ~cDispenserEntity();

	static const char * GetClassStatic(void) { return "cDispenserEntity"; }

	bool LoadFromJson(const Json::Value & a_Value);
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value & a_Value) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool Tick(float a_Dt) override;
	virtual void UsedBy(cPlayer * a_Player) override;
	
	// tolua_begin
	
	/// Sets the dispenser to dispense an item in the next tick
	void Activate(void);
	
	// tolua_end

private:
	bool m_ShouldDispense;  ///< If true, the dispenser will dispense an item in the next tick

	void Dispense(void);
} ;  // tolua_export




