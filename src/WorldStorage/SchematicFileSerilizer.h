
#include "../BlockArea.h"

// fwd: FastNBT.h
class cParsedNBT;

class cSchematicFileSerializer
{
public:
		
	/// Loads an area from a .schematic file. Returns true if successful
	static bool LoadFromSchematicFile(const AString & a_FileName);
	
	/// Saves the area into a .schematic file. Returns true if successful
	static bool SaveToSchematicFile(const AString & a_FileName);
	
private:
	/// Loads the area from a schematic file uncompressed and parsed into a NBT tree. Returns true if successful.
	static bool LoadFromSchematicNBT(cBlockArea& a_BlockArea, cParsedNBT & a_NBT);
};
