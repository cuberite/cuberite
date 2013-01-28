
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
#include <cstdlib>
#endif

#include "Pickup.h"
#include "ClientHandle.h"
#include "Inventory.h"
#include "World.h"
#include "Simulator/FluidSimulator.h"
#include "Server.h"
#include "Player.h"
#include "PluginManager.h"
#include "Item.h"
#include "Root.h"
#include "Tracer.h"

#include "Vector3d.h"
#include "Vector3f.h"





cPickup::cPickup(int a_MicroPosX, int a_MicroPosY, int a_MicroPosZ, const cItem & a_Item, float a_SpeedX /* = 0.f */, float a_SpeedY /* = 0.f */, float a_SpeedZ /* = 0.f */)
	:	cEntity(etPickup, ((double)(a_MicroPosX)) / 32, ((double)(a_MicroPosY)) / 32, ((double)(a_MicroPosZ)) / 32)
	, m_Speed( a_SpeedX, a_SpeedY, a_SpeedZ )
	, m_bOnGround( false )
	, m_bReplicated( false )
	, m_Timer( 0.f )
	, m_Item(a_Item)
	, m_bCollected( false )
{
}





void cPickup::Initialize(cWorld * a_World)
{
	super::Initialize(a_World);
	a_World->BroadcastSpawn(*this);
}





void cPickup::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendPickupSpawn(*this);	
}





void cPickup::Tick(float a_Dt, MTRand & a_TickRandom)
{
	super::Tick(a_Dt, a_TickRandom);
	
	m_Timer += a_Dt;
	a_Dt = a_Dt / 1000.f;
	if (m_bCollected)
	{
		if (m_Timer > 500.f)  // 0.5 second
		{
			Destroy();
			return;
		}
	}

	if (m_Timer > 1000 * 60 * 5)  // 5 minutes
	{
		Destroy();
		return;
	}

	if (m_Pos.y < -8) // Out of this world and no more visible!
	{
		Destroy();
		return;
	}

	if (!m_bReplicated || m_bDirtyPosition)
	{
		MoveToCorrectChunk();
		m_bReplicated = true;
		m_bDirtyPosition = false;
		GetWorld()->BroadcastTeleportEntity(*this);
	}
}





