
// ForEachChunkProvider.h

// Declares the cForEachChunkProvider class which acts as an interface for classes that provide a ForEachChunkInRect() function
// Primarily serves as a decoupling between cBlockArea and cWorld





#pragma once




// fwd:
class cChunkDataCallback;
class cBlockArea;





class cForEachChunkProvider
{
public:
	virtual ~cForEachChunkProvider() {}

	/** Calls the callback for each chunk in the specified range. */
	virtual bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback) = 0;

	/** Writes the block area into the specified coords.
	Returns true if all chunks have been processed.
	a_DataTypes is a bitmask of cBlockArea::baXXX constants ORed together.
	*/
	virtual bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes) = 0;
};




