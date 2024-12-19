
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderCrystal.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "../World.h"





cEnderCrystal::cEnderCrystal(Vector3d a_Pos, bool a_ShowBottom) :
	cEnderCrystal(a_Pos, {}, false, a_ShowBottom)
{
}





cEnderCrystal::cEnderCrystal(Vector3d a_Pos, Vector3i a_BeamTarget, bool a_DisplayBeam, bool a_ShowBottom) :
	Super(etEnderCrystal, a_Pos, 2.0f, 2.0f),
	m_BeamTarget(a_BeamTarget),
	m_DisplayBeam(a_DisplayBeam),
	m_ShowBottom(a_ShowBottom)
{
	SetMaxHealth(5);
}





void cEnderCrystal::SetShowBottom(bool a_ShowBottom)
{
	m_ShowBottom = a_ShowBottom;
	m_World->BroadcastEntityMetadata(*this);
}





void cEnderCrystal::SetBeamTarget(Vector3i a_BeamTarget)
{
	m_BeamTarget = a_BeamTarget;
	m_World->BroadcastEntityMetadata(*this);
}





void cEnderCrystal::SetDisplayBeam(bool a_DisplayBeam)
{
	m_DisplayBeam = a_DisplayBeam;
	m_World->BroadcastEntityMetadata(*this);
}





void cEnderCrystal::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnEntity(*this);
	a_ClientHandle.SendEntityMetadata(*this);
}





void cEnderCrystal::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	if ((m_World->GetDimension() == dimEnd) && (m_World->GetBlock(POS_TOINT).Type() != BlockType::Fire))
	{
		m_World->SetBlock(POS_TOINT, Block::Fire::Fire());
	}
}





void cEnderCrystal::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);

	// Destroy first so the Explodinator doesn't find us (when iterating through entities):
	Destroy();

	m_World->DoExplosionAt(6.0, GetPosX(), GetPosY() + GetHeight() / 2, GetPosZ(), true, esEnderCrystal, this);

	const auto Position = GetPosition().Floor();
	if (cChunkDef::IsValidHeight(Position))
	{
		m_World->SetBlock(Position, Block::Fire::Fire());
	}
}
