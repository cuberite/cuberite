
#pragma once

#include "../../Bindings/LuaState.h"

#include "Blackboard.h"

#include "../MonsterTypes.h"

// Check for logging at the bottom of the file

namespace BehaviorTree
{

enum class eStatus : Int8
{
	/** The task has not been ticked yet */
	bsUntouched,
	/** The task completed with success */
	bsSuccess,
	/** The task completed with failure */
	bsFailure,
	/** The task is running and will need more ticks to complete */
	bsRunning,
	/** The task was cancelled by its parent */
	bsCancelled
};

const char * to_string(eStatus a_Status);





class cTask
{
public:
	virtual ~cTask() = default;

	eStatus Tick(cBlackboard & a_Blackboard);

	void Cancel(cBlackboard & a_Blackboard)
	{
		if (GetStatus() == eStatus::bsRunning)
		{
			DoCancel(a_Blackboard);
			m_Status = eStatus::bsCancelled;
		}
	}

	eStatus GetStatus() const
	{
		return m_Status;
	}

private:
	eStatus m_Status = eStatus::bsUntouched;

	/** Called when this task starts running */
	virtual void DoEntry(cBlackboard & a_Blackboard) {}

	/** Called on each tick where control reaches this task */
	virtual eStatus DoTick(cBlackboard & a_Blackboard) = 0;

	/** Called when the task finishes via success or failure */
	virtual void DoExit(cBlackboard & a_Blackboard) {}

	/** Called when the task is cancelled by its parent */
	virtual void DoCancel(cBlackboard & a_Blackboard) {}
};

using cTasks = std::vector<std::unique_ptr<cTask>>;





std::unique_ptr<cTask> CreateBehaviourTree(eMonsterType a_MobType, const AString & a_CustomFileName = "");
cTasks CreateChildBehaviours(cLuaState & a_LuaState);
std::unique_ptr<cTask> CreateBehaviour(const AString & a_BehaviourName, cLuaState & a_LuaState);


/** Creates a vector from a variadic set of arguments
This helps avoid the issue that you can't move from `std::initializer_list`
so vector's list constructor won't work with e.g. `std::unique_ptr<cBehavior>`. */
template <typename ValueType, typename... Args>
std::vector<ValueType> MakeVector(Args && ... a_Behaviors)
{
	std::vector<ValueType> Result;
	Result.reserve(sizeof...(a_Behaviors));
	// Insert each item in order
	using cExpander = int[];
	(void)cExpander{0, (Result.push_back(std::forward<Args>(a_Behaviors)), 0) ...};
	return Result;
}

}  // namespace BehaviorTree


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
