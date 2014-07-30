
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BeaconEntity.h"
#include "../BlockArea.h"
#include "../Entities/Player.h"





cBeaconEntity::cBeaconEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
	: super(E_BLOCK_BEACON, a_BlockX, a_BlockY, a_BlockZ, 1, 1, a_World)
	, m_IsActive(false)
	, m_BeaconLevel(0)
	, m_PrimaryPotion(cEntityEffect::effNoEffect)
	, m_SecondaryPotion(cEntityEffect::effNoEffect)
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





bool cBeaconEntity::IsValidPotion(cEntityEffect::eType a_Potion, char a_BeaconLevel)
{
	if (a_Potion == cEntityEffect::effNoEffect)
	{
		return true;
	}

	switch (a_BeaconLevel)
	{
		case 4:
		{
			// Beacon level 4
			if (a_Potion == cEntityEffect::effRegeneration)
			{
				return true;
			}
		}
		case 3:
		{
			// Beacon level 3
			if (a_Potion == cEntityEffect::effStrength)
			{
				return true;
			}
		}
		case 2:
		{
			// Beacon level 2
			switch (a_Potion)
			{
				case cEntityEffect::effResistance:
				case cEntityEffect::effJumpBoost:
				{
					return true;
				}
			}
		}
		case 1:
		{
			// Beacon level 1
			switch (a_Potion)
			{
				case cEntityEffect::effSpeed:
				case cEntityEffect::effHaste:
				{
					return true;
				}
			}
		}
	}
	return false;
}





bool cBeaconEntity::SelectPrimaryPotion(cEntityEffect::eType a_Potion)
{
	if (!IsValidPotion(a_Potion, m_BeaconLevel))
	{
		return false;
	}

	m_PrimaryPotion = a_Potion;
	return true;
}





bool cBeaconEntity::SelectSecondaryPotion(cEntityEffect::eType a_Potion)
{
	if (!IsValidPotion(a_Potion, m_BeaconLevel))
	{
		return false;
	}

	m_SecondaryPotion = a_Potion;
	return true;
}





bool cBeaconEntity::IsBeaconBlocked(void)
{
	bool IsBlocked = false;
	for (int Y = m_PosY; Y < cChunkDef::Height; ++Y)
	{
		BLOCKTYPE Block = m_World->GetBlock(m_PosX, Y, m_PosZ);
		if (!cBlockInfo::IsTransparent(Block))
		{
			IsBlocked = true;
			break;
		}
	}
	return IsBlocked;
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
	if ((m_BeaconLevel >= 4) && (m_PrimaryPotion == m_SecondaryPotion))
	{
		EffectLevel = 1;
	}

	cEntityEffect::eType SecondaryPotion = cEntityEffect::effNoEffect;
	if ((m_BeaconLevel >= 4) && (m_PrimaryPotion != m_SecondaryPotion) && (m_SecondaryPotion > 0))
	{
		SecondaryPotion = m_SecondaryPotion;
	}

	class cPlayerCallback : public cPlayerListCallback
	{
		int m_Radius;
		int m_PosX, m_PosY, m_PosZ;
		cEntityEffect::eType m_PrimaryPotion, m_SecondaryPotion;
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
				a_Player->AddEntityEffect(m_PrimaryPotion, 180, m_EffectLevel);

				if (m_SecondaryPotion != cEntityEffect::effNoEffect)
				{
					a_Player->AddEntityEffect(m_SecondaryPotion, 180, 0);
				}
			}
			return false;
		}

	public:
		cPlayerCallback(int a_Radius, int a_PosX, int a_PosY, int a_PosZ, cEntityEffect::eType a_PrimaryPotion, cEntityEffect::eType a_SecondaryPotion, short a_EffectLevel)
			: m_Radius(a_Radius)
			, m_PosX(a_PosX)
			, m_PosY(a_PosY)
			, m_PosZ(a_PosZ)
			, m_PrimaryPotion(a_PrimaryPotion)
			, m_SecondaryPotion(a_SecondaryPotion)
			, m_EffectLevel(a_EffectLevel)
		{};

	} PlayerCallback(Radius, m_PosX, m_PosY, m_PosZ, m_PrimaryPotion, SecondaryPotion, EffectLevel);
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
	int PrimaryPotion = a_Value.get("PrimaryPotion", 0).asInt();
	int SecondaryPotion = a_Value.get("SecondaryPotion", 0).asInt();

	if ((PrimaryPotion >= 0) && (PrimaryPotion <= (int)cEntityEffect::effSaturation))
	{
		m_PrimaryPotion = (cEntityEffect::eType)PrimaryPotion;
	}

	if ((SecondaryPotion >= 0) && (SecondaryPotion <= (int)cEntityEffect::effSaturation))
	{
		m_SecondaryPotion = (cEntityEffect::eType)SecondaryPotion;
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
	a_Value["PrimaryPotion"] = (int)m_PrimaryPotion;
	a_Value["SecondaryPotion"] = (int)m_SecondaryPotion;
}





void cBeaconEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}




