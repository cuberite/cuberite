
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BrewingstandEntity.h"
#include "../UI/BrewingstandWindow.h"
#include "../Entities/Player.h"
#include "../Root.h"
#include "../Chunk.h"












cBrewingstandEntity::cBrewingstandEntity(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cWorld * a_World) :
	super(a_BlockType, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, a_World),
	m_BlockMeta(a_BlockMeta),
	m_IsDestroyed(false),
	m_IsBrewing(false),
	m_TimeBrewed(0)
{
	m_Contents.AddListener(*this);
}





cBrewingstandEntity::~cBrewingstandEntity()
{
	// Tell window its owner is destroyed
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





void cBrewingstandEntity::UsedBy(cPlayer * a_Player)
{
	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenWindow(new cBrewingstandWindow(m_PosX, m_PosY, m_PosZ, this));
		Window = GetWindow();
	}

	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(Window);
		}
	}

	BroadcastProgress(0, 0);
}





bool cBrewingstandEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);

	if (!m_IsBrewing)
	{
		return false;
	}

	// The necessary brewing time, has been reached
	if (m_TimeBrewed >= m_NeedBrewingTime)
	{
		BroadcastProgress(0, 0);
		m_IsBrewing = false;
		m_TimeBrewed = 0;

		// Decrease item count, full stacks are allowed in the ingredient slot
		cItem Ingredient = m_Contents.GetSlot(3);
		Ingredient.m_ItemCount -= 1;
		m_Contents.SetSlot(3, Ingredient);

		// Loop over all bottle slots and update available bottles
		for (int i = 0; i < 3;i++)
		{
			if (m_Contents.GetSlot(i).IsEmpty())
			{
				continue;
			}

			// Update bottles to the new potion
			cBrewingRecipe * BR = cRoot::Get()->GetBrewingRecipe();
			const cBrewingRecipe::cRecipe * Recipe = BR->GetRecipeFrom(m_Contents.GetSlot(i), m_Contents.GetSlot(3));

			if (Recipe == nullptr)
			{
				// It's allowed to have items in the slots, that result not into a recipe
				continue;
			}

			m_Contents.SetSlot(i, (*Recipe).Output->CopyOne());
		}
		return true;
	}

	m_TimeBrewed++;
	UpdateProgressBars(false);
	return false;
}






void cBrewingstandEntity::SendTo(cClientHandle & a_Client)
{
	// Nothing needs to be sent
	UNUSED(a_Client);
}





void cBrewingstandEntity::BroadcastProgress(short a_ProgressbarID, short a_Value)
{
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->SetProperty(a_ProgressbarID, a_Value);
	}
}






void cBrewingstandEntity::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	super::OnSlotChanged(a_ItemGrid, a_SlotNum);

	if (m_IsDestroyed)
	{
		return;
	}

	ASSERT(a_ItemGrid == &m_Contents);

	// Check if still a item is in the ingredient slot
	if (GetSlot(3).IsEmpty())
	{
		if (m_IsBrewing)
		{
			// Cancel brewing
			BroadcastProgress(0, 0);
			m_IsBrewing = false;
			m_TimeBrewed = 0;
		}
		return;
	}

	// Recheck the bottles
	cBrewingRecipe * BR = cRoot::Get()->GetBrewingRecipe();
	bool Stop = true;
	for (int i = 0; i < 3;i++)
	{
		if (GetSlot(i).IsEmpty())
		{
			continue;
		}

		const cBrewingRecipe::cRecipe * Recipe = BR->GetRecipeFrom(m_Contents.GetSlot(i), m_Contents.GetSlot(3));
		if (Recipe != nullptr)
		{
			// Found a brewing recipe for the items
			Stop = false;
			break;  // If one has been found, no need to check the others
		}
	}

	if (Stop)
	{
		if (m_IsBrewing)
		{
			// Cancel brewing
			BroadcastProgress(0, 0);
			m_IsBrewing = false;
			m_TimeBrewed = 0;
		}
		return;
	}

	// Check if still is brewing
	if (!m_IsBrewing)
	{
		// Start brewing process
		m_IsBrewing = true;
	}
}





void cBrewingstandEntity::UpdateProgressBars(bool a_ForceUpdate)
{
	/** Sending an update every 3th tick, using a higher value lets look the progressbar ugly */
	if (!a_ForceUpdate && (m_World->GetWorldAge() % 3 != 0))
	{
		return;
	}

	BroadcastProgress(0, m_NeedBrewingTime - m_TimeBrewed);
}
