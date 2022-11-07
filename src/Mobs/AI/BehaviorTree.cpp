#include "Globals.h"

#include "BehaviorTree.h"
#include "Blackboard.h"

#include "../Monster.h"

#include "../../WorldStorage/NamespaceSerializer.h"
#include "../../World.h"

#include "Mobs/AI/Behaviours/IncludeAllBehaviours.h"

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

	// If file does not exist no behaviour
	if (!cFile::IsFile(FilePath))
	{
		return nullptr;
	}

	auto Lua = cLuaState(LuaSubsystemName);
	Lua.Create();
	cLuaState::cLock lock(Lua);
	Lua.LoadFile(FilePath);

	auto BehavioursLua = Lua.WalkToNamedGlobal("BehaviorTree");

	if (!BehavioursLua.IsValid() || !lua_istable(Lua, 1))
	{
		LOGWARNING("The BehaviorTree file %s doesn't contain any valid behaviours", FilePath.c_str());
		return nullptr;
	}

	lua_pushnil(Lua);
	lua_next(Lua, -2);

	// There is no behaviour
	if (lua_gettop(Lua) == 1)
	{
		return nullptr;
	}

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




/** Newly created cTasks need to be added here */
std::unique_ptr<cTask> CreateBehaviour(const AString & a_BehaviourName, cLuaState & a_LuaState)
{
	if (!lua_istable(a_LuaState, -1))
	{
		LOGWARNING("Behaviour %s is missing its setup values", a_BehaviourName);
		return nullptr;
	}

	if (NoCaseCompare(a_BehaviourName, "BreedingItemGuard") == 0)
	{
		return std::make_unique<cBreedingItemGuard>(a_LuaState);
	}
	else if (NoCaseCompare(a_BehaviourName, "DynamicGuardSelector") == 0)
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
		ASSERT(!"Unimplemented behaviour parsed!");
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

}  // namespace BehaviorTree
