
// Scoreboard.h

// Implementation of a scoreboard that keeps track of specified objectives





#pragma once





class cObjective;
class cTeam;
class cWorld;

typedef cItemCallback<cObjective> cObjectiveCallback;
typedef cItemCallback<cTeam>      cTeamCallback;





// tolua_begin
class cObjective
{
public:

	typedef int Score;

	enum eDisplayType
	{
		dispInteger = 0,
		dispHearts
	};

	/** A criterion is represented by a single int, split into two field:
	the sub criteria field (in the higher bits) and the class field (in the
	low bits). The class is in the low bits so that an eCriteriaClass can be
	used as-is to represent a Criteria. */
	typedef int Criteria;

	static const int SubCriteriaOffset = 16;
	static const int CriteriaClassMask = 0xffff;

	enum eCriteriaClass
	{
		crDummy,

		crDeathCount,
		crPlayerKillCount,
		crTotalKillCount,
		crHealth,

		crAchievement,

		crStat,
		crStatItemCraft,
		crStatItemUse,
		crStatItemBreak,

		crStatBlockMine,
		crStatEntityKill,
		crStatEntityKilledBy,

		crClassCount
	};

	/** Create a criteria given a class and sub-criteria ID */
	static Criteria CriteriaFromClassAndSub(eCriteriaClass a_Class, short a_SubCriteria)
	{
		return (a_SubCriteria << SubCriteriaOffset) | static_cast<int>(a_Class);
	}

	/** Create a criteria from a class, with a default sub ID of zero. Used
	for classes that don't use the sub-ID, such as dummy or health. */
	static Criteria CriteriaFromClass(eCriteriaClass a_Class)
	{
		return CriteriaFromClassAndSub(a_Class, 0);
	}

	/** Given a criteria, return the enumerated class. */
	static eCriteriaClass GetCriteriaClass(Criteria a_Criteria)
	{
		int Class = a_Criteria & CriteriaClassMask;
		ASSERT(Class < otClassCount);
		return static_cast<eCriteriaClass>(Class);
	}

	/** Given a criteria, return the sub class. */
	static short GetCriteriaSub(Criteria a_Criteria)
	{
		return (a_Criteria & ~CriteriaClassMask) >> SubCriteriaOffset;
	}

	static Criteria StringToCriteria(const AString & a_Name);
	static AString CriteriaToString(Criteria a_Type);

	// tolua_end

	// For the "Scoreboard Objective" packet
	enum eUpdateAction
	{
		uaCreate = 0,
		uaRemove = 1,
		uaUpdateText = 2
	};

public:

	cObjective(const AString & a_Name, const AString & a_DisplayName, Criteria a_Type, cWorld * a_World);

	// tolua_begin

	Criteria GetType(void) const { return m_Type; }

	const AString & GetName(void)        const { return m_Name; }
	const AString & GetDisplayName(void) const { return m_DisplayName; }

	/** Resets the objective */
	void Reset(void);

	/** Returns the score of the specified key */
	Score GetScore(const AString & a_Key) const;

	/** Sets the score of all tracked keys */
	void SetAllScores(Score a_Score);

	/** Sets the score of the specified key */
	void SetScore(const AString & a_Key, Score a_Score);

	/** Resets the score of the specified key */
	void ResetScore(const AString & a_Key);

	/** Adds a_Delta and returns the new score */
	Score AddScore(const AString & a_Key, Score a_Delta);

	/** Subtracts a_Delta and returns the new score */
	Score SubScore(const AString & a_Key, Score a_Delta);

	void SetDisplayName(const AString & a_Name);

	/** Whether, in the list slot, to display as integers or hearts */
	void SetDisplayType(eDisplayType a_DisplayType);
	eDisplayType GetDisplayType(void) const { return m_DisplayType; }

	// tolua_end

	/** If true, increments the number of slots we're displayed in.
	if false, decrements the number. In either case, handles broadcasting
	the objective to the world if the objective is either newly displayed
	or newly not displayed. */
	void SetIsDisplayed(bool a_IsDisplayed);

	bool IsDisplayed(void) const { return m_DisplayCount > 0; }

	/** Returns a set of all players */
	AStringVector GetKeys(void) const;  // Exported in ManualBindings.cpp

	/** Send this objective to the specified client */
	void SendTo(cClientHandle & a_Client) const;

	static const char * GetClassStatic(void)  // Needed for ManualBindings's ForEach templates
	{
		return "cObjective";
	}


private:

	typedef std::map<AString, Score> cScoreMap;

	cScoreMap m_Scores;

	AString m_DisplayName;
	AString m_Name;

	/** (for list slots) whether to display as an integer or as hearts */
	eDisplayType m_DisplayType;

	Criteria m_Type;

	cWorld * m_World;

	/** A count of how many slots we're being displayed in */
	int m_DisplayCount;

	friend class cScoreboardSerializer;


};  // tolua_export





// tolua_begin
class cTeam
{
public:

	enum eColor
	{
		teamInvalid = -1,
		teamBlack = 0,
		teamDarkBlue,
		teamDarkGreen,
		teamDarkAqua,
		teamDarkRed,
		teamDarkPurple,
		teamGold,
		teamGray,
		teamDarkGray,
		teamBlue,
		teamGreen,
		teamAqua,
		teamRed,
		teamLightPurple,
		teamYellow,
		teamWhite
	};

	// tolua_end

	enum eProtocolAction
	{
		paCreateTeam = 0,
		paRemoveTeam = 1,
		paUpdateTeam = 2
	};

	cTeam(
		const AString & a_Name, const AString & a_DisplayName,
		const AString & a_Prefix, const AString & a_Suffix,
		cWorld * a_World
	);

	// tolua_begin

