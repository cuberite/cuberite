
// ItemBoat.h

// Declares the various boat ItemHandlers





#pragma once

#include "../Entities/Boat.h"
#include "../LineBlockTracer.h"





class cItemBoatHandler :
	public cItemHandler
{
	typedef cItemHandler super;
	
public:
	cItemBoatHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}
	
	
	
	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		if (a_Dir > 0)
		{
			return false;
		}

		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:
			Vector3d Pos;
			virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, char a_EntryFace) override
			{
				if (a_BlockType != E_BLOCK_AIR)
				{
					Pos = Vector3d(a_BlockX, a_BlockY, a_BlockZ);
					return true;
				}
				return false;
			}
		} Callbacks;
		
		cLineBlockTracer Tracer(*a_World, Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start.x, Start.y, Start.z, End.x, End.y, End.z);

		double x = Callbacks.Pos.x;
		double y = Callbacks.Pos.y;
		double z = Callbacks.Pos.z;
		
		if ((x == 0) && (y == 0) && (z == 0))
		{
			return false;
		}

		cBoat * Boat = new cBoat(x + 0.5, y + 1, z + 0.5);
		Boat->Initialize(a_World);

		return true;
	}
} ;




