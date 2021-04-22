
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "../Item.h"





/** Wrapper for blocks that have a cBlockEntity descendant attached to them and can be "used" by the player.
Forwards the "use" event to the block entity. */
class cBlockEntityHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

protected:

	~cBlockEntityHandler() = default;

private:

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		return a_ChunkInterface.UseBlockEntity(&a_Player, a_BlockPos.x, a_BlockPos.y, a_BlockPos.z);
	}

	virtual bool IsUseable() const override
	{
		return true;
	}
};





class cDefaultBlockEntityHandler final :
	public cBlockEntityHandler
{
public:

	using cBlockEntityHandler::cBlockEntityHandler;
};
