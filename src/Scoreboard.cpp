
// Scoreboard.cpp

// Implementation of a scoreboard that keeps track of specified objectives

#include "Globals.h"

#include "Scoreboard.h"
#include "World.h"
#include "ClientHandle.h"





AString cObjective::TypeToString(eType a_Type)
{
	switch (a_Type)
	{
		case E_TYPE_DUMMY:                 return "dummy";
		case E_TYPE_DEATH_COUNT:           return "deathCount";
		case E_TYPE_PLAYER_KILL_COUNT:     return "playerKillCount";
		case E_TYPE_TOTAL_KILL_COUNT:      return "totalKillCount";
		case E_TYPE_HEALTH:                return "health";
		case E_TYPE_ACHIEVEMENT:           return "achievement";
		case E_TYPE_STAT:                  return "stat";
		case E_TYPE_STAT_ITEM_CRAFT:       return "stat.craftItem";
		case E_TYPE_STAT_ITEM_USE:         return "stat.useItem";
		case E_TYPE_STAT_ITEM_BREAK:       return "stat.breakItem";
		case E_TYPE_STAT_BLOCK_MINE:       return "stat.mineBlock";
		case E_TYPE_STAT_ENTITY_KILL:      return "stat.killEntity";
		case E_TYPE_STAT_ENTITY_KILLED_BY: return "stat.entityKilledBy";

		default: return "";
	}
}





cObjective::eType cObjective::StringToType(const AString & a_Name)
{
	static struct {
		eType m_Type;
		const char * m_String;
	} TypeMap [] =
	{
		{E_TYPE_DUMMY,                 "dummy"},
		{E_TYPE_DEATH_COUNT,           "deathCount"},
		{E_TYPE_PLAYER_KILL_COUNT,     "playerKillCount"},
		{E_TYPE_TOTAL_KILL_COUNT,      "totalKillCount"},
		{E_TYPE_HEALTH,                "health"},
		{E_TYPE_ACHIEVEMENT,           "achievement"},
		{E_TYPE_STAT,                  "stat"},
		{E_TYPE_STAT_ITEM_CRAFT,       "stat.craftItem"},
		{E_TYPE_STAT_ITEM_USE,         "stat.useItem"},
		{E_TYPE_STAT_ITEM_BREAK,       "stat.breakItem"},
		{E_TYPE_STAT_BLOCK_MINE,       "stat.mineBlock"},
		{E_TYPE_STAT_ENTITY_KILL,      "stat.killEntity"},
		{E_TYPE_STAT_ENTITY_KILLED_BY, "stat.entityKilledBy"}
	};
	for (size_t i = 0; i < ARRAYCOUNT(TypeMap); i++)
	{
		if (NoCaseCompare(TypeMap[i].m_String, a_Name) == 0)
		{
			return TypeMap[i].m_Type;
		}
	}  // for i - TypeMap[]
	return E_TYPE_DUMMY;
}





cObjective::cObjective(const AString & a_Name, const AString & a_DisplayName, cObjective::eType a_Type, cWorld * a_World)
	: m_DisplayName(a_DisplayName)
	, m_Name(a_Name)
	, m_Type(a_Type)
	, m_World(a_World)
{
}





void cObjective::Reset(void)
{
	for (cScoreMap::iterator it = m_Scores.begin(); it != m_Scores.end(); ++it)
	{
		m_World->BroadcastScoreUpdate(m_Name, it->first, 0, 1);
	}

	m_Scores.clear();
}





cObjective::Score cObjective::GetScore(const AString & a_Name) const
{
	cScoreMap::const_iterator it = m_Scores.find(a_Name);

	if (it == m_Scores.end())
	{
		return 0;
	}
	else
	{
		return it->second;
	}
}





void cObjective::SetScore(const AString & a_Name, cObjective::Score a_Score)
{
	m_Scores[a_Name] = a_Score;

	m_World->BroadcastScoreUpdate(m_Name, a_Name, a_Score, 0);
}





void cObjective::ResetScore(const AString & a_Name)
{
	m_Scores.erase(a_Name);

	m_World->BroadcastScoreUpdate(m_Name, a_Name, 0, 1);
}





