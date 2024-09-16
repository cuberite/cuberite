
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

	virtual bool CallHookBlockSpread(Vector3i a_BlockPos, eSpreadSource a_Source) = 0;
	virtual bool CallHookPlayerBreakingBlock(cPlayer & a_Player, Vector3i a_BlockPos, eBlockFace a_BlockFace, BlockState a_Block) = 0;
	virtual bool CallHookPlayerBrokenBlock(cPlayer & a_Player, Vector3i a_BlockPos, eBlockFace a_BlockFace, BlockState a_Block) = 0;
};




