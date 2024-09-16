
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Sheep.h"
#include "../Entities/Player.h"
#include "../World.h"
#include "../EffectID.h"
#include "../FastRandom.h"
#include "Items/ItemDye.h"


cSheep::cSheep(int a_Color) :
	Super("Sheep", mtSheep, "entity.sheep.hurt", "entity.sheep.death", "entity.sheep.ambient", 0.9f, 1.3f),
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
		a_Drops.emplace_back(GetItemFromColor(m_WoolColor));
	}

	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 1, 3 + LootingLevel, IsOnFire() ? Item::CookedMutton : Item::Mutton);
}





void cSheep::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	const cItem & EquippedItem = a_Player.GetEquippedItem();
	if ((EquippedItem.m_ItemType == Item::Shears) && !IsSheared() && !IsBaby())
	{
		m_IsSheared = true;
		m_World->BroadcastEntityMetadata(*this);
		a_Player.UseEquippedItem();

		cItems Drops;
		char NumDrops = GetRandomProvider().RandInt<char>(1, 3);
		Drops.emplace_back(GetItemFromColor(m_WoolColor), NumDrops);
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
		m_World->BroadcastSoundEffect("entity.sheep.shear", GetPosition(), 1.0f, 1.0f);
	}
	else if (cItemDyeHandler::IsDye(EquippedItem) && (m_WoolColor != 15 - EquippedItem.m_ItemDamage))
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
	auto Pos = POS_TOINT.addedY(-1);

	if (!cChunkDef::IsValidHeight(Pos.y))
	{
		return;
	}

	if (m_TimeToStopEating > 0)
	{
		StopMovingToPosition();
		m_TimeToStopEating--;

		if (m_TimeToStopEating == 0)
		{
			if (m_World->GetBlock(Pos).Type() == BlockType::GrassBlock)  // Make sure grass hasn't been destroyed in the meantime
			{
				// The sheep ate the grass so we change it to dirt
				m_World->SetBlock(Pos, Block::Dirt::Dirt());
				GetWorld()->BroadcastSoundParticleEffect(EffectID::PARTICLE_BLOCK_BREAK, Pos, 2);  // This is a magic number corresponding to the old value of the grass block
				m_IsSheared = false;
				m_World->BroadcastEntityMetadata(*this);
			}
		}
	}
	else
	{
		if (GetRandomProvider().RandBool(1.0 / 600.0))
		{
			if (m_World->GetBlock(Pos).Type() == BlockType::GrassBlock)
			{
				m_World->BroadcastEntityAnimation(*this, EntityAnimation::SheepEatsGrass);
				m_TimeToStopEating = 40;
			}
		}
	}
}





void cSheep::InheritFromParents(cMonster * a_Parent1, cMonster * a_Parent2)
{
	static const struct
	{
		eSheepColor Parent1, Parent2, Child;
	} ColorInheritance[] =
	{
		{ eSheepColor::Blue,   eSheepColor::Red,   eSheepColor::Purple     },
		{ eSheepColor::Blue,   eSheepColor::Green, eSheepColor::Cyan       },
		{ eSheepColor::Yellow, eSheepColor::Red,   eSheepColor::Orange     },
		{ eSheepColor::Green,  eSheepColor::White, eSheepColor::Lime       },
		{ eSheepColor::Red,    eSheepColor::White, eSheepColor::Pink       },
		{ eSheepColor::White,  eSheepColor::Black, eSheepColor::Gray       },
		{ eSheepColor::Purple, eSheepColor::Pink,  eSheepColor::Magenta    },
		{ eSheepColor::White,  eSheepColor::Gray,  eSheepColor::LightGray  },
		{ eSheepColor::Blue,   eSheepColor::White, eSheepColor::LightBlue  },
	};
	cSheep * Parent1 = static_cast<cSheep *>(a_Parent1);
	cSheep * Parent2 = static_cast<cSheep *>(a_Parent2);
	for (size_t i = 0; i < ARRAYCOUNT(ColorInheritance); i++)
	{
		if (
			((Parent1->GetFurColorEnum() == ColorInheritance[i].Parent1) && (Parent2->GetFurColorEnum() == ColorInheritance[i].Parent2)) ||
			((Parent1->GetFurColorEnum() == ColorInheritance[i].Parent2) && (Parent2->GetFurColorEnum() == ColorInheritance[i].Parent1))
		)
		{
			SetFurColor(ColorInheritance[i].Child);
			return;
		}
	}
	SetFurColor(GetRandomProvider().RandBool() ? Parent1->GetFurColor() : Parent2->GetFurColor());
}





unsigned char cSheep::GenerateNaturalRandomColor(void)
{
	int Chance = GetRandomProvider().RandInt(100);

	if (Chance <= 81)
	{
		return static_cast<unsigned char>(eSheepColor::White);
	}
	else if (Chance <= 86)
	{
		return static_cast<unsigned char>(eSheepColor::Black);
	}
	else if (Chance <= 91)
	{
		return static_cast<unsigned char>(eSheepColor::Gray);
	}
	else if (Chance <= 96)
	{
		return static_cast<unsigned char>(eSheepColor::LightGray);
	}
	else if (Chance <= 99)
	{
		return static_cast<unsigned char>(eSheepColor::Brown);
	}
	else
	{
		return static_cast<unsigned char>(eSheepColor::Pink);
	}
}





Item cSheep::GetItemFromColor(int a_Color)
{
	// TODO(12xx12): replace number with enum constant
	switch (a_Color)
	{
		case 0: return Item::WhiteWool;
		case 1: return Item::OrangeWool;
		case 2: return Item::MagentaWool;
		case 3: return Item::LightBlueWool;
		case 4: return Item::YellowWool;
		case 5: return Item::LimeWool;
		case 6: return Item::PinkWool;
		case 7: return Item::GrayWool;
		case 8: return Item::LightGrayWool;
		case 9: return Item::CyanWool;
		case 10: return Item::PurpleWool;
		case 11: return Item::BlueWool;
		case 12: return Item::BrownWool;
		case 13: return Item::GreenWool;
		case 14: return Item::RedWool;
		case 15: return Item::BlackWool;
		default: return Item::WhiteWool;
	}
}
