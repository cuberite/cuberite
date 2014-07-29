
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FurnaceEntity.h"
#include "../UI/Window.h"
#include "../Entities/Player.h"
#include "../Root.h"
#include "../Chunk.h"
#include "json/json.h"






enum
{
	PROGRESSBAR_SMELTING = 0,
	PROGRESSBAR_FUEL = 1,
} ;





cFurnaceEntity::cFurnaceEntity(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cWorld * a_World) :
	super(E_BLOCK_FURNACE, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, a_World),
	m_BlockType(a_BlockType),
	m_BlockMeta(a_BlockMeta),
	m_CurrentRecipe(NULL),
	m_IsCooking((a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_LIT_FURNACE)),
	m_NeedCookTime(0),
	m_TimeCooked(0),
	m_FuelBurnTime(0),
	m_TimeBurned(0),
	m_LastProgressFuel(0),
	m_LastProgressCook(0)
{
	cBlockEntityWindowOwner::SetBlockEntity(this);
	m_Contents.AddListener(*this);
}





cFurnaceEntity::~cFurnaceEntity()
{
	// Tell window its owner is destroyed
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->OwnerDestroyed();
	}
}





void cFurnaceEntity::UsedBy(cPlayer * a_Player)
{
	if (GetWindow() == NULL)
	{
		OpenWindow(new cFurnaceWindow(m_PosX, m_PosY, m_PosZ, this));
	}
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(Window);
			BroadcastProgress(PROGRESSBAR_FUEL,     (short)m_LastProgressFuel);
			BroadcastProgress(PROGRESSBAR_SMELTING, (short)m_LastProgressCook);
		}
	}
}





/// Restarts cooking. Used after the furnace is loaded from storage to set up the internal variables so that cooking continues, if it was active. Returns true if cooking.
bool cFurnaceEntity::ContinueCooking(void)
{
	UpdateInput();
	UpdateFuel();
	return m_IsCooking;
}





bool cFurnaceEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	if (m_FuelBurnTime <= 0)
	{
		// No fuel is burning, reset progressbars and bail out
		if ((m_LastProgressCook > 0) || (m_LastProgressFuel > 0))
		{
			UpdateProgressBars();
		}
		return false;
	}

	if (m_IsCooking)
	{
		m_TimeCooked++;
		if (m_TimeCooked >= m_NeedCookTime)
		{
			// Finished smelting one item
			FinishOne();
		}
	}
	
	m_TimeBurned++;
	if (m_TimeBurned >= m_FuelBurnTime)
	{
		// The current fuel has been exhausted, use another one, if possible
		BurnNewFuel();
	}
	
	UpdateProgressBars();
	
	return true;
}





bool cFurnaceEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	Json::Value AllSlots = a_Value.get("Slots", 0);
	int SlotIdx = 0;
	for (Json::Value::iterator itr = AllSlots.begin(); itr != AllSlots.end(); ++itr)
	{
		cItem Item;
		Item.FromJson(*itr);
		SetSlot(SlotIdx, Item);
		SlotIdx++;
	}

	m_NeedCookTime = (int)(a_Value.get("CookTime",   0).asDouble() / 50);
	m_TimeCooked   = (int)(a_Value.get("TimeCooked", 0).asDouble() / 50);
	m_FuelBurnTime = (int)(a_Value.get("BurnTime",   0).asDouble() / 50);
	m_TimeBurned   = (int)(a_Value.get("TimeBurned", 0).asDouble() / 50);

	return true;
}





void cFurnaceEntity::SaveToJson( Json::Value& a_Value)
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	Json::Value AllSlots;
	int NumSlots = m_Contents.GetNumSlots();
	for (int i = 0; i < NumSlots; i++)
	{
		Json::Value Slot;
		m_Contents.GetSlot(i).GetJson(Slot);
		AllSlots.append(Slot);
	}
	a_Value["Slots"] = AllSlots;

	a_Value["CookTime"]   = m_NeedCookTime * 50;
	a_Value["TimeCooked"] = m_TimeCooked   * 50;
	a_Value["BurnTime"]   = m_FuelBurnTime * 50;
	a_Value["TimeBurned"] = m_TimeBurned   * 50;
}





void cFurnaceEntity::SendTo(cClientHandle & a_Client)
{
	// Nothing needs to be sent
	UNUSED(a_Client);
}





void cFurnaceEntity::BroadcastProgress(int a_ProgressbarID, short a_Value)
{
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->BroadcastProgress(a_ProgressbarID, a_Value);
	}
}





/// One item finished cooking
void cFurnaceEntity::FinishOne()
{
	m_TimeCooked = 0;

	if (m_Contents.GetSlot(fsOutput).IsEmpty())
	{
		m_Contents.SetSlot(fsOutput, *m_CurrentRecipe->Out);
	}
	else
	{
		m_Contents.ChangeSlotCount(fsOutput, m_CurrentRecipe->Out->m_ItemCount);
	}
	m_Contents.ChangeSlotCount(fsInput, -m_CurrentRecipe->In->m_ItemCount);
	
	UpdateIsCooking();
}





void cFurnaceEntity::BurnNewFuel(void)
{
	cFurnaceRecipe * FR = cRoot::Get()->GetFurnaceRecipe();
	int NewTime = FR->GetBurnTime(m_Contents.GetSlot(fsFuel));
	if (NewTime == 0)
	{
		// The item in the fuel slot is not suitable
		m_FuelBurnTime = 0;
		m_TimeBurned = 0;
		SetIsCooking(false);
		return;
	}
	
	// Is the input and output ready for cooking?
	if (!CanCookInputToOutput())
	{
		return;
	}

	// Burn one new fuel:
	m_FuelBurnTime = NewTime;
	m_TimeBurned = 0;
	SetIsCooking(true);
	if (m_Contents.GetSlot(fsFuel).m_ItemType == E_ITEM_LAVA_BUCKET)
	{
		m_Contents.SetSlot(fsFuel, cItem(E_ITEM_BUCKET));
	}
	else
	{
		m_Contents.ChangeSlotCount(fsFuel, -1);
	}
}





