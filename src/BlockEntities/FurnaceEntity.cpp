
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





cFurnaceEntity::cFurnaceEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, ContentsWidth, ContentsHeight, a_World),
	m_CurrentRecipe(nullptr),
	m_IsDestroyed(false),
	m_IsCooking(a_BlockType == E_BLOCK_LIT_FURNACE),
	m_NeedCookTime(0),
	m_TimeCooked(0),
	m_FuelBurnTime(0),
	m_TimeBurned(0),
	m_RewardCounter(0),
	m_IsLoading(false)
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





void cFurnaceEntity::Destroy()
{
	m_IsDestroyed = true;
	Super::Destroy();
}





void cFurnaceEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cFurnaceEntity &>(a_Src);
	m_Contents.CopyFrom(src.m_Contents);
	m_CurrentRecipe = src.m_CurrentRecipe;
	m_FuelBurnTime = src.m_FuelBurnTime;
	m_IsCooking = src.m_IsCooking;
	m_IsDestroyed = src.m_IsDestroyed;
	m_IsLoading = src.m_IsLoading;
	m_LastInput = src.m_LastInput;
	m_NeedCookTime = src.m_NeedCookTime;
	m_TimeBurned = src.m_TimeBurned;
	m_TimeCooked = src.m_TimeCooked;
}





void cFurnaceEntity::SendTo(cClientHandle & a_Client)
{
	// Nothing needs to be sent
	UNUSED(a_Client);
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
		a_Chunk.FastSetBlock(GetRelPos(), E_BLOCK_FURNACE, m_BlockMeta);
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





bool cFurnaceEntity::UsedBy(cPlayer * a_Player)
{
	a_Player->GetStatManager().AddValue(Statistic::InteractWithFurnace);

	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenWindow(new cFurnaceWindow(this));
		Window = GetWindow();
	}

	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(*Window);
		}
	}

	UpdateProgressBars(true);
	return true;
}





bool cFurnaceEntity::ContinueCooking(void)
{
	UpdateInput();
	UpdateFuel();
	return m_IsCooking;
}





int cFurnaceEntity::GetAndResetReward(void)
{
	int Reward = FloorC(m_RewardCounter);
	float Remainder = m_RewardCounter - static_cast<float>(Reward);
	// Remainder is used as the percent chance of getting an extra xp point
	if (GetRandomProvider().RandBool(Remainder))
	{
		Reward++;
	}
	m_RewardCounter = 0.0;
	return Reward;
}





void cFurnaceEntity::BroadcastProgress(size_t a_ProgressbarID, short a_Value)
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
	m_RewardCounter += m_CurrentRecipe->Reward;

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
	if ((NewTime == 0) || !CanCookInputToOutput())
	{
		// The item in the fuel slot is not suitable
		// or the input and output isn't available for cooking
		SetBurnTimes(0, 0);
		SetIsCooking(false);
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
	Super::OnSlotChanged(a_ItemGrid, a_SlotNum);

	if (m_IsDestroyed)
	{
		return;
	}

	if (m_IsLoading)
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
		if (!m_IsLoading)
		{
			m_TimeCooked = 0;
		}
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

		// Start burning new fuel if there's no flame now:
		if (GetFuelBurnTimeLeft() <= 0)
		{
			BurnNewFuel();
		}
		// Already burning, set cooking to ensure that cooking is occuring
		else
		{
			SetIsCooking(true);
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

	// Can cook, start cooking if not already underway:
	m_NeedCookTime = m_CurrentRecipe->CookTime;

	// Check if fuel needs to start a burn
	if (GetFuelBurnTimeLeft() <= 0)
	{
		BurnNewFuel();
	}
	// Already burning, set cooking to ensure that cooking is occuring
	else
	{
		SetIsCooking(true);
	}
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
		m_World->FastSetBlock(m_Pos, E_BLOCK_LIT_FURNACE, m_BlockMeta);
	}
}
