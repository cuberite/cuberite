
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Sheep.h"
#include "../Entities/Player.h"
#include "../World.h"
#include "../EffectID.h"
#include "../FastRandom.h"





cSheep::cSheep(int a_Color) :
	Super("Sheep", mtSheep, "entity.sheep.hurt", "entity.sheep.death", "entity.sheep.ambient", 0.6, 1.3),
	m_IsSheared(false),
	m_WoolColor(a_Color),
	m_TimeToStopEating(-1)
{
	// Generate random wool color.
	if (m_WoolColor == -1)
	{
		m_WoolColor = GenerateNaturalRandomColor();
	}

	if ((m_WoolColor < 0) || (m_WoolColor > 15))
	{
		m_WoolColor = 0;
	}
}





void cSheep::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (IsBaby())
	{
		return;  // Babies don't drop items
	}

	if (!m_IsSheared)
	{
		a_Drops.push_back(cItem(E_BLOCK_WOOL, 1, static_cast<short>(m_WoolColor)));
	}

	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 1, 3 + LootingLevel, IsOnFire() ? E_ITEM_COOKED_MUTTON : E_ITEM_RAW_MUTTON);
}





void cSheep::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	const cItem & EquippedItem = a_Player.GetEquippedItem();
	if ((EquippedItem.m_ItemType == E_ITEM_SHEARS) && !IsSheared() && !IsBaby())
	{
		m_IsSheared = true;
		m_World->BroadcastEntityMetadata(*this);
		a_Player.UseEquippedItem();

		cItems Drops;
		char NumDrops = GetRandomProvider().RandInt<char>(1, 3);
		Drops.emplace_back(E_BLOCK_WOOL, NumDrops, static_cast<short>(m_WoolColor));
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
		m_World->BroadcastSoundEffect("entity.sheep.shear", GetPosition(), 1.0f, 1.0f);
	}
	else if ((EquippedItem.m_ItemType == E_ITEM_DYE) && (m_WoolColor != 15 - EquippedItem.m_ItemDamage))
	{
		m_WoolColor = 15 - EquippedItem.m_ItemDamage;
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cSheep::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}
	int PosX = POSX_TOINT;
	int PosY = POSY_TOINT - 1;
	int PosZ = POSZ_TOINT;

	if ((PosY <= 0) || (PosY >= cChunkDef::Height))
	{
		return;
	}

	if (m_TimeToStopEating > 0)
	{
		StopMovingToPosition();
		m_TimeToStopEating--;

		if (m_TimeToStopEating == 0)
		{
			if (m_World->GetBlock(PosX, PosY, PosZ) == E_BLOCK_GRASS)  // Make sure grass hasn't been destroyed in the meantime
			{
				// The sheep ate the grass so we change it to dirt
				m_World->SetBlock(PosX, PosY, PosZ, E_BLOCK_DIRT, 0);
				GetWorld()->BroadcastSoundParticleEffect(EffectID::PARTICLE_BLOCK_BREAK, {PosX, PosY, PosZ}, E_BLOCK_GRASS);
				m_IsSheared = false;
				m_World->BroadcastEntityMetadata(*this);
			}
		}
	}
	else
	{
		if (GetRandomProvider().RandBool(1.0 / 600.0))
		{
			if (m_World->GetBlock(PosX, PosY, PosZ) == E_BLOCK_GRASS)
			{
				m_World->BroadcastEntityStatus(*this, esSheepEating);
				m_TimeToStopEating = 40;
			}
		}
	}
}





void cSheep::InheritFromParents(cMonster * a_Parent1, cMonster * a_Parent2)
{
	static const struct
	{
		short Parent1, Parent2, Child;
	} ColorInheritance[] =
	{
		{ E_META_WOOL_BLUE,   E_META_WOOL_RED,   E_META_WOOL_PURPLE     },
		{ E_META_WOOL_BLUE,   E_META_WOOL_GREEN, E_META_WOOL_CYAN       },
		{ E_META_WOOL_YELLOW, E_META_WOOL_RED,   E_META_WOOL_ORANGE     },
		{ E_META_WOOL_GREEN,  E_META_WOOL_WHITE, E_META_WOOL_LIGHTGREEN },
		{ E_META_WOOL_RED,    E_META_WOOL_WHITE, E_META_WOOL_PINK       },
		{ E_META_WOOL_WHITE,  E_META_WOOL_BLACK, E_META_WOOL_GRAY       },
		{ E_META_WOOL_PURPLE, E_META_WOOL_PINK,  E_META_WOOL_MAGENTA    },
		{ E_META_WOOL_WHITE,  E_META_WOOL_GRAY,  E_META_WOOL_LIGHTGRAY  },
		{ E_META_WOOL_BLUE,   E_META_WOOL_WHITE, E_META_WOOL_LIGHTBLUE  },
	};
	cSheep * Parent1 = static_cast<cSheep *>(a_Parent1);
	cSheep * Parent2 = static_cast<cSheep *>(a_Parent2);
	for (size_t i = 0; i < ARRAYCOUNT(ColorInheritance); i++)
	{
		if (
			((Parent1->GetFurColor() == ColorInheritance[i].Parent1) && (Parent2->GetFurColor() == ColorInheritance[i].Parent2)) ||
			((Parent1->GetFurColor() == ColorInheritance[i].Parent2) && (Parent2->GetFurColor() == ColorInheritance[i].Parent1))
		)
		{
			SetFurColor(ColorInheritance[i].Child);
			return;
		}
	}
	SetFurColor(GetRandomProvider().RandBool() ? Parent1->GetFurColor() : Parent2->GetFurColor());
}





NIBBLETYPE cSheep::GenerateNaturalRandomColor(void)
{
	int Chance = GetRandomProvider().RandInt(100);

	if (Chance <= 81)
	{
		return E_META_WOOL_WHITE;
	}
	else if (Chance <= 86)
	{
		return E_META_WOOL_BLACK;
	}
	else if (Chance <= 91)
	{
		return E_META_WOOL_GRAY;
	}
	else if (Chance <= 96)
	{
		return E_META_WOOL_LIGHTGRAY;
	}
	else if (Chance <= 99)
	{
		return E_META_WOOL_BROWN;
	}
	else
	{
		return E_META_WOOL_PINK;
	}
}
