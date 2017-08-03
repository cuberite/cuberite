
// ScoreboardAttachee.h

// Provides the interface that scoreboards call to update whomever the board is
// attached to (e.g. the World, or a ClientHandle)

#pragma once

class cScoreboardAttachee
{
public:
	enum eScoreActions : Byte
	{
		SCORE_CREATE_OR_UPDATE = 0,
		SCORE_REMOVE = 1,
	};

	enum eObjectiveActions : Byte
	{
		OBJECTIVE_CREATE = 0,
		OBJECTIVE_REMOVE = 1,
		OBJECTIVE_UPDATE_DISPLAY = 2,
	};

	/** Called to send a change in score for the given objective.
	If a_Mode = 0, then this will create or update the score.
	If a_Mode = 1, then this will remove the score entry. */
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, eScoreActions a_Mode) = 0;

	/** Called to modify an objective on the scoreboard.
	If a_Mode = 0, then this function is to create the objective.
	If a_Mode = 1, then this function is to remove the objective.
	If a_Mode = 2, then this function is to update the display text. */
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, eObjectiveActions a_Mode) = 0;

	/** Called to set where the given objective is displayed. */
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) = 0;
};
