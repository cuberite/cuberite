
// ScoreboardAttachee.h

// Provides the interface that scoreboards call to update whomever the board is
// attached to (e.g. the World, or a ClientHandle)

#pragma once

class cScoreboardAttachee
{
public:
	virtual ~cScoreboardAttachee() {}

	enum eScoreAction : Byte
	{
		saCreateOrUpdate = 0,
		saRemove = 1,
	};

	enum eObjectiveAction : Byte
	{
		objCreate = 0,
		objRemove = 1,
		objUpdateDisplay = 2,
	};

	/** Called to send a change in score for the given objective. */
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, eScoreAction a_Mode) = 0;

	/** Called to modify an objective on the scoreboard. */
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, eObjectiveAction a_Mode) = 0;

	/** Called to set where the given objective is displayed. */
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) = 0;
};
