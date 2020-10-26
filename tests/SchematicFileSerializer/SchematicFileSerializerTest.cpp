
// SchematicFileSerializerTest.cpp

// Implements the SchematicFileSerializer test main entrypoint

#include "Globals.h"
#include "../TestHelpers.h"
#include "WorldStorage/SchematicFileSerializer.h"





static void DoTest(void)
{
	cBlockArea ba;
	ba.Create(21, 256, 21);
	ba.RelLine(0, 0, 0, 9, 8, 7, cBlockArea::baTypes | cBlockArea::baMetas, E_BLOCK_WOODEN_STAIRS, 1);
	AString Schematic;
	TEST_TRUE(cSchematicFileSerializer::SaveToSchematicString(ba, Schematic));
	cBlockArea ba2;
	TEST_TRUE(cSchematicFileSerializer::LoadFromSchematicString(ba2, Schematic));
}





IMPLEMENT_TEST_MAIN("SchematicFileSerializer",
	DoTest();
)
