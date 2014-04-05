
#pragma once

/** This interface is used to decouple block handlers from the cPluginManager dependancy through cWorld.
The block handlers call this interface, which is then implemented by the specific classes that
the caller provides.
*/
class cBlockPluginInterface
{
public:
	virtual ~cBlockPluginInterface() {}
	
	virtual bool CallHookBlockToPickups(cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups) = 0;
};
