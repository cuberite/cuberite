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

// MapManager.h





#pragma once





#include "Map.h"




typedef cItemCallback<cMap> cMapCallback;




// tolua_begin

/** Manages the in-game maps of a single world - Thread safe. */
class cMapManager
{
public:
	// tolua_end

	cMapManager(cWorld * a_World);

	/** Returns the map with the specified ID, NULL if out of range.
	 *
	 * WARNING: The returned map object is not thread safe.
	 */
	cMap * GetMapData(unsigned int a_ID);

	/** Creates a new map. Returns NULL on error */
	cMap * CreateMap(int a_CenterX, int a_CenterY, int a_Scale = 3);

	/** Calls the callback for the map with the specified ID.
	 *
	 * Returns true if the map was found and the callback called, false if map not found. 
	 * Callback return ignored.
	 */
	bool DoWithMap(int a_ID, cMapCallback & a_Callback); // Exported in ManualBindings.cpp

	/** Calls the callback for each map. 
	 *
	 * Returns true if all maps processed, false if the callback aborted by returning true.
	 */
	bool ForEachMap(cMapCallback & a_Callback);

	unsigned int GetNumMaps(void) const; // tolua_export

	/** Loads the map data from the disk */
	void LoadMapData(void);

	/** Saves the map data to the disk */
	void SaveMapData(void);


private:

	typedef std::vector<cMap> cMapList;

	cCriticalSection m_CS;

	cMapList m_MapData;

	cWorld * m_World;

}; // tolua_export



