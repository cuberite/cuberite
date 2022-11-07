
#include "BreedingItemGuard.h"

namespace BehaviorTree
{
	cBreedingItemGuard::cBreedingItemGuard(cLuaState &a_LuaState)
	{
		AString ItemString;
		if (!a_LuaState.GetNamedValue("Item", ItemString))
		{
			LOGWARNING("cBreedingItemGuard is missing parameter Item. Setting to Air, no action will take place!");
			m_Item = E_BLOCK_AIR;
			return;
		}
		cItem Storage;
		if (!StringToItem(ItemString, Storage))
		{
			FLOGWARNING("cBreedingItemGuard received a invalid item string: %s. Using air instead, no action will take place!", ItemString);
			m_Item = E_BLOCK_AIR;
			return;
		}

		m_Item = Storage.m_ItemType;

		if (!a_LuaState.GetNamedValue("Range", m_Range))
		{
			LOGWARNING("cBreedingItemGuard is missing parameter Range. Setting to 5!");
			m_Range = 5;
		}

	}

	eStatus cBreedingItemGuard::DoTick(cBlackboard &a_Blackboard)
	{
		if (m_Item == E_BLOCK_AIR)
		{
			return eStatus::bsFailure;
		}

		auto World = a_Blackboard.Self().GetWorld();
		if (World == nullptr)
		{
			return eStatus::bsFailure;
		}

		Vector3d Dest;

		World->ForEachEntityInBox(cBoundingBox(a_Blackboard.Self().GetPosition(), m_Range, m_Range), [&] (cEntity & a_Entity)
		{
			if (a_Entity.GetEntityType() != cEntity::etPlayer)
			{
				return true;
			}

			auto & Player = static_cast<cPlayer &>(a_Entity);

			if (Player.GetEquippedItem().m_ItemType != m_Item)
			{
				return true;
			}

			if ((a_Blackboard.Self().GetPosition() - Dest).SqrLength() > (a_Blackboard.Self().GetPosition() - a_Entity.GetPosition()).SqrLength())
			{
				Dest = a_Entity.GetPosition();
			}

			return true;
		});

		Json::Value VectorPos;
		VectorPos["x"] = Dest.x;
		VectorPos["y"] = Dest.y;
		VectorPos["z"] = Dest.z;
		a_Blackboard["FeederPosition"] = VectorPos;

		return eStatus::bsSuccess;
	}
}
