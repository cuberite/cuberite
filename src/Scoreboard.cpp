
// Scoreboard.cpp

// Implementation of a scoreboard that keeps track of specified objectives

#include "Globals.h"

#include "Scoreboard.h"
#include "World.h"
#include "ClientHandle.h"





AString cObjective::TypeToString(Criteria a_Type)
{
	switch (a_Type.m_Criteria)
	{
		case otDummy:              return "dummy";
		case otDeathCount:         return "deathCount";
		case otPlayerKillCount:    return "playerKillCount";
		case otTotalKillCount:     return "totalKillCount";
		case otHealth:             return "health";
		case otAchievement:        return "achievement";  // DEPRECATED

		// These all have extra things after the dot
		case otStat:               return "stat";
		case otStatItemCraft:      return "stat.craftItem";
		case otStatItemUse:        return "stat.useItem";
		case otStatItemBreak:      return "stat.breakItem";
		case otStatBlockMine:      return "stat.mineBlock";

		// Have to convert the entity ID to a name
		case otStatEntityKill:
		{
			return "stat.killEntity." + cMonster::MobTypeToVanillaName(static_cast<eMonsterType>(a_Type.m_SubCriteria));
		}
		case otStatEntityKilledBy:
		{
			return "stat.entityKilledBy." + cMonster::MobTypeToVanillaName(static_cast<eMonsterType>(a_Type.m_SubCriteria));
		}

		// clang optimisises this line away then warns that it has done so.
		#if !defined(__clang__)
		default: return "";
		#endif
	}

}





cObjective::Criteria cObjective::StringToType(const AString & a_Name)
{
	// For some, we don't have to do any further processing
	static const std::map<AString, Criteria> SimpleCriteria =
	{
		{"dummy", otDummy},
		{"deathCount", otDeathCount},
		{"playerKillCount", otPlayerKillCount},
		{"totalKillCount", otTotalKillCount},
		{"health", otHealth}
	};
	auto it = SimpleCriteria.find(a_Name);
	if (it != SimpleCriteria.end())
	{
		return it->second;
	}


	// TODO: "achievement" is unimplemented (and, removed in MC 1.12, so maybe it shouldn't be implemented) - lkolbly 08/2017


	// Some require converting entity names
	if (a_Name.substr(0, 16) == "stat.killEntity.")
	{
		AString Entity = a_Name.substr(16);
		eMonsterType MonsterType = cMonster::StringToMobType(Entity);
		return Criteria(otStatEntityKill, static_cast<int>(MonsterType));
	}
	if (a_Name.substr(0, 20) == "stat.entityKilledBy.")
	{
		AString Entity = a_Name.substr(20);
		eMonsterType MonsterType = cMonster::StringToMobType(Entity);
		return Criteria(otStatEntityKilledBy, static_cast<int>(MonsterType));
	}

	// TODO: Handle other cases
	LOGWARNING("Could not parse cObjective criteria '%s', returning otDummy", a_Name.c_str());
	return otDummy;
}





cObjective::cObjective(const AString & a_Name, const AString & a_DisplayName, cObjective::Criteria a_Type, cWorld * a_World)
	: m_DisplayName(a_DisplayName)
	, m_Name(a_Name)
	, m_DisplayType(dispInteger)
	, m_Type(a_Type)
	, m_World(a_World)
	, m_DisplayCount(0)
{
}





void cObjective::SetIsDisplayed(bool a_IsDisplayed)
{
	if (a_IsDisplayed)
	{
		m_DisplayCount++;
	}
	else
	{
		ASSERT(m_DisplayCount > 0);
		m_DisplayCount--;
	}

	// If we just started being displayed, tell clients about us
	if (a_IsDisplayed && (m_DisplayCount == 1))
	{
		m_World->BroadcastScoreboardObjective(*this, uaCreate);

		for (const auto & it : m_Scores)
		{
			m_World->BroadcastScoreUpdate(m_Name, it.first, it.second, cScoreboard::uaUpsert);
		}
	}

	// If we aren't displayed anymore, tell clients to forget about us
	if (m_DisplayCount == 0)
	{
		m_World->BroadcastScoreboardObjective(*this, uaRemove);

		for (const auto & it : m_Scores)
		{
			m_World->BroadcastScoreUpdate(m_Name, it.first, it.second, cScoreboard::uaRemove);
		}
	}
}