cObjective::Score cObjective::AddScore(const AString & a_Name, cObjective::Score a_Delta)
{
	// TODO 2014-01-19 xdot: Potential optimization - Reuse iterator
	Score NewScore = m_Scores[a_Name] + a_Delta;

	SetScore(a_Name, NewScore);

	return NewScore;
}





cObjective::Score cObjective::SubScore(const AString & a_Name, cObjective::Score a_Delta)
{
	// TODO 2014-01-19 xdot: Potential optimization - Reuse iterator
	Score NewScore = m_Scores[a_Name] - a_Delta;

	SetScore(a_Name, NewScore);

	return NewScore;
}





void cObjective::SetDisplayName(const AString & a_Name)
{
	m_DisplayName = a_Name;

	m_World->BroadcastScoreboardObjective(m_Name, m_DisplayName, 2);
}





void cObjective::SendTo(cClientHandle & a_Client)
{
	a_Client.SendScoreboardObjective(m_Name, m_DisplayName, 0);

	for (cScoreMap::const_iterator it = m_Scores.begin(); it != m_Scores.end(); ++it)
	{
		a_Client.SendScoreUpdate(m_Name, it->first, it->second, 0);
	}
}





cTeam::cTeam(const AString & a_Name, const AString & a_DisplayName,
	     const AString & a_Prefix, const AString & a_Suffix)
	: m_AllowsFriendlyFire(true)
	, m_CanSeeFriendlyInvisible(false)
	, m_DisplayName(a_DisplayName)
	, m_Name(a_Name)
	, m_Prefix(a_Prefix)
	, m_Suffix(a_Suffix)
{}





bool cTeam::AddPlayer(const AString & a_Name)
{
	return m_Players.insert(a_Name).second;
}





bool cTeam::RemovePlayer(const AString & a_Name)
{
	return m_Players.erase(a_Name) > 0;
}





bool cTeam::HasPlayer(const AString & a_Name) const
{
	cPlayerNameSet::const_iterator it = m_Players.find(a_Name);

	return it != m_Players.end();
}





void cTeam::Reset(void)
{
	// TODO 2014-01-22 xdot: Inform online players

	m_Players.clear();
}




void cTeam::SetDisplayName(const AString & a_Name)
{
	m_DisplayName = a_Name;

	// TODO 2014-03-01 xdot: Update clients
}





unsigned int cTeam::GetNumPlayers(void) const
{
	return m_Players.size();
}





cScoreboard::cScoreboard(cWorld * a_World) : m_World(a_World)
{
	for (int i = 0; i < (int) E_DISPLAY_SLOT_COUNT; ++i)
	{
		m_Display[i] = NULL;
	}
}





cObjective* cScoreboard::RegisterObjective(const AString & a_Name, const AString & a_DisplayName, cObjective::eType a_Type)
{
	cObjective Objective(a_Name, a_DisplayName, a_Type, m_World);

	std::pair<cObjectiveMap::iterator, bool> Status = m_Objectives.insert(cNamedObjective(a_Name, Objective));

	if (Status.second)
	{
		ASSERT(m_World != NULL);
		m_World->BroadcastScoreboardObjective(a_Name, a_DisplayName, 0);

		return &Status.first->second;
	}
	else
	{
		return NULL;
	}
}





bool cScoreboard::RemoveObjective(const AString & a_Name)
{
	cCSLock Lock(m_CSObjectives);

	cObjectiveMap::iterator it = m_Objectives.find(a_Name);

	if (it == m_Objectives.end())
	{
		return false;
	}

	m_Objectives.erase(it);

	ASSERT(m_World != NULL);
	m_World->BroadcastScoreboardObjective(it->second.GetName(), it->second.GetDisplayName(), 1);

	// TODO 2014-03-01 xdot: Remove objective from display slot

	return true;
}





cObjective * cScoreboard::GetObjective(const AString & a_Name)
{
	cCSLock Lock(m_CSObjectives);

	cObjectiveMap::iterator it = m_Objectives.find(a_Name);

	if (it == m_Objectives.end())
	{
		return NULL;
	}
	else
	{
		return &it->second;
	}
}





