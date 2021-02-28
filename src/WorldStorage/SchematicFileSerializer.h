
// SchematicFileSerializer.h

// Declares the cSchematicFileSerializer class representing the interface to load and save cBlockArea to a .schematic file





#pragma once

#include "../BlockArea.h"
#include "../StringCompression.h"





// fwd: FastNBT.h
class cParsedNBT;





class cSchematicFileSerializer
{
public:

	/** Loads an area from a .schematic file. */
	static void LoadFromSchematicFile(cBlockArea & a_BlockArea, const std::string & a_FileName);

	/** Loads an area from a string containing the .schematic file data. */
	static void LoadFromSchematicString(cBlockArea & a_BlockArea, ContiguousByteBufferView a_SchematicData);

	/** Saves the area into a .schematic file. */
	static void SaveToSchematicFile(const cBlockArea & a_BlockArea, const std::string & a_FileName);

	/** Saves the area into a string containing the .schematic file data. */
	static Compression::Result SaveToSchematicString(const cBlockArea & a_BlockArea);

private:

	/** Loads the area from a schematic file uncompressed and parsed into a NBT tree. */
	static void LoadFromSchematicNBT(cBlockArea & a_BlockArea, const cParsedNBT & a_NBT);

	/** Saves the area into a NBT representation and returns the NBT data as a string. */
	static ContiguousByteBuffer SaveToSchematicNBT(const cBlockArea & a_BlockArea);
};