void cObjective::Reset(void)
{
	if (IsDisplayed())
	{
		for (auto it : m_Scores)
		{
			m_World->BroadcastScoreUpdate(m_Name, it.first, 0, cScoreboard::uaRemove);
		}
	}

	m_Scores.clear();
}





AStringVector cObjective::GetKeys(void) const
{
	AStringVector Players;
	for (auto player : m_Scores)
	{
		Players.push_back(player.first);
	}
	return Players;
}





cObjective::Score cObjective::GetScore(const AString & a_Key) const
{
	auto it = m_Scores.find(a_Key);

	if (it == m_Scores.end())
	{
		return 0;
	}
	else
	{
		return it->second;
	}
}





void cObjective::SetAllScores(cObjective::Score a_Score)
{
	for (auto & it : m_Scores)
	{
		it.second = a_Score;

		if (IsDisplayed())
		{
			m_World->BroadcastScoreUpdate(m_Name, it.first, a_Score, cScoreboard::uaUpsert);
		}
	}
}





void cObjective::SetScore(const AString & a_Key, cObjective::Score a_Score)
{
	m_Scores[a_Key] = a_Score;

	if (IsDisplayed())
	{
		m_World->BroadcastScoreUpdate(m_Name, a_Key, a_Score, cScoreboard::uaUpsert);
	}
}





void cObjective::ResetScore(const AString & a_Key)
{
	m_Scores.erase(a_Key);

	if (IsDisplayed())
	{
		m_World->BroadcastScoreUpdate(m_Name, a_Key, 0, cScoreboard::uaRemove);
	}
}





cObjective::Score cObjective::AddScore(const AString & a_Key, cObjective::Score a_Delta)
{
	Score NewScore = m_Scores[a_Key] + a_Delta;

	SetScore(a_Key, NewScore);

	return NewScore;
}





cObjective::Score cObjective::SubScore(const AString & a_Key, cObjective::Score a_Delta)
{
	Score NewScore = m_Scores[a_Key] - a_Delta;

	SetScore(a_Key, NewScore);

	return NewScore;
}





void cObjective::SetDisplayName(const AString & a_Name)
{
	m_DisplayName = a_Name;

	if (IsDisplayed())
	{
		m_World->BroadcastScoreboardObjective(*this, uaUpdateText);
	}
}





void cObjective::SetDisplayType(eDisplayType a_DisplayType)
{
	m_DisplayType = a_DisplayType;

	if (IsDisplayed())
	{
		m_World->BroadcastScoreboardObjective(*this, uaUpdateText);
	}
}





void cObjective::SendTo(cClientHandle & a_Client) const
{
	if (!IsDisplayed())
	{
		// If this objective isn't displayed, we don't have to send it
		return;
	}

	a_Client.SendScoreboardObjective(*this, uaCreate);

	for (const auto & it : m_Scores)
	{
		a_Client.SendScoreUpdate(m_Name, it.first, it.second, cScoreboard::uaUpsert);
	}
}





cTeam::cTeam(
	const AString & a_Name, const AString & a_DisplayName,
	const AString & a_Prefix, const AString & a_Suffix,
	cWorld * a_World
)
	: m_World(a_World)
	, m_AllowsFriendlyFire(true)
	, m_CanSeeFriendlyInvisible(false)
	, m_DisplayName(a_DisplayName)
	, m_Name(a_Name)
	, m_Prefix(a_Prefix)
	, m_Suffix(a_Suffix)
	, m_Color(teamInvalid)
{
}



void cTeam::SetColor(eColor a_Color)
{
	m_Color = a_Color;
	m_World->BroadcastTeam(*this, paUpdateTeam);
}





