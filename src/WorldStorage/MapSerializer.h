
// MapSerializer.h

// Declares the cMapSerializer class that is used for saving maps into NBT format used by Anvil





#pragma once





// fwd:
class cFastNBTWriter;
class cParsedNBT;
class cMap;




class cMapSerializer
{
public:

	cMapSerializer(const AString& a_WorldName, cMap * a_Map);

	/// Try to load the scoreboard
	bool Load(void);

	/// Try to save the scoreboard
	bool Save(void);


private:

	void SaveMapToNBT(cFastNBTWriter & a_Writer);

	bool LoadMapFromNBT(const cParsedNBT & a_NBT);

	cMap * m_Map;

	AString m_Path;


} ;




class cIDCountSerializer
{
public:

	cIDCountSerializer(const AString & a_WorldName);

	bool Load(void);

	bool Save(void);

	inline unsigned int GetMapCount(void) const { return m_MapCount; }

	inline void SetMapCount(unsigned int a_MapCount) { m_MapCount = a_MapCount; }


private:

	AString m_Path;

	unsigned int m_MapCount;
};




