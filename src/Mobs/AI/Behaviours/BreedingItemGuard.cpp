
#include "BreedingItemGuard.h"
#include "Entities/Player.h"

namespace BehaviorTree
{
	cBreedingItemGuard::cBreedingItemGuard(cLuaState &a_LuaState)
	{
		AString RawItemString;
		if (!a_LuaState.GetNamedValue("Item", RawItemString))
		{
			LOGWARNING("cBreedingItemGuard is missing parameter Item. Setting to Air, no action will take place!");
			return;
		}
		auto ItemStrings = StringSplit(RawItemString, ",");

		cItem Storage;
		for (const auto & ItemString : ItemStrings)
		{
			if (!StringToItem(RawItemString, Storage))
			{
				FLOGWARNING("cBreedingItemGuard received a invalid item string: '%s'. Using air instead!", ItemString);
			}
			m_Items.emplace_back(Storage);
		}

		if (m_Items.empty())
		{
			FLOGWARNING("cBreedingItemGuard received an invalid item string: '%s'! No action will take place", RawItemString);
		}

		if (!a_LuaState.GetNamedValue("Range", m_Range))
		{
			LOGWARNING("cBreedingItemGuard is missing parameter Range. Setting to 5!");
			m_Range = 5;
		}

	}

	eStatus cBreedingItemGuard::DoTick(cBlackboard &a_Blackboard)
	{
		if (m_Items.empty())
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

			const auto & Player = static_cast<const cPlayer &>(a_Entity);

			if (!m_Items.ContainsType(Player.GetEquippedItem()))
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
