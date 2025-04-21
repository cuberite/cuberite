#include "Globals.h"
#include "Palette_1_19.h"
#include "Registries/BlockStates.h"
#include "BlockType.h"
#include "Root.h"

namespace Palette_1_19
{
	UInt32 From(BlockState Block)
	{
		return cRoot::Get()->GetBlockMap()->GetProtocolBlockId(cProtocol::Version::v1_19, Block);
	}
	UInt32 From(Item ID) { return 0; }
	UInt32 From(CustomStatistic ID) { return 0; }
	Item ToItem(UInt32 ID) { return Item::Air; }
}  // namespace Palette_1_19
