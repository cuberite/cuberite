#include "Globals.h"

#include "BehaviorTree.h"
#include "Blackboard.h"

#include "../Monster.h"

#include "../../World.h"


// This logging is far too noisy to always be on
#if 1

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
// cRandomPosition:

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

eStatus cHealthRange::DoTick(cBlackboard & a_Blackboard)
{
	auto Health = a_Blackboard.Self().GetHealth();
	if ((Health > m_MinHealth) && (Health < m_MaxHealth))
	{
		return eStatus::bsSuccess;
	}

	return eStatus::bsFailure;
}


}  // namespace Behaviors
