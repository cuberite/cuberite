
#pragma once

#include "BlockHandler.h"





class cBlockPistonHandler :
	public cBlockHandler
{
public:
	cBlockPistonHandler(BLOCKTYPE a_BlockType);
	
	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override;

	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override;
	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override;
	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override;
	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override;
	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override;
} ;





class cBlockPistonHeadHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;
	
public:
	cBlockPistonHeadHandler(void);
	
	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
		// Also with 1.7, the item forms of these tecnical blocks have been removed, so giving someone this will crash their client...
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override;
	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override;
	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override;
	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override;
	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override;
} ;




