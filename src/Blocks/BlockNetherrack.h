#pragma once


#include "BlockHandler.h"

class cBlockNetherrack : public cBlockHandler
{
public:
	cBlockNetherrack(BLOCKTYPE a_Type) : cBlockHandler(a_Type){}

	virtual bool CanSustainPlant(BLOCKTYPE a_Plant) override { return (a_Plant == E_BLOCK_NETHER_WART); }
};
