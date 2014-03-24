/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

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

	cScoreboardSerializer(const AString & a_WorldName, cScoreboard* a_ScoreBoard);

	/// Try to load the scoreboard
	bool Load(void);

	/// Try to save the scoreboard
	bool Save(void);


private:

	void SaveScoreboardToNBT(cFastNBTWriter & a_Writer);

	bool LoadScoreboardFromNBT(const cParsedNBT & a_NBT);

	cScoreboard* m_ScoreBoard;

	AString m_Path;


} ;