	/** Adds a new player to the team */
	bool AddPlayer(const AString & a_Name);

	/** Removes a player from the team */
	bool RemovePlayer(const AString & a_Name);

	/** Returns whether the specified player is in this team */
	bool HasPlayer(const AString & a_Name) const;

	/** Removes all registered players */
	void Reset(void);

	// tolua_begin

	/** Returns the number of registered players */
	size_t GetNumPlayers(void) const;

	bool AllowsFriendlyFire(void)      const { return m_AllowsFriendlyFire; }
	bool CanSeeFriendlyInvisible(void) const { return m_CanSeeFriendlyInvisible; }

	const AString & GetDisplayName(void) const { return m_DisplayName; }
	const AString & GetName(void)        const { return m_Name; }

	const AString & GetPrefix(void) const { return m_Prefix; }
	const AString & GetSuffix(void) const { return m_Suffix; }

	void SetFriendlyFire(bool a_Flag);
	void SetCanSeeFriendlyInvisible(bool a_Flag);

	void SetDisplayName(const AString & a_Name);

	void SetPrefix(const AString & a_Prefix);
	void SetSuffix(const AString & a_Suffix);

	void SetColor(eColor a_Color);
	eColor GetColor(void) const { return m_Color; }

	// tolua_end

	std::set<AString> GetMembers(void) const { return m_Players; }

	static const char * GetClassStatic(void)  // Needed for ManualBindings's ForEach templates
	{
		return "cTeam";
	}

private:

	typedef std::set<AString> cPlayerNameSet;

	cWorld * m_World;

	bool m_AllowsFriendlyFire;
	bool m_CanSeeFriendlyInvisible;

	AString m_DisplayName;
	AString m_Name;

	AString m_Prefix;
	AString m_Suffix;

	cPlayerNameSet m_Players;

	eColor m_Color;

	friend class cScoreboardSerializer;


};  // tolua_export





// tolua_begin
class cScoreboard
{
public:

	enum eDisplaySlot
	{
		dsList = 0,
		dsSidebar,
		dsName,
		dsSidebarTeamOffset,  // Add this to the cTeam::eColor to get the enum for that team

		// Taking into account the team colors
		dsCount = 19
	};

	// tolua_end

	// For the "Update Score" packet
	enum eUpdateAction
	{
		uaUpsert = 0,
		uaRemove = 1
	};


public:

	cScoreboard(cWorld * a_World);

	// tolua_begin

	/** Registers a new scoreboard objective, returns the cObjective instance, nullptr on name collision */
	cObjective * RegisterObjective(const AString & a_Name, const AString & a_DisplayName, cObjective::Criteria a_Type);

	/** Removes a registered objective, returns true if operation was successful */
	bool RemoveObjective(const AString & a_Name);

	/** Retrieves the objective with the specified name, nullptr if not found */
	cObjective * GetObjective(const AString & a_Name);

	/** Registers a new team, returns the cTeam instance, nullptr on name collision */
	cTeam * RegisterTeam(const AString & a_Name, const AString & a_DisplayName, const AString & a_Prefix, const AString & a_Suffix);

	/** Removes a registered team, returns true if operation was successful */
	bool RemoveTeam(const AString & a_Name);

	/** Retrieves the team with the specified name, nullptr if not found */
	cTeam * GetTeam(const AString & a_Name);

	/** Sets the given display slot to display the given objective. */
	void SetDisplay(const AString & a_Objective, eDisplaySlot a_Slot);

	/** Set the objective displayed in the given display slot, given the
	actual objective. */
	void SetDisplay(cObjective * a_Objective, eDisplaySlot a_Slot);

	/** Returns a pointer to the objective that is being displayed in the
	given display slot. If the given display slot is empty, nullptr is
	returned. */
	cObjective * GetObjectiveIn(eDisplaySlot a_Slot);

	/** Return number of registered objectives. */
	size_t GetNumObjectives(void) const;

	/** Return number of registered teams. */
	size_t GetNumTeams(void) const;

	/** Adds the given score to the given key in all objectives matching
	the given criteria. For example, one could add 1 to every playerKills
	objective for "bob". */
	void AddToScore(const AString & a_Key, cObjective::Criteria a_Type, cObjective::Score a_Value = 1);

	// tolua_end

	/** Retrieves the list of team names */
	AStringVector GetTeamNames();  // Exported in ManualBindings.cpp

	/** Send this scoreboard to the specified client */
	void SendTo(cClientHandle & a_Client);

	/** Tell the specified client to forget all about us */
	void RemoveFrom(cClientHandle & a_Client);

	/** Return a pointer to the team the player is currently on. nullptr if
	the given username is not a member of a team. */
	cTeam * QueryPlayerTeam(const AString & a_Name);  // WARNING: O(n logn)

	/** Returns whether the two named players are both on the same team.
	Returns false if both players are on no team. */
	bool AreOnSameTeam(const AString & a_Player1, const AString & a_Player2);

	/** Execute callback for each objective with the specified type
	Returns true if all objectives processed, false if the callback aborted by returning true. */
	bool ForEachObjectiveWith(cObjective::Criteria a_Type, cObjectiveCallback & a_Callback);

	/** Execute callback for each objective.
	Returns true if all objectives have been processed, false if the callback aborted by returning true. */
	bool ForEachObjective(cObjectiveCallback & a_Callback);  // Exported in ManualBindings.cpp

	/** Execute callback for each team.
	Returns true if all teams have been processed, false if the callback aborted by returning true. */
	bool ForEachTeam(cTeamCallback & a_Callback);  // Exported in ManualBindings.cpp

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

	cObjective * m_Display[dsCount];

	friend class cScoreboardSerializer;


};  // tolua_export




