#include "Globals.h"
#include "Palette_1_18.h"
#include "Registries/BlockStates.h"
#include "BlockType.h"
#include "Root.h"
#include "Protocol/Protocol.h"

namespace Palette_1_18
{
	UInt32 From(BlockState Block)
	{
		return cRoot::Get()->GetBlocMap()->GetProtocolBlockId(cProtocol::Version::v1_19, Block);
	}
}
