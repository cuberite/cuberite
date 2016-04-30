
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BrewingstandEntity.h"
#include "../Bindings/PluginManager.h"
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
	for (int i = 0; i < 3; i++)
	{
		m_Results[i] = cItem{};
	}
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





bool cBrewingstandEntity::UsedBy(cPlayer * a_Player)
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

	if (m_IsBrewing)
	{
		BroadcastProgress(0, m_NeedBrewingTime - m_TimeBrewed);
	}
	else
	{
		BroadcastProgress(0, 0);
	}
	return true;
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
		const cBrewingRecipes::cRecipe * Recipe = nullptr;
		BroadcastProgress(0, 0);
		m_IsBrewing = false;
		m_TimeBrewed = 0;

		// Return if the hook has been canceled
		if (cPluginManager::Get().CallHookBrewingCompleting(*m_World, *this))
		{
			return false;
		}

		// Decrease item count, full stacks are allowed in the ingredient slot
		cItem Ingredient = m_Contents.GetSlot(bsIngredient);
		Ingredient.m_ItemCount -= 1;
		m_Contents.SetSlot(bsIngredient, Ingredient);

		// Loop over all bottle slots and update available bottles
		for (int i = 0; i < 3; i++)
		{
			if (m_Contents.GetSlot(i).IsEmpty() || (m_CurrentBrewingRecipes[i] == nullptr))
			{
				continue;
			}

			Recipe = m_CurrentBrewingRecipes[i];
			m_Contents.SetSlot(i, Recipe->Output->CopyOne());
		}

		// Brewing process completed
		cPluginManager::Get().CallHookBrewingCompleted(*m_World, *this);

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
	if (GetSlot(bsIngredient).IsEmpty())
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
	auto & BR = cRoot::Get()->GetBrewingRecipes();
	const cBrewingRecipes::cRecipe * Recipe = nullptr;
	bool Stop = true;
	for (int i = 0; i < 3; i++)
	{
		if (GetSlot(i).IsEmpty())
		{
			m_CurrentBrewingRecipes[i] = nullptr;
			m_Results[i].Clear();
			continue;
		}

		if (m_CurrentBrewingRecipes[i] != nullptr)
		{
			Recipe = m_CurrentBrewingRecipes[i];
			if (Recipe->Ingredient->IsEqual(GetSlot(bsIngredient)) && Recipe->Input->IsEqual(GetSlot(i)))
			{
				Stop = false;
				continue;
			}
		}

		Recipe = BR.GetRecipeFrom(m_Contents.GetSlot(i), m_Contents.GetSlot(bsIngredient));
		if (Recipe != nullptr)
		{
			// Found a brewing recipe for the items
			m_CurrentBrewingRecipes[i] = Recipe;
			m_Results[i] = Recipe->Output->CopyOne();
			Stop = false;
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

	// Start brewing process, if not running
	if (!m_IsBrewing)
	{
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





void cBrewingstandEntity::setTimeBrewed(short a_TimeBrewed)
{
	m_TimeBrewed = a_TimeBrewed;
}





void cBrewingstandEntity::ContinueBrewing(void)
{
	// Continue brewing if number is greater than 0
	if (m_TimeBrewed > 0)
	{
		m_IsBrewing = true;
	}
}





void cBrewingstandEntity::GetRecipes(void)
{
	if (GetSlot(3).IsEmpty())
	{
		return;
	}

	auto & BR = cRoot::Get()->GetBrewingRecipes();
	const cBrewingRecipes::cRecipe * Recipe = nullptr;
	for (int i = 0; i < 3; i++)
	{
		if (GetSlot(i).IsEmpty())
		{
			continue;
		}
		Recipe = BR.GetRecipeFrom(GetSlot(i), GetSlot(bsIngredient));
		if (Recipe != nullptr)
		{
			m_CurrentBrewingRecipes[i] = Recipe;
			m_Results[i] = Recipe->Output->CopyOne();
		}
	}
}





