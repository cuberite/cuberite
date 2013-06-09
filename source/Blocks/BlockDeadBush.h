
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockDeadBushHandler :
    public cBlockHandler
{
public:
    cBlockDeadBushHandler(BLOCKTYPE a_BlockType)
        : cBlockHandler(a_BlockType)
    {
    }


    virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
    {
        a_Pickups.push_back(cItem(E_BLOCK_DEAD_BUSH, 1, a_BlockMeta));
    }


    virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
    {
        return (a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_SAND);
    }


    virtual bool DoesAllowBlockOnTop(void) override
    {
        return false;
    }


    virtual bool CanBePlacedOnSide() override
    {
        return false;
    }
} ;




