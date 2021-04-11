
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "../Entities/Player.h"
#include "../UI/AnvilWindow.h"





class cBlockAnvilHandler final :
	public cYawRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x01, 0x02>
{
	using Super = cYawRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x01, 0x02>;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(m_BlockType, 1, a_BlockMeta >> 2);
	}





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		cWindow * Window = new cAnvilWindow(a_BlockPos);
		a_Player.OpenWindow(*Window);
		return true;
	}





	virtual bool IsUseable() const override
	{
		return true;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 6;
	}
} ;




