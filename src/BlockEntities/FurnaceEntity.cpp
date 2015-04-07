
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FurnaceEntity.h"
#include "../UI/FurnaceWindow.h"
#include "../Entities/Player.h"
#include "../Root.h"
#include "../Chunk.h"






enum
{
	PROGRESSBAR_FUEL = 0,
	PROGRESSBAR_SMELTING = 2,
	PROGRESSBAR_SMELTING_CONFIRM = 3,
} ;





cFurnaceEntity::cFurnaceEntity(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cWorld * a_World) :
	super(a_BlockType, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, a_World),
	m_BlockMeta(a_BlockMeta),
	m_CurrentRecipe(nullptr),
	m_IsDestroyed(false),
	m_IsCooking(a_BlockType == E_BLOCK_LIT_FURNACE),
	m_NeedCookTime(0),
	m_TimeCooked(0),
	m_FuelBurnTime(0),
	m_TimeBurned(0)
{
	m_Contents.AddListener(*this);
}





cFurnaceEntity::~cFurnaceEntity()
{
	// Tell window its owner is destroyed
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





void cFurnaceEntity::UsedBy(cPlayer * a_Player)
{
	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenWindow(new cFurnaceWindow(m_PosX, m_PosY, m_PosZ, this));
		Window = GetWindow();
	}

	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(Window);
		}
	}

	UpdateProgressBars(true);
}





bool cFurnaceEntity::ContinueCooking(void)
{
	UpdateInput();
	UpdateFuel();
	return m_IsCooking;
}





bool cFurnaceEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);

	if (m_FuelBurnTime <= 0)
	{
		// If a furnace is out of fuel, the progress bar reverses at twice the speed of cooking.
		m_TimeCooked = std::max((m_TimeCooked - 2), 0);

		// Reset progressbars, block type, and bail out
		m_BlockType = E_BLOCK_FURNACE;
		a_Chunk.FastSetBlock(GetRelX(), m_PosY, GetRelZ(), E_BLOCK_FURNACE, m_BlockMeta);
		UpdateProgressBars();
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





void cFurnaceEntity::SendTo(cClientHandle & a_Client)
{
	// Nothing needs to be sent
	UNUSED(a_Client);
}





void cFurnaceEntity::BroadcastProgress(short a_ProgressbarID, short a_Value)
{
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->SetProperty(a_ProgressbarID, a_Value);
	}
}





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
}





void cFurnaceEntity::BurnNewFuel(void)
{
	cFurnaceRecipe * FR = cRoot::Get()->GetFurnaceRecipe();
	int NewTime = FR->GetBurnTime(m_Contents.GetSlot(fsFuel));
	if (NewTime == 0)
	{
		// The item in the fuel slot is not suitable
		SetBurnTimes(0, 0);
		SetIsCooking(false);
		return;
	}

	// Is the input and output ready for cooking?
	if (!CanCookInputToOutput())
	{
		return;
	}

	// Burn one new fuel:
	SetBurnTimes(NewTime, 0);
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

	if (m_IsDestroyed)
	{
		return;
	}

	ASSERT(a_ItemGrid == &m_Contents);
	switch (a_SlotNum)
	{
		case fsInput:  UpdateInput();  break;
		case fsFuel:   UpdateFuel();   break;
		case fsOutput: UpdateOutput(); break;
		default: ASSERT(!"Invalid furnace slot update!"); break;
	}
}






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
		// This input cannot be cooked, reset cook counter immediately
		SetCookTimes(0, 0);
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





void cFurnaceEntity::UpdateOutput(void)
{
	if (!CanCookInputToOutput())
	{
		// Cannot cook anymore:
		SetCookTimes(0, 0);
		SetIsCooking(false);
		return;
	}

	// No need to burn new fuel, the Tick() function will take care of that

	// Can cook, start cooking if not already underway:
	m_NeedCookTime = m_CurrentRecipe->CookTime;
	SetIsCooking(m_FuelBurnTime > 0);
}





bool cFurnaceEntity::CanCookInputToOutput(void) const
{
	if (m_CurrentRecipe == nullptr)
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





void cFurnaceEntity::UpdateProgressBars(bool a_ForceUpdate)
{
	// In order to preserve bandwidth, an update is sent only every 10th tick
	if (!a_ForceUpdate && (m_World->GetWorldAge() % 10 != 0))
	{
		return;
	}

	int CurFuel = (m_FuelBurnTime > 0) ? 200 - (200 * m_TimeBurned / m_FuelBurnTime) : 0;
	BroadcastProgress(PROGRESSBAR_FUEL, static_cast<short>(CurFuel));
	
	int CurCook = (m_NeedCookTime > 0) ? (200 * m_TimeCooked / m_NeedCookTime) : 0;
	BroadcastProgress(PROGRESSBAR_SMELTING_CONFIRM, 200);  // Post 1.8, Mojang requires a random packet with an ID of three and value of 200. Wat. Wat. Wat.
	BroadcastProgress(PROGRESSBAR_SMELTING, static_cast<short>(CurCook));
}





void cFurnaceEntity::SetIsCooking(bool a_IsCooking)
{
	if (a_IsCooking == m_IsCooking)
	{
		return;
	}
	m_IsCooking = a_IsCooking;

	// Only light the furnace as it is extinguished only when the fuel runs out, not when cooking stops - handled in this::Tick()
	if (m_IsCooking)
	{
		m_BlockType = E_BLOCK_LIT_FURNACE;
		m_World->FastSetBlock(m_PosX, m_PosY, m_PosZ, E_BLOCK_LIT_FURNACE, m_BlockMeta);
	}
}
