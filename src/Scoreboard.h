
// Scoreboard.h

// Implementation of a scoreboard that keeps track of specified objectives





#pragma once





class cObjective;
class cWorld;

typedef cItemCallback<cObjective> cObjectiveCallback;





// tolua_begin
class cObjective
{
public:

	// tolua_end

	typedef int Score;

	enum eType
	{
		E_TYPE_DUMMY,

		E_TYPE_DEATH_COUNT,
		E_TYPE_PLAYER_KILL_COUNT,
		E_TYPE_TOTAL_KILL_COUNT,
		E_TYPE_HEALTH,

		E_TYPE_ACHIEVEMENT,

		E_TYPE_STAT,
		E_TYPE_STAT_ITEM_CRAFT,
		E_TYPE_STAT_ITEM_USE,
		E_TYPE_STAT_ITEM_BREAK,

		E_TYPE_STAT_BLOCK_MINE,
		E_TYPE_STAT_ENTITY_KILL,
		E_TYPE_STAT_ENTITY_KILLED_BY
	};

	static AString TypeToString(eType a_Type);

	static eType StringToType(const AString & a_Name);

public:

	cObjective(const AString & a_Name, const AString & a_DisplayName, eType a_Type, cWorld * a_World);

	eType GetType(void) const { return m_Type; }

	const AString & GetName(void)        const { return m_Name; }
	const AString & GetDisplayName(void) const { return m_DisplayName; }

	/// Resets the objective
	void Reset(void);

	/// Returns the score of the specified player
	Score GetScore(const AString & a_Name) const;

	/// Sets the score of the specified player
	void SetScore(const AString & a_Name, Score a_Score);

	/// Resets the score of the specified player
	void ResetScore(const AString & a_Name);

	/// Adds a_Delta and returns the new score
	Score AddScore(const AString & a_Name, Score a_Delta);

	/// Subtracts a_Delta and returns the new score
	Score SubScore(const AString & a_Name, Score a_Delta);

	void SetDisplayName(const AString & a_Name);

	/// Send this objective to the specified client
	void SendTo(cClientHandle & a_Client);

private:

	typedef std::pair<AString, Score> cTrackedPlayer;

	typedef std::map<AString, Score> cScoreMap;

	cScoreMap m_Scores;

	AString m_DisplayName;
	AString m_Name;

	eType m_Type;

	cWorld * m_World;

	friend class cScoreboardSerializer;

};





// tolua_begin
class cTeam
{
public:

	// tolua_end

	cTeam(
		const AString & a_Name, const AString & a_DisplayName,
		const AString & a_Prefix, const AString & a_Suffix
	);

	/// Adds a new player to the team
	bool AddPlayer(const AString & a_Name);

	/// Removes a player from the team
	bool RemovePlayer(const AString & a_Name);

	/// Returns whether the specified player is in this team
	bool HasPlayer(const AString & a_Name) const;

	/// Removes all registered players
	void Reset(void);

	/// Returns the number of registered players
	unsigned int GetNumPlayers(void) const;

	bool AllowsFriendlyFire(void)      const { return m_AllowsFriendlyFire; }
	bool CanSeeFriendlyInvisible(void) const { return m_CanSeeFriendlyInvisible; }

	const AString & GetDisplayName(void) const { return m_DisplayName; }
	const AString & GetName(void)        const { return m_DisplayName; }

	const AString & GetPrefix(void) const { return m_Prefix; }
	const AString & GetSuffix(void) const { return m_Suffix; }

	void SetFriendlyFire(bool a_Flag)            { m_AllowsFriendlyFire = a_Flag; }
	void SetCanSeeFriendlyInvisible(bool a_Flag) { m_CanSeeFriendlyInvisible = a_Flag; }

	void SetDisplayName(const AString & a_Name);

	void SetPrefix(const AString & a_Prefix);
	void SetSuffix(const AString & a_Suffix);

private:

	typedef std::set<AString> cPlayerNameSet;

	bool m_AllowsFriendlyFire;
	bool m_CanSeeFriendlyInvisible;

	AString m_DisplayName;
	AString m_Name;

	AString m_Prefix;
	AString m_Suffix;

	cPlayerNameSet m_Players;

	friend class cScoreboardSerializer;

};





// tolua_begin
class cScoreboard
{
public:

	// tolua_end

	enum eDisplaySlot
	{
		E_DISPLAY_SLOT_LIST = 0,
		E_DISPLAY_SLOT_SIDEBAR,
		E_DISPLAY_SLOT_NAME,

		E_DISPLAY_SLOT_COUNT
	};


public:

	cScoreboard(cWorld * a_World);

	/// Registers a new scoreboard objective, returns the cObjective instance, NULL on name collision
	cObjective * RegisterObjective(const AString & a_Name, const AString & a_DisplayName, cObjective::eType a_Type);

	/// Removes a registered objective, returns true if operation was successful
	bool RemoveObjective(const AString & a_Name);

	/// Retrieves the objective with the specified name, NULL if not found
	cObjective * GetObjective(const AString & a_Name);

	/// Registers a new team, returns the cTeam instance, NULL on name collision
	cTeam * RegisterTeam(const AString & a_Name, const AString & a_DisplayName, const AString & a_Prefix, const AString & a_Suffix);

	/// Removes a registered team, returns true if operation was successful
	bool RemoveTeam(const AString & a_Name);

	/// Retrieves the team with the specified name, NULL if not found
	cTeam * GetTeam(const AString & a_Name);

	cTeam * QueryPlayerTeam(const AString & a_Name); // WARNING: O(n logn)

	void SetDisplay(const AString & a_Objective, eDisplaySlot a_Slot);

	void SetDisplay(cObjective * a_Objective, eDisplaySlot a_Slot);

	cObjective * GetObjectiveIn(eDisplaySlot a_Slot);

	/// Execute callback for each objective with the specified type
	void ForEachObjectiveWith(cObjective::eType a_Type, cObjectiveCallback& a_Callback);

	/// Send this scoreboard to the specified client
	void SendTo(cClientHandle & a_Client);

	unsigned int GetNumObjectives(void) const;

	unsigned int GetNumTeams(void) const;


private:

	typedef std::pair<AString, cObjective> cNamedObjective;
	typedef std::pair<AString, cTeam>      cNamedTeam;

	typedef std::map<AString, cObjective> cObjectiveMap;
	typedef std::map<AString, cTeam>      cTeamMap;

	// TODO 2014-01-19 xdot: Potential optimization - Sort objectives based on type
	cCriticalSection m_CSObjectives;
	cObjectiveMap m_Objectives;

	cCriticalSection m_CSTeams;
	cTeamMap m_Teams;

	cWorld * m_World;

	cObjective* m_Display[E_DISPLAY_SLOT_COUNT];

	friend class cScoreboardSerializer;

} ;




