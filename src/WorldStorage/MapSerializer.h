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

// MapSerializer.h

// Declares the cMapSerializer class that is used for saving maps into NBT format used by Anvil





#pragma once





// fwd:
class cFastNBTWriter;
class cParsedNBT;
class cMap;




/** Utility class used to serialize maps. */
class cMapSerializer
{
public:

	cMapSerializer(const AString& a_WorldName, cMap * a_Map);

	/** Try to load the scoreboard */
	bool Load(void);

	/** Try to save the scoreboard */
	bool Save(void);


private:

	void SaveMapToNBT(cFastNBTWriter & a_Writer);

	bool LoadMapFromNBT(const cParsedNBT & a_NBT);

	cMap * m_Map;

	AString m_Path;


} ;




/** Utility class used to serialize item ID counts.
 *
 * In order to perform bounds checking (while loading),
 * the last registered ID of each item is serialized to an NBT file.
 */
class cIDCountSerializer
{
public:

	cIDCountSerializer(const AString & a_WorldName);

	/** Try to load the ID counts */
	bool Load(void);

	/** Try to save the ID counts */
	bool Save(void);

	inline unsigned int GetMapCount(void) const { return m_MapCount; }

	inline void SetMapCount(unsigned int a_MapCount) { m_MapCount = a_MapCount; }


private:

	AString m_Path;

	unsigned int m_MapCount;

};




