#include "Globals.h"

#include "BehaviorTree.h"
#include "Blackboard.h"

#include "../Monster.h"

#include "../../WorldStorage/NamespaceSerializer.h"
#include "../../World.h"


// This logging is far too noisy to always be on
#ifdef ABC

	template <typename... Args>
	static void BehaviorLog(cBlackboard & a_Blackboard, const char * a_FormatString, Args && ... a_Args)
	{
		auto Msg = fmt::format(a_FormatString, std::forward<Args>(a_Args)...);
		FLOG("{0}: {1}", a_Blackboard.Self().GetUniqueID(), Msg);
	}

	#define BT_LOG(...) BehaviorLog(a_Blackboard, __VA_ARGS__)
#else
	#define BT_LOG(...)
#endif



namespace BehaviorTree
{

const char * to_string(eStatus a_Status)
{
	switch (a_Status)
	{
		case eStatus::bsUntouched: return "Untouched";
		case eStatus::bsSuccess:   return "Success";
		case eStatus::bsFailure:   return "Failure";
		case eStatus::bsRunning:   return "Running";
		case eStatus::bsCancelled: return "Cancelled";
	}
	UNREACHABLE("Invalid BehaviorTree::eStatus");
}





std::unique_ptr<cTask> CreateBehaviourTree(eMonsterType a_MobType, const AString & a_CustomFileName)
{
	AString LuaSubsystemName = "BehaviorTree: ";
	AString FilePath = "MobAi%c%s%s";

	if (!a_CustomFileName.empty())
	{
		LuaSubsystemName += a_CustomFileName;
		FilePath = Printf(FilePath.c_str(), cFile::PathSeparator(), a_CustomFileName.c_str(), ".lua");
	}
	else
	{
		LuaSubsystemName += AString(NamespaceSerializer::From(a_MobType));
		FilePath = Printf(FilePath.c_str(), cFile::PathSeparator(), NamespaceSerializer::From(a_MobType), ".lua");
	}

	auto Lua = cLuaState(LuaSubsystemName);
	Lua.Create();
	cLuaState::cLock lock(Lua);
	Lua.LoadFile(FilePath);

	auto BehavioursLua = Lua.WalkToNamedGlobal("BehaviorTree");

	if (!BehavioursLua.IsValid() || !lua_istable(Lua, 1))
	{
		LOGWARNING("The BehaviorTree file %s doesn't contain any behaviours", FilePath.c_str());
		return nullptr;
	}

	lua_pushnil(Lua);
	lua_next(Lua, -2);

	lua_pushvalue(Lua, -2);
	auto Name = lua_tostring(Lua, -1);
	Lua.Pop(1);

	auto Behaviour = CreateBehaviour(Name, Lua);

	Lua.Pop(2);

	return Behaviour;
}





cTasks CreateChildBehaviours(cLuaState & a_LuaState)
{
	auto Behaviours = cTasks();

	lua_pushnil(a_LuaState);
	while (lua_next(a_LuaState, -2) != 0)
	{

		if (lua_istable(a_LuaState, -1))
		{
			lua_pushvalue(a_LuaState, -2);
			auto Name = lua_tostring(a_LuaState, -1);
			a_LuaState.Pop(1);  // Pops the name
			Behaviours.push_back(CreateBehaviour(Name, a_LuaState));
		}

		a_LuaState.Pop(1);  // Pos the entry
	}

	return Behaviours;
}





std::unique_ptr<cTask> CreateBehaviour(const AString & a_BehaviourName, cLuaState & a_LuaState)
{
	if (!lua_istable(a_LuaState, -1))
	{
		LOGWARNING("Behaviour %s is missing its setup values", a_BehaviourName);
		return nullptr;
	}

	if (NoCaseCompare(a_BehaviourName, "DynamicGuardSelector") == 0)
	{
		return std::make_unique<cDynamicGuardSelector>(a_LuaState);
	}
	else if (NoCaseCompare(a_BehaviourName, "HealthRange") == 0)
	{
		return std::make_unique<cHealthRange>(a_LuaState);
	}
	else if (NoCaseCompare(a_BehaviourName, "MoveToPosition") == 0)
	{
		return std::make_unique<cMoveToPosition>(a_LuaState);
	}
	else if (NoCaseCompare(a_BehaviourName, "RandomPosition") == 0)
	{
		return std::make_unique<cRandomPosition>(a_LuaState);
	}
	else if (NoCaseCompare(a_BehaviourName, "RandomWait") == 0)
	{
		return std::make_unique<cRandomWait>(a_LuaState);
	}
	else if (NoCaseCompare(a_BehaviourName, "Sequence") == 0)
	{
		return std::make_unique<cSequence>(a_LuaState);
	}
	else if (NoCaseCompare(a_BehaviourName, "Succeeder") == 0)
	{
		return std::make_unique<cSucceeder>(a_LuaState);
	}
	else
	{
		LOGERROR("Unimplemented behaviour parsed!: %s", a_BehaviourName);
		return nullptr;
	}
}


////////////////////////////////////////////////////////////////////////////////
// cBehavior:

eStatus cTask::Tick(cBlackboard & a_Blackboard)
{
	if (m_Status != eStatus::bsRunning)
	{
		DoEntry(a_Blackboard);
	}

	m_Status = DoTick(a_Blackboard);
	ASSERT(
		((m_Status != eStatus::bsUntouched) && (m_Status != eStatus::bsCancelled)) ||
		!"Invalid status returned from cBehavior::DoTick"
	);

	if (m_Status != eStatus::bsRunning)
	{
		DoExit(a_Blackboard);
	}

	return m_Status;
}





////////////////////////////////////////////////////////////////////////////////
// cGuardedBehavior:

cGuardedTask::cGuardedTask(cLuaState & a_LuaState)
{
	{
		auto IsGuard = a_LuaState.WalkToValue("Guard");
		if (!IsGuard.IsValid())
		{
			m_Guard = nullptr;
		}
		else
		{
			lua_pushnil(a_LuaState);
			lua_next(a_LuaState, -2);

			auto Name = lua_tostring(a_LuaState, -2);
			m_Guard = CreateBehaviour(Name, a_LuaState);

			a_LuaState.Pop(2);
		}
	}

	{
		auto IsTask = a_LuaState.WalkToValue("Task");
		if (!IsTask.IsValid())
		{
			m_Child = nullptr;
		}
		else
		{
			lua_pushnil(a_LuaState);
			lua_next(a_LuaState, -2);

			auto Name = lua_tostring(a_LuaState, -2);
			m_Child = CreateBehaviour(Name, a_LuaState);
			a_LuaState.Pop(2);
		}
	}
}





bool cGuardedTask::EvaluateGuard(cBlackboard & a_Blackboard)
{
	if (m_Guard == nullptr)
	{
		return true;
	}

	auto Status = m_Guard->Tick(a_Blackboard);
	switch (Status)
	{
		case eStatus::bsSuccess: return true;
		case eStatus::bsFailure: return false;
		default:
		{
			FLOGERROR("Invalid status returned from guard task: {0}", Status);
			m_Guard->Cancel(a_Blackboard);
			return false;
		}
	}
}





eStatus cGuardedTask::DoTick(cBlackboard & a_Blackboard)
{
	if (EvaluateGuard(a_Blackboard))
	{
		return m_Child->Tick(a_Blackboard);
	}

	m_Child->Cancel(a_Blackboard);
	return eStatus::bsFailure;
}





////////////////////////////////////////////////////////////////////////////////
// cDynamicGuardSelector


cDynamicGuardSelector::cDynamicGuardSelector(cLuaState & a_LuaState)
{
	lua_pushnil(a_LuaState);
	while (lua_next(a_LuaState, -2) != 0)
	{
		if (lua_istable(a_LuaState, -1))
		{
			m_Children.push_back(std::make_unique<cGuardedTask>(a_LuaState));
		}
		a_LuaState.Pop(1);
	}
	m_CurrentTask = m_Children.begin();
}





void cDynamicGuardSelector::DoEntry(cBlackboard & a_Blackboard)
{
	m_CurrentTask = m_Children.begin();
}





eStatus cDynamicGuardSelector::DoTick(cBlackboard & a_Blackboard)
{
	eStatus Status = eStatus::bsFailure;
	auto First  = m_Children.begin();
	const auto Last   = m_Children.end();

	for (;;)
	{
		First = std::find_if(First, Last, [&](const std::unique_ptr<cGuardedTask> & a_Behavior)
			{
				return a_Behavior->EvaluateGuard(a_Blackboard);
			}
		);

		if ((First != m_CurrentTask) && (m_CurrentTask != Last))
		{
			m_CurrentTask->get()->Cancel(a_Blackboard);
			m_CurrentTask = Last;
		}

		if (First == Last)
		{
			break;
		}

		Status = First->get()->m_Child->Tick(a_Blackboard);
		if (Status != eStatus::bsFailure)
		{
			m_CurrentTask = First;
			break;
		}
	}
	return Status;
}

////////////////////////////////////////////////////////////////////////////////
// cSucceeder:

cSucceeder::cSucceeder(cLuaState & a_LuaState)
{
	lua_pushnil(a_LuaState);
	lua_next(a_LuaState, -2);

	lua_pushvalue(a_LuaState, -2);
	auto Name = lua_tostring(a_LuaState, -1);
	a_LuaState.Pop(1);

	m_Child = CreateBehaviour(Name, a_LuaState);

	a_LuaState.Pop(2);
}



////////////////////////////////////////////////////////////////////////////////
// cRandomPosition:

cRandomPosition::cRandomPosition(cLuaState & a_LuaState)
{
	if (!a_LuaState.GetNamedValue("UniqueID", m_Key))
	{
		LOGWARNING("cRandomPosition is missing parameter UniqueID. Using Defaul: \"RandomPosition\".");
		m_Key = "RandomPosition";
	}
	if (!a_LuaState.GetNamedValue("Range", m_MaxRange))
	{
		LOGWARNING("cRandomPosition is missing parameter Range. Setting to 10!");
		m_MaxRange = 10;
	}
}





eStatus cRandomPosition::DoTick(cBlackboard & a_Blackboard)
{
	auto & Random = GetRandomProvider();
	Vector3d Dist;
	Dist.x = Random.RandReal(-m_MaxRange, m_MaxRange);
	Dist.z = Random.RandReal(-m_MaxRange, m_MaxRange);

	Vector3d Destination = a_Blackboard.Self().GetPosition() + Dist;
	Destination.y = Clamp(Destination.y, 0.0, static_cast<double>(cChunkDef::Height));

	Json::Value Position;
	Position["x"] = Destination.x;
	Position["y"] = Destination.y;
	Position["z"] = Destination.z;

	a_Blackboard[m_Key] = std::move(Position);

	return eStatus::bsSuccess;
}





////////////////////////////////////////////////////////////////////////////////
// cMoveToPosition:

cMoveToPosition::cMoveToPosition(cLuaState & a_LuaState)
{
	if (!a_LuaState.GetNamedValue("UniqueID", m_Key))
	{
		LOGWARNING("cMoveToPosition is missing parameter UniqueID. Using Default: \"RandomPosition\".");
		m_Key = "RandomPosition";
	}
}





void cMoveToPosition::DoEntry(cBlackboard & a_Blackboard)
{
	Vector3d Position;
	auto & JsonPos = a_Blackboard[m_Key];
	Position.x = JsonPos["x"].asDouble();
	Position.y = JsonPos["y"].asDouble();
	Position.z = JsonPos["z"].asDouble();

	BT_LOG("Moving to position ({0}, {1}, {2})", Position.x, Position.y, Position.z);
	a_Blackboard.Self().MoveToPosition(Position);
}





eStatus cMoveToPosition::DoTick(cBlackboard & a_Blackboard)
{
	auto & Self = a_Blackboard.Self();
	if (Self.ReachedFinalDestination())
	{
		BT_LOG("Reached destination");
		return eStatus::bsSuccess;
	}

	if (!Self.m_PathfinderActivated)
	{
		// Path not found
		BT_LOG("Could not reach destination");
		return eStatus::bsFailure;
	}

	// We're on our way
	return eStatus::bsRunning;
}





void cMoveToPosition::DoExit(cBlackboard & a_Blackboard)
{
	a_Blackboard.Self().StopMovingToPosition();
}





void cMoveToPosition::DoCancel(cBlackboard & a_Blackboard)
{
	a_Blackboard.Self().StopMovingToPosition();
}





////////////////////////////////////////////////////////////////////////////////
// cRandomWait:

cRandomWait::cRandomWait(cLuaState & a_LuaState)
{
	int Min = 0, Max = 1;
	if (!a_LuaState.GetNamedValue("Min", Min))
	{
		LOGWARNING("cRandomWait is missing parameter Min. Using Default: 0.");
	}
	m_WaitMin = std::chrono::seconds(Min);

	if (!a_LuaState.GetNamedValue("Max", Max))
	{
		LOGWARNING("cRandomWait is missing parameter Max. Using Default: 1.");
	}
	m_WaitMax = std::chrono::seconds(Max);
}





void cRandomWait::DoEntry(cBlackboard & a_Blackboard)
{
	cTickTimeLong CurrentAge(a_Blackboard.Self().GetWorld()->GetWorldAge());
	cTickTime Wait(GetRandomProvider().RandInt(m_WaitMin.count(), m_WaitMax.count()));

	BT_LOG("Waiting for {0} ticks", Wait.count());
	m_WaitEnd = CurrentAge + Wait;
}





eStatus cRandomWait::DoTick(cBlackboard & a_Blackboard)
{
	cTickTimeLong CurrentAge(a_Blackboard.Self().GetWorld()->GetWorldAge());

	return (m_WaitEnd <= CurrentAge) ? eStatus::bsSuccess : eStatus::bsRunning;
}





////////////////////////////////////////////////////////////////////////////////
// cHealthRange:



cHealthRange::cHealthRange(cLuaState & a_LuaState)
{
	if (!a_LuaState.GetNamedValue("Max", m_MaxHealth))
	{
		LOGWARNING("cHealthRange is missing parameter Max. Setting to 1.0f!");
		m_MaxHealth = 1.0f;
	}
	if (!a_LuaState.GetNamedValue("Min", m_MinHealth))
	{
		LOGWARNING("cHealthRange is missing parameter Max. Setting to 0!");
		m_MinHealth = 0;
	}
}





eStatus cHealthRange::DoTick(cBlackboard & a_Blackboard)
{
	auto Health = a_Blackboard.Self().GetHealth();
	if ((Health > m_MinHealth) && (Health < m_MaxHealth * a_Blackboard.Self().GetMaxHealth()))
	{
		return eStatus::bsSuccess;
	}

	return eStatus::bsFailure;
}


}  // namespace Behaviors