void cTeam::SetPrefix(const AString & a_Prefix)
{
	m_Prefix = a_Prefix;
	m_World->BroadcastTeam(*this, paUpdateTeam);
}





void cTeam::SetSuffix(const AString & a_Suffix)
{
	m_Suffix = a_Suffix;
	m_World->BroadcastTeam(*this, paUpdateTeam);
}





void cTeam::SetFriendlyFire(bool a_Flag)
{
	m_AllowsFriendlyFire = a_Flag;
	m_World->BroadcastTeam(*this, paUpdateTeam);
}





void cTeam::SetCanSeeFriendlyInvisible(bool a_Flag)
{
	m_CanSeeFriendlyInvisible = a_Flag;
	m_World->BroadcastTeam(*this, paUpdateTeam);
}





bool cTeam::AddPlayer(const AString & a_Name)
{
	m_World->BroadcastTeamChangeMembership(m_Name, true, {a_Name});
	return m_Players.insert(a_Name).second;
}





bool cTeam::RemovePlayer(const AString & a_Name)
{
	m_World->BroadcastTeamChangeMembership(m_Name, false, {a_Name});
	return m_Players.erase(a_Name) > 0;
}





bool cTeam::HasPlayer(const AString & a_Name) const
{
	const auto & it = m_Players.find(a_Name);

	return it != m_Players.end();
}





void cTeam::Reset(void)
{
	m_World->BroadcastTeamChangeMembership(m_Name, false, m_Players);
	m_Players.clear();
}




void cTeam::SetDisplayName(const AString & a_Name)
{
	m_DisplayName = a_Name;
	m_World->BroadcastTeam(*this, paUpdateTeam);
}





size_t cTeam::GetNumPlayers(void) const
{
	return m_Players.size();
}





cScoreboard::cScoreboard(cWorld * a_World) : m_World(a_World)
{
	for (int i = 0; i < static_cast<int>(dsCount); ++i)
	{
		m_Display[i] = nullptr;
	}
}





cObjective * cScoreboard::RegisterObjective(const AString & a_Name, const AString & a_DisplayName, cObjective::Criteria a_Type)
{
	if (m_Objectives.count(a_Name) > 0)
	{
		return nullptr;
	}

	cObjective Objective(a_Name, a_DisplayName, a_Type, m_World);

	auto Status = m_Objectives.insert(cNamedObjective(a_Name, Objective));

	if (Status.second)
	{
		return &Status.first->second;
	}
	else
	{
		return nullptr;
	}
}





bool cScoreboard::RemoveObjective(const AString & a_Name)
{
	cCSLock Lock(m_CSObjectives);

	auto it = m_Objectives.find(a_Name);

	if (it == m_Objectives.end())
	{
		return false;
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(dsCount); ++i)
	{
		if (m_Display[i] == &it->second)
		{
			SetDisplay(nullptr, static_cast<eDisplaySlot>(i));
		}
	}

	m_Objectives.erase(it);

	return true;
}





