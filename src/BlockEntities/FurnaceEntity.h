
#pragma once

#include "BlockEntityWithItems.h"
#include "../FurnaceRecipe.h"





class cClientHandle;





// tolua_begin
class cFurnaceEntity :
	public cBlockEntityWithItems
{
	// tolua_end

	using Super = cBlockEntityWithItems;

	// tolua_begin

public:

	enum
	{
		fsInput  = 0,  // Input slot number
		fsFuel   = 1,  // Fuel slot number
		fsOutput = 2,  // Output slot number

		ContentsWidth  = 3,
		ContentsHeight = 1,
	};

	// tolua_end

	/** Constructor used for normal operation */
	cFurnaceEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	virtual ~cFurnaceEntity() override;

	// cBlockEntity overrides:
	virtual void Destroy() override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual bool UsedBy(cPlayer * a_Player) override;

	/** Restarts cooking
	Used after the furnace is loaded from storage to set up the internal variables so that cooking continues, if it was active
	Returns true if cooking */
	bool ContinueCooking(void);

	// tolua_begin

	/** Returns the item in the input slot */
	const cItem & GetInputSlot(void) const { return GetSlot(fsInput); }

	/** Returns the item in the fuel slot */
	const cItem & GetFuelSlot(void) const { return GetSlot(fsFuel); }

	/** Returns the item in the output slot */
	const cItem & GetOutputSlot(void) const { return GetSlot(fsOutput); }

	/** Sets the item in the input slot */
	void SetInputSlot(const cItem & a_Item) { SetSlot(fsInput, a_Item); }

	/** Sets the item in the fuel slot */
	void SetFuelSlot(const cItem & a_Item) { SetSlot(fsFuel, a_Item); }

	/** Sets the item in the output slot */
	void SetOutputSlot(const cItem & a_Item) { SetSlot(fsOutput, a_Item); }

	/** Returns the time that the current item has been cooking, in ticks */
	int GetTimeCooked(void) const { return m_TimeCooked; }

	/** Returns the time until the current item finishes cooking, in ticks */
	int GetCookTimeLeft(void) const { return m_NeedCookTime - m_TimeCooked; }

	/** Returns the time until the current fuel is depleted, in ticks */
	int GetFuelBurnTimeLeft(void) const { return m_FuelBurnTime - m_TimeBurned; }

	/** Returns true if there's time left before the current fuel is depleted */
	bool HasFuelTimeLeft(void) const { return (GetFuelBurnTimeLeft() > 0); }

	/** Calculates, resets, and returns the experience reward in this furnace */
	int GetAndResetReward(void);

	// tolua_end

	void SetBurnTimes(int a_FuelBurnTime, int a_TimeBurned)
	{
		m_FuelBurnTime = a_FuelBurnTime;
		m_TimeBurned = a_TimeBurned;
	}

	void SetCookTimes(int a_NeedCookTime, int a_TimeCooked)
	{
		m_NeedCookTime = a_NeedCookTime;
		m_TimeCooked = a_TimeCooked;
	}

	void SetLoading(bool a_IsLoading)
	{
		m_IsLoading = a_IsLoading;
	}


protected:

	/** The recipe for the current input slot */
	const cFurnaceRecipe::cRecipe * m_CurrentRecipe;

	/** The item that is being smelted */
	cItem m_LastInput;

	/** Set to true when the furnace entity has been destroyed to prevent the block being set again */
	bool m_IsDestroyed;

	/** Set to true if the furnace is cooking an item */
	bool m_IsCooking;

	/** Amount of ticks needed to fully cook current item */
	int m_NeedCookTime;

	/** Amount of ticks that the current item has been cooking */
	int m_TimeCooked;

	/** Amount of ticks that the current fuel can burn (in total); zero if no fuel burning */
	int m_FuelBurnTime;

	/** Amount of ticks that the current fuel has been burning */
	int m_TimeBurned;

	/** Running total of experience that can be picked up */
	float m_RewardCounter;

	/** Is the block currently being loaded into the world? */
	bool m_IsLoading;

	/** Sends the specified progressbar value to all clients of the window */
	void BroadcastProgress(size_t a_ProgressbarID, short a_Value);

	/** One item finished cooking */
	void FinishOne();

	/** Starts burning a new fuel, if possible */
	void BurnNewFuel(void);

	/** Updates the recipe, based on the current input */
	void UpdateInput(void);

	/** Called when the fuel slot changes or when the fuel is spent, burns another piece of fuel if appropriate */
	void UpdateFuel(void);

	/** Called when the output slot changes */
	void UpdateOutput(void);

	/** Returns true if the input can be cooked into output and the item counts allow for another cooking operation */
	bool CanCookInputToOutput(void) const;

	/** Broadcasts progressbar updates, if needed */
	void UpdateProgressBars(bool a_ForceUpdate = false);

	/** Sets the m_IsCooking variable, updates the furnace block type based on the value */
	void SetIsCooking(bool a_IsCooking);

	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum) override;

} ;  // tolua_export
