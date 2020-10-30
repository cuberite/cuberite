
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderCrystal.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "../World.h"





cEnderCrystal::cEnderCrystal(Vector3d a_Pos, bool a_ShowBottom):
	Super(etEnderCrystal, a_Pos, 1.0, 1.0),
	m_ShowBottom(a_ShowBottom),
	m_DisplayBeam(false)
{
	SetMaxHealth(5);
}





void cEnderCrystal::SetShowBottom(bool a_ShowBottom)
{
	m_ShowBottom = a_ShowBottom;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cEnderCrystal::SetBeamTarget(Vector3i a_BeamTarget)
{
	m_BeamTarget = a_BeamTarget;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cEnderCrystal::SetDisplayBeam(bool a_DisplayBeam)
{
	m_DisplayBeam = a_DisplayBeam;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cEnderCrystal::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnEntity(*this);
	a_ClientHandle.SendEntityMetadata(*this);
}





void cEnderCrystal::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	// No further processing (physics e.t.c.) is needed

	BLOCKTYPE Block;
	NIBBLETYPE Meta;
	m_World->GetBlockTypeMeta(POS_TOINT, Block, Meta);
	UNUSED(Meta);
	if ((m_World->GetDimension() == dimEnd) && (Block != E_BLOCK_FIRE))
	{
		m_World->SetBlock(POS_TOINT, E_BLOCK_FIRE, 0);
	}
}





void cEnderCrystal::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);
	Destroy();

	m_World->DoExplosionAt(6.0, GetPosX(), GetPosY(), GetPosZ(), true, esEnderCrystal, this);
	m_World->SetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT, E_BLOCK_FIRE, 0);
}