void cPickup::HandlePhysics(float a_Dt)
{
	m_ResultingSpeed.Set(0.f, 0.f, 0.f);
	cWorld * World = GetWorld();
	
	a_Dt /= 1000;  // Go from msec to sec, so that physics formulas work

	if (m_bOnGround) // check if it's still on the ground
	{
		int BlockX = (m_Pos.x < 0) ? (int)m_Pos.x - 1 : (int)m_Pos.x;
		int BlockZ = (m_Pos.z < 0) ? (int)m_Pos.z - 1 : (int)m_Pos.z;
		char BlockBelow = World->GetBlock(BlockX, (int)m_Pos.y - 1, BlockZ);
		if (BlockBelow == E_BLOCK_AIR || IsBlockWater(BlockBelow))
		{
			m_bOnGround = false;
		}
		char Block = World->GetBlock(BlockX, (int)m_Pos.y - (int)m_bOnGround, BlockZ );
		char BlockIn = World->GetBlock(BlockX, (int)m_Pos.y, BlockZ );

		if( IsBlockLava(Block) || Block == E_BLOCK_FIRE
			|| IsBlockLava(BlockIn) || BlockIn == E_BLOCK_FIRE)
		{
			m_bCollected = true;
			m_Timer = 0;
			return;
		}

		if( BlockIn != E_BLOCK_AIR && !IsBlockWater(BlockIn) ) // If in ground itself, push it out
		{
			m_bOnGround = true;
			m_Pos.y += 0.2;
			m_bReplicated = false;
		}
		m_Speed.x *= 0.7f / (1 + a_Dt);
		if (fabs(m_Speed.x) < 0.05) m_Speed.x = 0;
		m_Speed.z *= 0.7f / (1 + a_Dt);
		if (fabs(m_Speed.z) < 0.05) m_Speed.z = 0;
	}

	// get flowing direction
	Direction WaterDir = World->GetWaterSimulator()->GetFlowingDirection((int) m_Pos.x - 1, (int) m_Pos.y, (int) m_Pos.z - 1);

	m_WaterSpeed *= 0.9f;		//Keep old speed but lower it

	switch(WaterDir)
	{
		case X_PLUS:
			m_WaterSpeed.x = 1.f;
			m_bOnGround = false;
			break;
		case X_MINUS:
			m_WaterSpeed.x = -1.f;
			m_bOnGround = false;
			break;
		case Z_PLUS:
			m_WaterSpeed.z = 1.f;
			m_bOnGround = false;
			break;
		case Z_MINUS:
			m_WaterSpeed.z = -1.f;
			m_bOnGround = false;
			break;
		
	default:
		break;
	}

	m_ResultingSpeed += m_WaterSpeed;

	if (!m_bOnGround)
	{

		float Gravity = -9.81f * a_Dt;
		if (Gravity < -3)  // Cap gravity-caused speed at 3 m / sec
		{
			Gravity = -3;
		}
		m_Speed.y += Gravity;

		// Set to hit position
		m_ResultingSpeed += m_Speed;
		
		/*
		LOGD("Pickup #%d speed: {%.03f, %.03f, %.03f}, pos {%.02f, %.02f, %.02f}", 
			m_UniqueID,
			m_ResultingSpeed.x, m_ResultingSpeed.y, m_ResultingSpeed.z,
			m_Pos.x, m_Pos.y, m_Pos.z
		);
		*/

		cTracer Tracer(GetWorld());
		int Ret = Tracer.Trace(m_Pos, m_Speed, 2);
		if (Ret) // Oh noez! we hit something
		{
			if ((Tracer.RealHit - Vector3f(m_Pos)).SqrLength() <= ( m_ResultingSpeed * a_Dt ).SqrLength())
			{
				m_bReplicated = false; // It's only interesting to replicate when we actually hit something...
				if (Ret == 1)
				{

					if (Tracer.HitNormal.x != 0.f ) m_Speed.x = 0.f;
					if (Tracer.HitNormal.y != 0.f ) m_Speed.y = 0.f;
					if (Tracer.HitNormal.z != 0.f ) m_Speed.z = 0.f;

					if (Tracer.HitNormal.y > 0) // means on ground
					{
						m_bOnGround = true;
					}
				}
				m_Pos = Tracer.RealHit;
				m_Pos += Tracer.HitNormal * 0.2f;

			}
			else
				m_Pos += m_ResultingSpeed * a_Dt;
		}
		else
		{	// We didn't hit anything, so move =]
			m_Pos += m_ResultingSpeed * a_Dt;
		}
	}
	// Usable for debugging
	SetPosition(m_Pos.x, m_Pos.y, m_Pos.z);
}





bool cPickup::CollectedBy(cPlayer * a_Dest)
{
	if (m_bCollected)
	{
		return false; // It's already collected!
	}
	
	// 800 is to long
	if (m_Timer < 500.f)
	{
		return false; // Not old enough
	}

	if (cRoot::Get()->GetPluginManager()->CallHookCollectingPickup(a_Dest, *this))
	{
		return false;
	}

	if (a_Dest->GetInventory().AddItemAnyAmount(m_Item))
	{
		m_World->BroadcastCollectPickup(*this, *a_Dest);
		m_bCollected = true;
		m_Timer = 0;
		if (m_Item.m_ItemCount != 0)
		{
			cItems Pickup;
			Pickup.push_back(cItem(m_Item));
			m_World->SpawnItemPickups(Pickup, m_Pos.x, m_Pos.y, m_Pos.z);
		}
		return true;
	}

	return false;
}




