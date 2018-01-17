
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

	cMapSerializer(const AString & a_WorldName, cMap * a_Map);

	/** Try to load the map */
	bool Load(void);

	/** Try to save the map */
	bool Save(void);


private:

	void SaveMapToNBT(cFastNBTWriter & a_Writer);

	bool LoadMapFromNBT(const cParsedNBT & a_NBT);

	cMap * m_Map;

	AString m_Path;


} ;




/** Utility class used to serialize item ID counts.
In order to perform bounds checking (while loading),
the last registered ID of each item is serialized to an NBT file.
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