cObjective * cScoreboard::GetObjective(const AString & a_Name)
{
	cCSLock Lock(m_CSObjectives);

	auto it = m_Objectives.find(a_Name);

	if (it == m_Objectives.end())
	{
		return nullptr;
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
	cTeam Team(a_Name, a_DisplayName, a_Prefix, a_Suffix, m_World);

	std::pair<cTeamMap::iterator, bool> Status = m_Teams.insert(cNamedTeam(a_Name, Team));
	m_World->BroadcastTeam(Team, cTeam::paCreateTeam);

	return Status.second ? &Status.first->second : nullptr;
}





bool cScoreboard::RemoveTeam(const AString & a_Name)
{
	cCSLock Lock(m_CSTeams);

	auto it = m_Teams.find(a_Name);

	if (it == m_Teams.end())
	{
		return false;
	}

	m_World->BroadcastTeam(it->second, cTeam::paRemoveTeam);

	m_Teams.erase(it);

	return true;
}





cTeam * cScoreboard::GetTeam(const AString & a_Name)
{
	cCSLock Lock(m_CSTeams);

	auto it = m_Teams.find(a_Name);

	if (it == m_Teams.end())
	{
		return nullptr;
	}
	else
	{
		return &it->second;
	}
}





AStringVector cScoreboard::GetTeamNames()
{
	AStringVector TeamNames;

	for (const auto & Team: m_Teams)
	{
		TeamNames.push_back(Team.first);
	}

	return TeamNames;
}





cTeam * cScoreboard::QueryPlayerTeam(const AString & a_Name)
{
	cCSLock Lock(m_CSTeams);

	for (auto & it : m_Teams)
	{
		if (it.second.HasPlayer(a_Name))
		{
			return &it.second;
		}
	}

	return nullptr;
}





bool cScoreboard::AreOnSameTeam(const AString & a_Player1, const AString & a_Player2)
{
	cTeam * Team1 = QueryPlayerTeam(a_Player1);
	if (Team1 == nullptr)
	{
		return false;
	}

	cTeam * Team2 = QueryPlayerTeam(a_Player2);
	return Team1 == Team2;
}





void cScoreboard::SetDisplay(const AString & a_Objective, eDisplaySlot a_Slot)
{
	ASSERT(a_Slot < dsCount);

	cObjective * Objective = GetObjective(a_Objective);

	SetDisplay(Objective, a_Slot);
}





void cScoreboard::SetDisplay(cObjective * a_Objective, eDisplaySlot a_Slot)
{
	if (m_Display[a_Slot] != nullptr)
	{
		m_Display[a_Slot]->SetIsDisplayed(false);
	}

	m_Display[a_Slot] = a_Objective;
	if (m_Display[a_Slot] != nullptr)
	{
		m_Display[a_Slot]->SetIsDisplayed(true);
	}

	ASSERT(m_World != nullptr);
	m_World->BroadcastDisplayObjective(a_Objective ? a_Objective->GetName() : "", a_Slot);
}





cObjective * cScoreboard::GetObjectiveIn(eDisplaySlot a_Slot)
{
	ASSERT(a_Slot < dsCount);

	return m_Display[a_Slot];
}





bool cScoreboard::ForEachObjectiveWith(cObjective::Criteria a_Type, cObjectiveCallback & a_Callback)
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





bool cScoreboard::ForEachObjective(cObjectiveCallback & a_Callback)
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





bool cScoreboard::ForEachTeam(cTeamCallback & a_Callback)
{
	cCSLock Lock(m_CSTeams);

	for (cTeamMap::iterator it = m_Teams.begin(); it != m_Teams.end(); ++it)
	{
		// Call callback
		if (a_Callback.Item(&it->second))
		{
			return false;
		}
	}
	return true;
}





void cScoreboard::AddToScore(const AString & a_Name, cObjective::Criteria a_Type, cObjective::Score a_Value)
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

	for (const auto & it : m_Objectives)
	{
		if (it.second.IsDisplayed())
		{
			it.second.SendTo(a_Client);
		}
	}

	for (const auto & team : m_Teams)
	{
		a_Client.SendTeam(team.second, cTeam::paUpdateTeam);
	}

	for (int i = 0; i < static_cast<int>(dsCount); ++i)
	{
		// Avoid race conditions
		cObjective * Objective = m_Display[i];

		if (Objective)
		{
			a_Client.SendDisplayObjective(Objective->GetName(), static_cast<eDisplaySlot>(i));
		}
	}
}





void cScoreboard::RemoveFrom(cClientHandle & a_Client)
{
	cCSLock Lock(m_CSObjectives);

	// Remove every objective
	for (const auto & objective : m_Objectives)
	{
		if (objective.second.IsDisplayed())
		{
			a_Client.SendScoreboardObjective(objective.second, cObjective::uaRemove);
		}
	}

	// Remove every team
	for (const auto & team : m_Teams)
	{
		a_Client.SendTeam(team.second, cTeam::paRemoveTeam);
	}
}





size_t cScoreboard::GetNumObjectives(void) const
{
	return m_Objectives.size();
}





size_t cScoreboard::GetNumTeams(void) const
{
	return m_Teams.size();
}





