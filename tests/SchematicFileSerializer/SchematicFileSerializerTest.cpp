
// SchematicFileSerializerTest.cpp

// Implements the SchematicFileSerializer test main entrypoint

#include "Globals.h"
#include "../TestHelpers.h"
#include "WorldStorage/SchematicFileSerializer.h"





static void DoTest(void)
{
	cBlockArea ba;
	ba.Create(21, 256, 21);
	ba.RelLine(0, 0, 0, 9, 8, 7, cBlockArea::baBlocks, Block::OakStairs::OakStairs());
	const auto Schematic = cSchematicFileSerializer::SaveToSchematicString(ba);
	cBlockArea ba2;
	cSchematicFileSerializer::LoadFromSchematicString(ba2, Schematic.GetView());
}





IMPLEMENT_TEST_MAIN("SchematicFileSerializer",
	DoTest();
)