cTeam * cScoreboard::RegisterTeam(
	const AString & a_Name, const AString & a_DisplayName,
	const AString & a_Prefix, const AString & a_Suffix
)
{
	cTeam Team(a_Name, a_DisplayName, a_Prefix, a_Suffix);

	std::pair<cTeamMap::iterator, bool> Status = m_Teams.insert(cNamedTeam(a_Name, Team));

	return Status.second ? &Status.first->second : NULL;
}





bool cScoreboard::RemoveTeam(const AString & a_Name)
{
	cCSLock Lock(m_CSTeams);

	cTeamMap::iterator it = m_Teams.find(a_Name);

	if (it == m_Teams.end())
	{
		return false;
	}

	m_Teams.erase(it);

	return true;
}





cTeam * cScoreboard::GetTeam(const AString & a_Name)
{
	cCSLock Lock(m_CSTeams);

	cTeamMap::iterator it = m_Teams.find(a_Name);

	if (it == m_Teams.end())
	{
		return NULL;
	}
	else
	{
		return &it->second;
	}
}





cTeam * cScoreboard::QueryPlayerTeam(const AString & a_Name)
{
	cCSLock Lock(m_CSTeams);

	for (cTeamMap::iterator it = m_Teams.begin(); it != m_Teams.end(); ++it)
	{
		if (it->second.HasPlayer(a_Name))
		{
			return &it->second;
		}
	}

	return NULL;
}





void cScoreboard::SetDisplay(const AString & a_Objective, eDisplaySlot a_Slot)
{
	ASSERT(a_Slot < E_DISPLAY_SLOT_COUNT);

	cObjective * Objective = GetObjective(a_Objective);

	SetDisplay(Objective, a_Slot);
}





void cScoreboard::SetDisplay(cObjective * a_Objective, eDisplaySlot a_Slot)
{
	m_Display[a_Slot] = a_Objective;

	ASSERT(m_World != NULL);
	m_World->BroadcastDisplayObjective(a_Objective ? a_Objective->GetName() : "", a_Slot);
}





cObjective * cScoreboard::GetObjectiveIn(eDisplaySlot a_Slot)
{
	ASSERT(a_Slot < E_DISPLAY_SLOT_COUNT);

	return m_Display[a_Slot];
}





bool cScoreboard::ForEachObjectiveWith(cObjective::eType a_Type, cObjectiveCallback& a_Callback)
{
	cCSLock Lock(m_CSObjectives);

	for (cObjectiveMap::iterator it = m_Objectives.begin(); it != m_Objectives.end(); ++it)
	{
		if (it->second.GetType() == a_Type)
		{
			// Call callback
			if (a_Callback.Item(&it->second))
			{
				return false;
			}
		}
	}
	return true;
}





bool cScoreboard::ForEachObjective(cObjectiveCallback& a_Callback)
{
	cCSLock Lock(m_CSObjectives);

	for (cObjectiveMap::iterator it = m_Objectives.begin(); it != m_Objectives.end(); ++it)
	{
		// Call callback
		if (a_Callback.Item(&it->second))
		{
			return false;
		}
	}
	return true;
}





void cScoreboard::AddPlayerScore(const AString & a_Name, cObjective::eType a_Type, cObjective::Score a_Value)
{
	cCSLock Lock(m_CSObjectives);

	for (cObjectiveMap::iterator it = m_Objectives.begin(); it != m_Objectives.end(); ++it)
	{
		if (it->second.GetType() == a_Type)
		{
			it->second.AddScore(a_Name, a_Value);
		}
	}
}





void cScoreboard::SendTo(cClientHandle & a_Client)
{
	cCSLock Lock(m_CSObjectives);

	for (cObjectiveMap::iterator it = m_Objectives.begin(); it != m_Objectives.end(); ++it)
	{
		it->second.SendTo(a_Client);
	}

	for (int i = 0; i < (int) E_DISPLAY_SLOT_COUNT; ++i)
	{
		// Avoid race conditions
		cObjective * Objective = m_Display[i];

		if (Objective)
		{
			a_Client.SendDisplayObjective(Objective->GetName(), (eDisplaySlot) i);
		}
	}
}





unsigned int cScoreboard::GetNumObjectives(void) const
{
	return m_Objectives.size();
}





unsigned int cScoreboard::GetNumTeams(void) const
{
	return m_Teams.size();
}





