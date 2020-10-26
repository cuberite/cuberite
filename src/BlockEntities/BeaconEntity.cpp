
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BeaconEntity.h"
#include "../BlockInfo.h"
#include "../BlockArea.h"
#include "../Entities/Player.h"
#include "../UI/BeaconWindow.h"
#include "../ClientHandle.h"





cBeaconEntity::cBeaconEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, 1, 1, a_World),
	m_IsActive(false),
	m_BeaconLevel(0),
	m_PrimaryEffect(cEntityEffect::effNoEffect),
	m_SecondaryEffect(cEntityEffect::effNoEffect)
{
	ASSERT(a_BlockType == E_BLOCK_BEACON);
	if (m_World != nullptr)
	{
		UpdateBeacon();
	}
}





char cBeaconEntity::CalculatePyramidLevel(void)
{
	cBlockArea Area;
	int MinY = std::max(GetPosY() - 4, 0);
	int MaxY = std::max(GetPosY() - 1, 0);

	Area.Read(
		*m_World,
		GetPosX() - 4, GetPosX() + 4,
		MinY, MaxY,
		GetPosZ() - 4, GetPosZ() + 4,
		cBlockArea::baTypes
	);

	int Layer = 1;
	int MiddleXZ = 4;

	for (int Y = (Area.GetSizeY() - 1); Y >= 0; Y--)
	{
		for (int X = MiddleXZ - Layer; X <= (MiddleXZ + Layer); X++)
		{
			for (int Z = MiddleXZ - Layer; Z <= (MiddleXZ + Layer); Z++)
			{
				if (!IsMineralBlock(Area.GetRelBlockType(X, Y, Z)))
				{
					return static_cast<char>(Layer - 1);
				}
			}
		}
		Layer++;
	}

	return static_cast<char>(Layer - 1);
}





bool cBeaconEntity::IsValidEffect(cEntityEffect::eType a_Effect, char a_BeaconLevel)
{
	switch (a_Effect)
	{
		case cEntityEffect::effRegeneration: return (a_BeaconLevel >= 4);
		case cEntityEffect::effStrength:     return (a_BeaconLevel >= 3);
		case cEntityEffect::effResistance:   return (a_BeaconLevel >= 2);
		case cEntityEffect::effJumpBoost:    return (a_BeaconLevel >= 2);
		case cEntityEffect::effSpeed:        return (a_BeaconLevel >= 1);
		case cEntityEffect::effHaste:        return (a_BeaconLevel >= 1);
		case cEntityEffect::effNoEffect:     return true;

		default:
		{
			LOGD("%s: Invalid beacon effect: %d", __FUNCTION__, static_cast<int>(a_Effect));
			return false;
		}
	}
}





bool cBeaconEntity::SetPrimaryEffect(cEntityEffect::eType a_Effect)
{
	if (!IsValidEffect(a_Effect, m_BeaconLevel))
	{
		m_PrimaryEffect = cEntityEffect::effNoEffect;
		return false;
	}

	m_PrimaryEffect = a_Effect;

	// Send window update:
	if (GetWindow() != nullptr)
	{
		GetWindow()->SetProperty(1, m_PrimaryEffect);
	}
	return true;
}





bool cBeaconEntity::SetSecondaryEffect(cEntityEffect::eType a_Effect)
{
	if (!IsValidEffect(a_Effect, m_BeaconLevel))
	{
		m_SecondaryEffect = cEntityEffect::effNoEffect;
		return false;
	}

	m_SecondaryEffect = a_Effect;

	// Send window update:
	if (GetWindow() != nullptr)
	{
		GetWindow()->SetProperty(2, m_SecondaryEffect);
	}
	return true;
}





bool cBeaconEntity::IsBeaconBlocked(void)
{
	for (int Y = m_Pos.y; Y < cChunkDef::Height; ++Y)
	{
		BLOCKTYPE Block = m_World->GetBlock({m_Pos.x, Y, m_Pos.z});
		if (!cBlockInfo::IsTransparent(Block))
		{
			return true;
		}
	}
	return false;
}





