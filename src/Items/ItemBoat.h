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
	
	
	
	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		if ((a_Dir != BLOCK_FACE_YM) && (a_Dir != BLOCK_FACE_NONE))
		{
			return false;
		}

		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:
			Vector3d m_Pos;
			bool m_HasFound;
			
			cCallbacks(void) :
				m_HasFound(false)
			{
			}
			
			virtual bool OnNextBlock(int a_CBBlockX, int a_CBBlockY, int a_CBBlockZ, BLOCKTYPE a_CBBlockType, NIBBLETYPE a_CBBlockMeta, char a_CBEntryFace) override
			{
				if (a_CBBlockType != E_BLOCK_AIR)
				{
					m_Pos.Set(a_CBBlockX, a_CBBlockY, a_CBBlockZ);
					m_HasFound = true;
					return true;
				}
				return false;
			}
		} Callbacks;
		
		cLineBlockTracer Tracer(*a_World, Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start.x, Start.y, Start.z, End.x, End.y, End.z);

		if (!Callbacks.m_HasFound)
		{
			return false;
		}

		double x = Callbacks.m_Pos.x;
		double y = Callbacks.m_Pos.y;
		double z = Callbacks.m_Pos.z;
		
		cBoat * Boat = new cBoat(x + 0.5, y + 1, z + 0.5);
		Boat->Initialize(*a_World);

		return true;
	}
} ;




