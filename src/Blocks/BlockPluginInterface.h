
// BlockPluginInterface.h

// Declares the cBlockPluginInterface class representing an interface that the blockhandlers and itemhandlers use for calling plugins





#pragma once





// fwd:
class cPlayer;





/** This interface is used to decouple block handlers from the cPluginManager dependency through cWorld.
The block handlers call this interface, which is then implemented by the specific classes that
the caller provides.
*/
class cBlockPluginInterface
{
public:
	virtual ~cBlockPluginInterface() {}

	virtual bool CallHookBlockSpread(int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source) = 0;
	virtual bool CallHookPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool CallHookPlayerBrokenBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
};