bool cBeaconEntity::IsMineralBlock(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_DIAMOND_BLOCK:
		case E_BLOCK_GOLD_BLOCK:
		case E_BLOCK_IRON_BLOCK:
		case E_BLOCK_EMERALD_BLOCK:
		{
			return true;
		}
	}
	return false;
}





void cBeaconEntity::UpdateBeacon(void)
{
	int OldBeaconLevel = m_BeaconLevel;

	if (IsBeaconBlocked())
	{
		m_IsActive = false;
		m_BeaconLevel = 0;
	}
	else
	{
		m_BeaconLevel = CalculatePyramidLevel();
		m_IsActive = (m_BeaconLevel > 0);
	}

	if ((m_BeaconLevel != OldBeaconLevel) && (m_BeaconLevel == 4))
	{
		// Send window update:
		if (GetWindow() != nullptr)
		{
			GetWindow()->SetProperty(0, m_BeaconLevel);
		}

		Vector3d BeaconPosition(m_Pos);
		GetWorld()->ForEachPlayer([=](cPlayer & a_Player)
			{
				Vector3d Distance = BeaconPosition - a_Player.GetPosition();
				if (
					(std::abs(Distance.y) <= 14) &&
					(std::abs(Distance.x) <= 20) &&
					(std::abs(Distance.z) <= 20)
				)
				{
					a_Player.AwardAchievement(Statistic::AchFullBeacon);
				}
				return false;
			}
		);
	}
}





void cBeaconEntity::GiveEffects(void)
{
	if (!m_IsActive || (m_BeaconLevel < 0))
	{
		return;
	}

	double Radius = static_cast<double>(m_BeaconLevel) * 10 + 10;
	short EffectLevel = 0;
	if ((m_BeaconLevel >= 4) && (m_PrimaryEffect == m_SecondaryEffect))
	{
		EffectLevel = 1;
	}

	bool HasSecondaryEffect = (m_BeaconLevel >= 4) && (m_PrimaryEffect != m_SecondaryEffect) && (m_SecondaryEffect > 0);

	auto Area = cBoundingBox(m_Pos, Radius, Radius + static_cast<double>(cChunkDef::Height), -Radius);
	GetWorld()->ForEachEntityInBox(Area, [&](cEntity & a_Entity)
	{
		if (!a_Entity.IsPlayer())
		{
			return false;
		}
		auto & Player = static_cast<cPlayer &>(a_Entity);
		Player.AddEntityEffect(m_PrimaryEffect, 180, EffectLevel);

		if (HasSecondaryEffect)
		{
			Player.AddEntityEffect(m_SecondaryEffect, 180, 0);
		}
		return false;
	});
}





void cBeaconEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cBeaconEntity &>(a_Src);
	m_BeaconLevel = src.m_BeaconLevel;
	m_Contents.CopyFrom(src.m_Contents);
	m_IsActive = src.m_IsActive;
	m_PrimaryEffect = src.m_PrimaryEffect;
	m_SecondaryEffect = src.m_SecondaryEffect;
}





void cBeaconEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





bool cBeaconEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	// Update the beacon every 4 seconds
	if ((GetWorld()->GetWorldAge() % 80) == 0)
	{
		UpdateBeacon();
		GiveEffects();
	}
	return false;
}





bool cBeaconEntity::UsedBy(cPlayer * a_Player)
{
	a_Player->GetStatManager().AddValue(Statistic::InteractWithBeacon);

	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenWindow(new cBeaconWindow(this));
		Window = GetWindow();
	}

	if (Window != nullptr)
	{
		// if (a_Player->GetWindow() != Window)
		// -> Because mojang doesn't send a 'close window' packet when you click the cancel button in the beacon inventory ...
		{
			a_Player->OpenWindow(*Window);
		}
	}
	return true;
}



