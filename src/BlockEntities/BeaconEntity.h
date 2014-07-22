
#pragma once

#include "BlockEntity.h"





namespace Json
{
	class Value;
}





class cBeaconEntity :
	public cBlockEntity
{
	typedef cBlockEntity super;

public:
	
	/** The initial constructor */
	cBeaconEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	
	/** Returns the amount of layers the pyramid below the beacon has. */
	int GetPyramidLevel(void);

	/** Returns true if the block is a diamond block, a golden block, an iron block or an emerald block. */
	static bool IsMineralBlock(BLOCKTYPE a_BlockType);
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value& a_Value)  override;
	virtual void SendTo(cClientHandle & a_Client)   override;
	virtual void UsedBy(cPlayer * a_Player)         override;
	virtual bool Tick(float a_Dt, cChunk & /* a_Chunk */) override;
} ;




