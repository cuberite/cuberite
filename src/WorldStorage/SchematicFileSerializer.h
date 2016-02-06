
// SchematicFileSerializer.h

// Declares the cSchematicFileSerializer class representing the interface to load and save cBlockArea to a .schematic file





#pragma once

#include "../BlockArea.h"





// fwd: FastNBT.h
class cParsedNBT;





class cSchematicFileSerializer
{
public:

	/** Loads an area from a .schematic file. Returns true if successful. */
	static bool LoadFromSchematicFile(cBlockArea & a_BlockArea, const AString & a_FileName);

	/** Loads an area from a string containing the .schematic file data. Returns true if successful. */
	static bool LoadFromSchematicString(cBlockArea & a_BlockArea, const AString & a_SchematicData);

	/** Saves the area into a .schematic file. Returns true if successful. */
	static bool SaveToSchematicFile(const cBlockArea & a_BlockArea, const AString & a_FileName);

	/** Saves the area into a string containing the .schematic file data.
	Returns true if successful, false on failure. The data is stored into a_Out. */
	static bool SaveToSchematicString(const cBlockArea & a_BlockArea, AString & a_Out);

private:
	/** Loads the area from a schematic file uncompressed and parsed into a NBT tree.
	Returns true if successful. */
	static bool LoadFromSchematicNBT(cBlockArea & a_BlockArea, cParsedNBT & a_NBT);

	/** Saves the area into a NBT representation and returns the NBT data as a string.
	Returns an empty string if failed. */
	static AString SaveToSchematicNBT(const cBlockArea & a_BlockArea);
};




