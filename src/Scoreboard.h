
// Scoreboard.h

// Implementation of a scoreboard that keeps track of specified objectives





#pragma once





class cObjective;

typedef cItemCallback<cObjective> cObjectiveCallback;





class cObjective
{
public:
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

	enum eDisplaySlot
	{
		E_DISPLAY_SLOT_LIST,
		E_DISPLAY_SLOT_SIDEBAR,
		E_DISPLAY_SLOT_NAME
	};

public:
	cObjective(eType a_Type);

	eType GetType(void) const { return m_Type; }

	eDisplaySlot GetDisplaySlot(void) const { return m_Display; }

	void SetDisplaySlot(eDisplaySlot a_Display);

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

private:
	typedef std::pair<AString, Score> TrackedPlayer;

	typedef std::map<AString, Score> ScoreMap;

	ScoreMap m_Scores;

	eType m_Type;

	eDisplaySlot m_Display;
};





class cTeam
{
public:

	cTeam(
		const AString & a_Name, const AString & a_DisplayName,
		const AString & a_Prefix, const AString & a_Suffix
	);

	/// Adds a new player to the team
	bool AddPlayer(const AString & a_Name);

	/// Removes a player from the team
	bool RemovePlayer(const AString & a_Name);

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

	void SetFriendlyFire(bool a_Flag);
	void SetCanSeeFriendlyInvisible(bool a_Flag);

	void SetDisplayName(const AString & a_Name);

	void SetPrefix(const AString & a_Prefix);
	void SetSuffix(const AString & a_Suffix);

private:

	bool m_AllowsFriendlyFire;
	bool m_CanSeeFriendlyInvisible;

	AString m_DisplayName;
	AString m_Name;

	AString m_Prefix;
	AString m_Suffix;

	// TODO 2014-01-19 xdot: Potential optimization - vector/list
	typedef std::set<AString> PlayerNameSet;

	PlayerNameSet m_Players;
};





class cScoreboard
{
public:
	cScoreboard() {}

	/// Registers a new scoreboard objective, returns the cObjective instance, NULL on name collision
	cObjective* RegisterObjective(const AString & a_Name, cObjective::eType a_Type);

	/// Removes a registered objective, returns true if operation was successful
	bool RemoveObjective(const AString & a_Name);

	/// Retrieves the objective with the specified name, NULL if not found
	cObjective* GetObjective(const AString & a_Name);

	/// Registers a new team, returns the cTeam instance, NULL on name collision
	cTeam* RegisterTeam(const AString & a_Name, const AString & a_DisplayName,
	                    const AString & a_Prefix, const AString & a_Suffix);

	/// Removes a registered team, returns true if operation was successful
	bool RemoveTeam(const AString & a_Name);

	/// Retrieves the team with the specified name, NULL if not found
	cTeam* GetTeam(const AString & a_Name);

	/// Execute callback for each objective with the specified type
	void ForEachObjectiveWith(cObjective::eType a_Type, cObjectiveCallback& a_Callback);

private:
	typedef std::pair<AString, cObjective> NamedObjective;
	typedef std::pair<AString, cTeam>      NamedTeam;

	typedef std::map<AString, cObjective> ObjectiveMap;
	typedef std::map<AString, cTeam>      TeamMap;

	// TODO 2014-01-19 xdot: Potential optimization - Sort objectives based on type
	ObjectiveMap m_Objectives;

	TeamMap m_Teams;
} ;




