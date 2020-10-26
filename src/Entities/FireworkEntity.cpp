#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FireworkEntity.h"
#include "../World.h"
#include "../Chunk.h"





cFireworkEntity::cFireworkEntity(cEntity * a_Creator, Vector3d a_Pos, const cItem & a_Item) :
	Super(pkFirework, a_Creator, a_Pos, 0.25, 0.25),
	m_TicksToExplosion(a_Item.m_FireworkItem.m_FlightTimeInTicks),
	m_FireworkItem(a_Item)
{
	SetGravity(0);
	SetAirDrag(0);
}





void cFireworkEntity::HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	int RelX = POSX_TOINT - a_Chunk.GetPosX() * cChunkDef::Width;
	int RelZ = POSZ_TOINT - a_Chunk.GetPosZ() * cChunkDef::Width;
	int PosY = POSY_TOINT;

	if ((PosY < 0) || (PosY >= cChunkDef::Height))
	{
		AddSpeedY(1);
		AddPosition(GetSpeed() * (static_cast<double>(a_Dt.count()) / 1000));
		return;
	}

	if (m_IsInGround)
	{
		if (a_Chunk.GetBlock(RelX, POSY_TOINT + 1, RelZ) == E_BLOCK_AIR)
		{
			m_IsInGround = false;
		}
		else
		{
			return;
		}
	}
	else
	{
		if (a_Chunk.GetBlock(RelX, POSY_TOINT + 1, RelZ) != E_BLOCK_AIR)
		{
			OnHitSolidBlock(GetPosition(), BLOCK_FACE_YM);
			return;
		}
	}

	AddSpeedY(1);
	AddPosition(GetSpeed() * (static_cast<double>(a_Dt.count()) / 1000));
}





void cFireworkEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (m_TicksToExplosion <= 0)
	{
		// TODO: Notify the plugins
		m_World->BroadcastEntityStatus(*this, esFireworkExploding);
		Destroy();
		return;
	}

	m_TicksToExplosion -= 1;
}
