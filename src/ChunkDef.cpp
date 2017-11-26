
#include "Globals.h"

#include "ChunkDef.h"
#include "BlockEntities/BlockEntity.h"





void sBlockEntityDeleter::operator () (cBlockEntity * a_BlockEntity) const NOEXCEPT
{
	delete a_BlockEntity;
}
