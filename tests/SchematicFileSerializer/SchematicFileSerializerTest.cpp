
// SchematicFileSerializerTest.cpp

// Implements the SchematicFileSerializer test main entrypoint

#include "Globals.h"
#include "WorldStorage/SchematicFileSerializer.h"





static void DoTest(void)
{
	cBlockArea ba;
	ba.Create(21, 256, 21);
	ba.RelLine(0, 0, 0, 9, 8, 7, cBlockArea::baTypes | cBlockArea::baMetas, E_BLOCK_WOODEN_STAIRS, 1);
	AString Schematic;
	if (!cSchematicFileSerializer::SaveToSchematicString(ba, Schematic))
	{
		assert_test(!"Schematic failed to save!");
	}
	cBlockArea ba2;
	if (!cSchematicFileSerializer::LoadFromSchematicString(ba2, Schematic))
	{
		assert_test(!"Schematic failed to load!");
	}
}





int main(int argc, char * argv[])
{
	DoTest();
	LOG("SchematicFileSerializer test done.");
	return 0;
}




