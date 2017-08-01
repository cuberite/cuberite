
// ScoreboardAttachee.h

// Provides the interface that scoreboards call to update whomever the board is
// attached to (e.g. the World, or a ClientHandle)

#pragma once

class cScoreboardAttachee
{
public:
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) = 0;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) = 0;
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) = 0;
};
