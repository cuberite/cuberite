
// DropSpenser.h

// Declares the cDropSpenser class representing a common ancestor to the cDispenserEntity and cDropperEntity
// The dropper and dispenser only needs to override the DropSpenseFromSlot() function to provide the specific item behavior





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
class cDropSpenserEntity :
	public cBlockEntityWithItems,
	public cBlockEntityWindowOwner
{
	typedef cBlockEntityWithItems super;

public:
	enum {
		ContentsHeight = 3,
		ContentsWidth  = 3,
	} ;
	
	// tolua_end
	
	cDropSpenserEntity(BLOCKTYPE a_BlockType, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	virtual ~cDropSpenserEntity();
	
	static const char * GetClassStatic(void) { return "cDropSpenserEntity"; }

	bool LoadFromJson(const Json::Value & a_Value);
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value & a_Value) override;
	virtual bool Tick(float a_Dt) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player) override;
	
	// tolua_begin
	
	/// Modifies the block coords to match the dropspenser direction given (where the dropspensed pickups should materialize)
	void AddDropSpenserDir(int & a_BlockX, int & a_BlockY, int & a_BlockZ, NIBBLETYPE a_Direction);

	/// Sets the dropspenser to dropspense an item in the next tick
	void Activate(void);
	
	// tolua_end

protected:
	bool m_ShouldDropSpense;  ///< If true, the dropspenser will dropspense an item in the next tick

	void DropSpense(void);
	
	/// Override this function to provide the specific behavior for item dropspensing (drop / shoot / pour / ...)
	virtual void DropSpenseFromSlot(int a_SlotNum) = 0;
	
	/// Helper function, drops one item from the specified slot (like a dropper)
	void DropFromSlot(int a_SlotNum);
} ;  // tolua_export




