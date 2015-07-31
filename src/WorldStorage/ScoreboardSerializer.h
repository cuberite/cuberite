
// ScoreboardSerializer.h

// Declares the cScoreboardSerializer class that is used for saving scoreboards into NBT format used by Anvil





#pragma once





// fwd:
class cFastNBTWriter;
class cParsedNBT;
class cScoreboard;




class cScoreboardSerializer
{
public:

	cScoreboardSerializer(const AString & a_WorldName, cScoreboard * a_ScoreBoard);

	/** Try to load the scoreboard */
	bool Load(void);

	/** Try to save the scoreboard */
	bool Save(void);


private:

	void SaveScoreboardToNBT(cFastNBTWriter & a_Writer);

	bool LoadScoreboardFromNBT(const cParsedNBT & a_NBT);

	cScoreboard * m_ScoreBoard;

	AString m_Path;


} ;




