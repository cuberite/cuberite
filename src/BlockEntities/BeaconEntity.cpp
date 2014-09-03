
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BeaconEntity.h"
#include "../BlockArea.h"
#include "../Entities/Player.h"





cBeaconEntity::cBeaconEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
	: super(E_BLOCK_BEACON, a_BlockX, a_BlockY, a_BlockZ, 1, 1, a_World)
	, m_IsActive(false)
	, m_BeaconLevel(0)
	, m_PrimaryEffect(cEntityEffect::effNoEffect)
	, m_SecondaryEffect(cEntityEffect::effNoEffect)
{
	UpdateBeacon();
}





char cBeaconEntity::CalculatePyramidLevel(void)
{
	cBlockArea Area;
	int MinY = std::max(GetPosY() - 4, 0);
	int MaxY = std::max(GetPosY() - 1, 0);

	Area.Read(
		m_World,
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
					return (Layer - 1);
				}
			}
		}
		Layer++;
	}

	return (Layer - 1);
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
			LOGD("%s: Invalid beacon effect: %d", __FUNCTION__, (int)a_Effect);
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
	if (GetWindow() != NULL)
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
	if (GetWindow() != NULL)
	{
		GetWindow()->SetProperty(2, m_SecondaryEffect);
	}
	return true;
}





bool cBeaconEntity::IsBeaconBlocked(void)
{
	for (int Y = m_PosY; Y < cChunkDef::Height; ++Y)
	{
		BLOCKTYPE Block = m_World->GetBlock(m_PosX, Y, m_PosZ);
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

	if (m_BeaconLevel != OldBeaconLevel)
	{
		// Send window update:
		if (GetWindow() != NULL)
		{
			GetWindow()->SetProperty(0, m_BeaconLevel);
		}
	}

	// TODO: Add achievement
}





void cBeaconEntity::GiveEffects(void)
{
	if (!m_IsActive || (m_BeaconLevel < 0))
	{
		return;
	}

	int Radius = m_BeaconLevel * 10 + 10;
	short EffectLevel = 0;
	if ((m_BeaconLevel >= 4) && (m_PrimaryEffect == m_SecondaryEffect))
	{
		EffectLevel = 1;
	}

	cEntityEffect::eType SecondaryEffect = cEntityEffect::effNoEffect;
	if ((m_BeaconLevel >= 4) && (m_PrimaryEffect != m_SecondaryEffect) && (m_SecondaryEffect > 0))
	{
		SecondaryEffect = m_SecondaryEffect;
	}

	class cPlayerCallback : public cPlayerListCallback
	{
		int m_Radius;
		int m_PosX, m_PosY, m_PosZ;
		cEntityEffect::eType m_PrimaryEffect, m_SecondaryEffect;
		short m_EffectLevel;

		virtual bool Item(cPlayer * a_Player)
		{
			Vector3d PlayerPosition = Vector3d(a_Player->GetPosition());
			if (PlayerPosition.y > (double)m_PosY)
			{
				PlayerPosition.y = (double)m_PosY;
			}

			// TODO: Vanilla minecraft uses an AABB check instead of a radius one
			Vector3d BeaconPosition = Vector3d(m_PosX, m_PosY, m_PosZ);
			if ((PlayerPosition - BeaconPosition).Length() <= m_Radius)
			{
				a_Player->AddEntityEffect(m_PrimaryEffect, 180, m_EffectLevel);

				if (m_SecondaryEffect != cEntityEffect::effNoEffect)
				{
					a_Player->AddEntityEffect(m_SecondaryEffect, 180, 0);
				}
			}
			return false;
		}

	public:
		cPlayerCallback(int a_Radius, int a_PosX, int a_PosY, int a_PosZ, cEntityEffect::eType a_PrimaryEffect, cEntityEffect::eType a_SecondaryEffect, short a_EffectLevel)
			: m_Radius(a_Radius)
			, m_PosX(a_PosX)
			, m_PosY(a_PosY)
			, m_PosZ(a_PosZ)
			, m_PrimaryEffect(a_PrimaryEffect)
			, m_SecondaryEffect(a_SecondaryEffect)
			, m_EffectLevel(a_EffectLevel)
		{};

	} PlayerCallback(Radius, m_PosX, m_PosY, m_PosZ, m_PrimaryEffect, SecondaryEffect, EffectLevel);
	GetWorld()->ForEachPlayer(PlayerCallback);
}





bool cBeaconEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	// Update the beacon every 4 seconds
	if ((GetWorld()->GetWorldAge() % 80) == 0)
	{
		UpdateBeacon();
		GiveEffects();
	}
	return false;
}





void cBeaconEntity::UsedBy(cPlayer * a_Player)
{
	cWindow * Window = GetWindow();
	if (Window == NULL)
	{
		OpenWindow(new cBeaconWindow(m_PosX, m_PosY, m_PosZ, this));
		Window = GetWindow();
	}
	
	if (Window != NULL)
	{
		// if (a_Player->GetWindow() != Window)
		// -> Because mojang doesn't send a 'close window' packet when you click the cancel button in the beacon inventory ...
		{
			a_Player->OpenWindow(Window);
		}
	}
}





bool cBeaconEntity::LoadFromJson(const Json::Value & a_Value)
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

	m_BeaconLevel = (char)a_Value.get("Level", 0).asInt();
	int PrimaryEffect = a_Value.get("PrimaryEffect", 0).asInt();
	int SecondaryEffect = a_Value.get("SecondaryEffect", 0).asInt();

	if ((PrimaryEffect >= 0) && (PrimaryEffect <= (int)cEntityEffect::effSaturation))
	{
		m_PrimaryEffect = (cEntityEffect::eType)PrimaryEffect;
	}

	if ((SecondaryEffect >= 0) && (SecondaryEffect <= (int)cEntityEffect::effSaturation))
	{
		m_SecondaryEffect = (cEntityEffect::eType)SecondaryEffect;
	}

	return true;
}





void cBeaconEntity::SaveToJson(Json::Value& a_Value)
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

	a_Value["Level"] = m_BeaconLevel;
	a_Value["PrimaryEffect"] = (int)m_PrimaryEffect;
	a_Value["SecondaryEffect"] = (int)m_SecondaryEffect;
}





void cBeaconEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}