void cFurnaceEntity::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	super::OnSlotChanged(a_ItemGrid, a_SlotNum);
	
	if (m_World == NULL)
	{
		// The furnace isn't initialized yet, do no processing
		return;
	}
	
	ASSERT(a_ItemGrid == &m_Contents);
	switch (a_SlotNum)
	{
		case fsInput:
		{
			UpdateInput();
			break;
		}
		
		case fsFuel:
		{
			UpdateFuel();
			break;
		}
		
		case fsOutput:
		{
			UpdateOutput();
			break;
		}
	}
}






/// Updates the current recipe, based on the current input
void cFurnaceEntity::UpdateInput(void)
{
	if (!m_Contents.GetSlot(fsInput).IsEqual(m_LastInput))
	{
		// The input is different from what we had before, reset the cooking time
		m_TimeCooked = 0;
	}
	m_LastInput = m_Contents.GetSlot(fsInput);
	
	cFurnaceRecipe * FR = cRoot::Get()->GetFurnaceRecipe();
	m_CurrentRecipe = FR->GetRecipeFrom(m_Contents.GetSlot(fsInput));
	if (!CanCookInputToOutput())
	{
		// This input cannot be cooked
		m_NeedCookTime = 0;
		SetIsCooking(false);
	}
	else
	{
		m_NeedCookTime = m_CurrentRecipe->CookTime;
		SetIsCooking(true);
		
		// Start burning new fuel if there's no flame now:
		if (GetFuelBurnTimeLeft() <= 0)
		{
			BurnNewFuel();
		}
	}
}





/// Called when the fuel slot changes or when the fuel is spent, burns another piece of fuel if appropriate
void cFurnaceEntity::UpdateFuel(void)
{
	if (m_FuelBurnTime > m_TimeBurned)
	{
		// The current fuel is still burning, don't modify anything:
		return;
	}
	
	// The current fuel is spent, try to burn some more:
	BurnNewFuel();
}





/// Called when the output slot changes; starts burning if space became available
void cFurnaceEntity::UpdateOutput(void)
{
	if (!CanCookInputToOutput())
	{
		// Cannot cook anymore:
		m_TimeCooked = 0;
		m_NeedCookTime = 0;
		SetIsCooking(false);
		return;
	}
	
	// No need to burn new fuel, the Tick() function will take care of that

	// Can cook, start cooking if not already underway:
	m_NeedCookTime = m_CurrentRecipe->CookTime;
	SetIsCooking(m_FuelBurnTime > 0);
}





/// Updates the m_IsCooking, based on the input slot, output slot and m_FuelBurnTime / m_TimeBurned
void cFurnaceEntity::UpdateIsCooking(void)
{
	if (
		!CanCookInputToOutput() ||        // Cannot cook this
		(m_FuelBurnTime <= 0) ||          // No fuel
		(m_TimeBurned >= m_FuelBurnTime)  // Fuel burnt out
	)
	{
		// Reset everything
		SetIsCooking(false);
		m_TimeCooked = 0;
		m_NeedCookTime = 0;
		return;
	}
	
	SetIsCooking(true);
}





/// Returns true if the input can be cooked into output and the item counts allow for another cooking operation
bool cFurnaceEntity::CanCookInputToOutput(void) const
{
	if (m_CurrentRecipe == NULL)
	{
		// This input cannot be cooked
		return false;
	}
	
	const cItem & Slot = m_Contents.GetSlot(fsOutput);
	if (Slot.IsEmpty())
	{
		// The output is empty, can cook
		return true;
	}

	if (!Slot.IsEqual(*m_CurrentRecipe->Out))
	{
		// The output slot is blocked with something that cannot be stacked with the recipe's output
		return false;
	}
	
	if (Slot.IsFullStack())
	{
		// Cannot add any more items to the output slot
		return false;
	}
	
	return true;
}





/// Broadcasts progressbar updates, if needed
void cFurnaceEntity::UpdateProgressBars(void)
{
	// In order to preserve bandwidth, an update is sent only every 10th tick
	// That's why the comparisons use the division by eight
	
	int CurFuel = (m_FuelBurnTime > 0) ? (200 - 200 * m_TimeBurned / m_FuelBurnTime) : 0;
	if ((CurFuel / 8) != (m_LastProgressFuel / 8))
	{
		BroadcastProgress(PROGRESSBAR_FUEL, (short)CurFuel);
		m_LastProgressFuel = CurFuel;
	}
	
	int CurCook = (m_NeedCookTime > 0) ? (200 * m_TimeCooked / m_NeedCookTime) : 0;
	if ((CurCook / 8) != (m_LastProgressCook / 8))
	{
		BroadcastProgress(PROGRESSBAR_SMELTING, (short)CurCook);
		m_LastProgressCook = CurCook;
	}
}





void cFurnaceEntity::SetIsCooking(bool a_IsCooking)
{
	if (a_IsCooking == m_IsCooking)
	{
		return;
	}

	m_IsCooking = a_IsCooking;
		
	// Light or extinguish the furnace:
	m_World->FastSetBlock(m_PosX, m_PosY, m_PosZ, m_IsCooking ? E_BLOCK_LIT_FURNACE : E_BLOCK_FURNACE, m_BlockMeta);
}



	
