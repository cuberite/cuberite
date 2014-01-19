
// Scoreboard.h

// Implementation of a scoreboard that keeps track of specified objectives





#pragma once





class cPlayer;
class cObjective;

typedef std::set< cPlayer * > cPlayerSet;
typedef cItemCallback<cObjective> cObjectiveCallback;





enum eObjectiveType
{
	E_OBJECTIVE_DUMMY,

	E_OBJECTIVE_DEATH_COUNT,
	E_OBJECTIVE_PLAYER_KILL_COUNT,
	E_OBJECTIVE_TOTAL_KILL_COUNT,
	E_OBJECTIVE_HEALTH,

	E_OBJECTIVE_ACHIEVEMENT,

	E_OBJECTIVE_STAT,
	E_OBJECTIVE_STAT_ITEM_CRAFT,
	E_OBJECTIVE_STAT_ITEM_USE,
	E_OBJECTIVE_STAT_ITEM_BREAK,

	E_OBJECTIVE_STAT_BLOCK_MINE,
	E_OBJECTIVE_STAT_ENTITY_KILL,
	E_OBJECTIVE_STAT_ENTITY_KILLED_BY
};





enum eDisplaySlot
{
	E_DISPLAY_SLOT_LIST,
	E_DISPLAY_SLOT_SIDEBAR,
	E_DISPLAY_SLOT_NAME
};





class cObjective
{
public:
	typedef int Score;

public:
	cObjective(eObjectiveType a_Type);

	eObjectiveType GetType(void) const { return m_Type; }

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

	eObjectiveType m_Type;

	eDisplaySlot m_Display;
};





class cTeam
{
public:
	cTeam(const AString & a_Name, const AString & a_DisplayName,
	      const AString & a_Prefix, const AString & a_Suffix);

	/// Adds a new player to the team
	bool AddPlayer(cPlayer * a_Player);

	/// Removes a player from the team
	bool RemovePlayer(cPlayer * a_Player);

	/// Removes all registered players
	void Reset(void);

	/// Returns the number of registered players
	unsigned int GetNumPlayers(void) const;

	bool GetFriendlyFire(void)            const { return m_FriendlyFire; }
	bool GetCanSeeFriendlyInvisible(void) const { return m_SeeFriendlyInvisible; }

	const AString & GetDisplayName(void) const { return m_Name; }
	const AString & GetName(void)        const { return m_DisplayName; }

	const AString & GetPrefix(void) const { return m_Prefix; }
	const AString & GetSuffix(void) const { return m_Suffix; }

	void SetFriendlyFire(bool a_Flag);
	void SetCanSeeFriendlyInvisible(bool a_Flag);

	void SetDisplayName(const AString & a_Name);

	void SetPrefix(const AString & a_Prefix);
	void SetSuffix(const AString & a_Suffix);

private:

	bool m_FriendlyFire;
	bool m_SeeFriendlyInvisible;

	AString m_DisplayName;
	AString m_Name;

	AString m_Prefix;
	AString m_Suffix;

	// TODO 2014-01-19 xdot: Potential optimization - vector/list
	cPlayerSet m_Players;
};





class cScoreboard
{
public:
	cScoreboard() {}
	virtual ~cScoreboard();

	/// Registers a new scoreboard objective, returns the cObjective instance
	cObjective* RegisterObjective(const AString & a_Name, eObjectiveType a_Type);

	/// Removes a registered objective, returns true if operation was successful
	bool RemoveObjective(const AString & a_Name);

	/// Retrieves the objective with the specified name, NULL if not found
	cObjective* GetObjective(const AString & a_Name);

	/// Registers a new team, returns the cTeam instance
	cTeam* RegisterTeam(const AString & a_Name, const AString & a_DisplayName,
	                    const AString & a_Prefix, const AString & a_Suffix);

	/// Removes a registered team, returns true if operation was successful
	bool RemoveTeam(const AString & a_Name);

	/// Retrieves the team with the specified name, NULL if not found
	cTeam* GetTeam(const AString & a_Name);

	/// Execute callback for each objective with the specified type
	void ForEachObjectiveWith(eObjectiveType a_Type, cObjectiveCallback& a_Callback);

private:
	typedef std::pair<AString, cObjective*> NamedObjective;
	typedef std::pair<AString, cTeam*>      NamedTeam;

	typedef std::map<AString, cObjective*> ObjectiveMap;
	typedef std::map<AString, cTeam*>      TeamMap;

	// TODO 2014-01-19 xdot: Potential optimization - Sort objectives based on type
	ObjectiveMap m_Objectives;

	TeamMap m_Teams;
} ;




