
#pragma once

#include "Blackboard.h"

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

inline void format_arg(fmt::BasicFormatter<char> & a_Formatter,  const char *& a_FormatStr, eStatus a_Status)
{
	a_Formatter.writer().write(to_string(a_Status));
}





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





template <eStatus ContinueStatus>
class cBasicComposite:
	public cTask
{
public:

	cBasicComposite(cTasks a_Children) :
		m_Children(std::move(a_Children)),
		m_CurrentTask(m_Children.begin())
	{
	}

private:
	cTasks m_Children;
	cTasks::iterator m_CurrentTask;

	virtual void DoEntry(cBlackboard & a_Blackboard) override
	{
		m_CurrentTask = m_Children.begin();
	}

	virtual eStatus DoTick(cBlackboard & a_Blackboard) override
	{
		auto Status = ContinueStatus;
		for (; (m_CurrentTask != m_Children.end()); ++m_CurrentTask)
		{
			Status = (*m_CurrentTask)->Tick(a_Blackboard);

			if (Status != ContinueStatus)
			{
				break;
			}
		}
		return Status;
	}

	virtual void DoCancel(cBlackboard & a_Blackboard) override
	{
		if ((*m_CurrentTask)->GetStatus() == eStatus::bsRunning)
		{
			(*m_CurrentTask)->Cancel(a_Blackboard);
		}
	}
};

/** Executes children while they succeed, returns success if all children succeed */
using cSequence = cBasicComposite<eStatus::bsSuccess>;

/** Executes children while they fail, returns success if any child succeeds */
using cSelector = cBasicComposite<eStatus::bsFailure>;





/** A task that should only tick when its guard task allows it */
class cGuardedTask :
	public cTask
{
public:
	cGuardedTask(std::unique_ptr<cTask> a_Guard, std::unique_ptr<cTask> a_Child) :
		m_Guard(std::move(a_Guard)),
		m_Child(std::move(a_Child))
	{
		ASSERT(m_Child != nullptr);
	}

private:
	std::unique_ptr<cTask> m_Guard, m_Child;

	bool EvaluateGuard(cBlackboard & a_Blackboard);
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;

	friend class cDynamicGuardSelector;
};





/** A selector that always runs the active task with the highest priority, cancelling running tasks as necessary */
class cDynamicGuardSelector:
	public cTask
{
	using cGuardedTasks = std::vector<std::unique_ptr<cGuardedTask>>;
public:
	cDynamicGuardSelector(cGuardedTasks a_Children) :
		m_Children(std::move(a_Children)),
		m_CurrentTask(m_Children.begin())
	{
	}

private:
	cGuardedTasks m_Children;
	cGuardedTasks::iterator m_CurrentTask;

	virtual void DoEntry(cBlackboard & a_Blackboard) override;
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
};





/** A task decorator that turns failure into success */
class cSucceeder:
	public cTask
{
public:
	cSucceeder(std::unique_ptr<cTask> a_Child):
		m_Child(std::move(a_Child))
	{
		ASSERT(m_Child != nullptr);
	}

private:
	std::unique_ptr<cTask> m_Child;

	virtual eStatus DoTick(cBlackboard & a_Blackboard) override
	{
		auto ChildStatus = m_Child->Tick(a_Blackboard);
		return (ChildStatus == eStatus::bsRunning) ?
			eStatus::bsRunning : eStatus::bsSuccess;
	}

};





/** Writes a random position around the monster onto the blackboard */
class cRandomPosition :
	public cTask
{
public:
	cRandomPosition(cBlackboardKey a_WriteKey, double a_MaxRange) :
		m_Key(a_WriteKey),
		m_MaxRange(a_MaxRange)
	{
	}

private:
	cBlackboardKey m_Key;
	double m_MaxRange;

	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
};





/** Move to a position in the world */
class cMoveToPosition:
	public cTask
{
public:
	cMoveToPosition(cBlackboardKey a_PositionKey) :
		m_Key(a_PositionKey)
	{
	}

private:
	cBlackboardKey m_Key;

	virtual void DoEntry(cBlackboard & a_Blackboard) override;
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
	virtual void DoExit(cBlackboard & a_Blackboard) override;
	virtual void DoCancel(cBlackboard & a_Blackboard) override;
};





/** Wait a random amount of time */
class cRandomWait:
	public cTask
{
public:
	cRandomWait(cTickTime a_WaitMin, cTickTime a_WaitMax):
		m_WaitMin(a_WaitMin),
		m_WaitMax(a_WaitMax)
	{
	}

private:
	cTickTime m_WaitMin, m_WaitMax;
	cTickTimeLong m_WaitEnd;

	virtual void DoEntry(cBlackboard & a_Blackboard) override;
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
};





/** Succeeds if the monster's health is within some range */
class cHealthRange:
	public cTask
{
public:
	cHealthRange(float a_MinHealth, float a_MaxHealth):
		m_MinHealth(a_MinHealth),
		m_MaxHealth(a_MaxHealth)
	{
	}

private:
	float m_MinHealth, m_MaxHealth;
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
};

}  // namespace Behaviors
