
// Scoreboard.cpp

// Implementation of a scoreboard that keeps track of specified objectives

#include "Globals.h"

#include "Scoreboard.h"





cObjective::cObjective(cObjective::eType a_Type) : m_Type(a_Type)
{}





void cObjective::SetDisplaySlot(cObjective::eDisplaySlot a_Display)
{
	m_Display = a_Display;
}





void cObjective::Reset(void)
{
	m_Scores.clear();
}





cObjective::Score cObjective::GetScore(const AString & a_Name) const
{
	ScoreMap::const_iterator it = m_Scores.find(a_Name);

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
}





void cObjective::ResetScore(const AString & a_Name)
{
	m_Scores.erase(a_Name);
}





cObjective::Score cObjective::AddScore(const AString & a_Name, cObjective::Score a_Delta)
{
	// TODO 2014-01-19 xdot: Potential optimization - Reuse iterator
	Score NewScore = m_Scores[a_Name] + a_Delta;

	m_Scores[a_Name] = NewScore;

	return NewScore;
}





cObjective::Score cObjective::SubScore(const AString & a_Name, cObjective::Score a_Delta)
{
	// TODO 2014-01-19 xdot: Potential optimization - Reuse iterator
	Score NewScore = m_Scores[a_Name] - a_Delta;

	m_Scores[a_Name] = NewScore;

	return NewScore;
}





cTeam::cTeam(const AString & a_Name, const AString & a_DisplayName,
	     const AString & a_Prefix, const AString & a_Suffix)
	: m_AllowsFriendlyFire(true)
	, m_CanSeeFriendlyInvisible(false)
	, m_Name(a_Name)
	, m_DisplayName(a_DisplayName)
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





void cTeam::Reset(void)
{
	m_Players.clear();
}




unsigned int cTeam::GetNumPlayers(void) const
{
	return m_Players.size();
}





cObjective* cScoreboard::RegisterObjective(const AString & a_Name, cObjective::eType a_Type)
{
	cObjective Objective(a_Type);

	std::pair<ObjectiveMap::iterator, bool> Status = m_Objectives.insert(NamedObjective(a_Name, Objective));

	return Status.second ? &Status.first->second : NULL;
}





bool cScoreboard::RemoveObjective(const AString & a_Name)
{
	ObjectiveMap::iterator it = m_Objectives.find(a_Name);

	if (it == m_Objectives.end())
	{
		return false;
	}

	m_Objectives.erase(it);

	return true;
}





cObjective* cScoreboard::GetObjective(const AString & a_Name)
{
	ObjectiveMap::iterator it = m_Objectives.find(a_Name);

	if (it == m_Objectives.end())
	{
		return NULL;
	}
	else
	{
		return &it->second;
	}
}





cTeam* cScoreboard::RegisterTeam(
	const AString & a_Name, const AString & a_DisplayName,
	const AString & a_Prefix, const AString & a_Suffix
)
{
	cTeam Team(a_Name, a_DisplayName, a_Prefix, a_Suffix);

	std::pair<TeamMap::iterator, bool> Status = m_Teams.insert(NamedTeam(a_Name, Team));

	return Status.second ? &Status.first->second : NULL;
}





bool cScoreboard::RemoveTeam(const AString & a_Name)
{
	TeamMap::iterator it = m_Teams.find(a_Name);

	if (it == m_Teams.end())
	{
		return false;
	}

	m_Teams.erase(it);

	return true;
}





cTeam* cScoreboard::GetTeam(const AString & a_Name)
{
	TeamMap::iterator it = m_Teams.find(a_Name);

	if (it == m_Teams.end())
	{
		return NULL;
	}
	else
	{
		return &it->second;
	}
}





void cScoreboard::ForEachObjectiveWith(cObjective::eType a_Type, cObjectiveCallback& a_Callback)
{
	for (ObjectiveMap::iterator it = m_Objectives.begin(); it != m_Objectives.end(); ++it)
	{
		if (it->second.GetType() == a_Type)
		{
			// Call callback
			if (a_Callback.Item(&it->second))
			{
				return;
			}
		}
	}
}




