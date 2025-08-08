#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownEnderEyeEntity.h"
#include "Player.h"
#include "FastRandom.h"





cThrownEnderEyeEntity::cThrownEnderEyeEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_TargetPos):
	// Set speed to zero, but use the data for eye of ender calculations
	Super(pkEnderEye, a_Creator, a_Pos, {0,0,0}, 0.25f, 0.25f),
	// 50% chance of surviving after death (hardcoded)
	m_SurviveAfterDeath(GetRandomProvider().RandBool(0.5))
	// m_SurviveAfterDeath(false)
{
	m_IsInGround = false;
	static const double diveDist = 12.0;
	double deltaX = a_TargetPos.x - a_Pos.x;
	double deltaZ = a_TargetPos.z - a_Pos.z;
	double distXZ = sqrt(deltaX * deltaX + deltaZ * deltaZ);
	if (distXZ > diveDist) {
		a_Pos += {
			deltaX / distXZ * diveDist,
			8,
			deltaZ / distXZ * diveDist
		};
		m_Target = a_Pos;
	} else {
		m_Target = a_TargetPos;
	}
}





void cThrownEnderEyeEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) { }





void cThrownEnderEyeEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) { }





void cThrownEnderEyeEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) {
	Super::Tick(a_Dt, a_Chunk);

	// Death of the ender eye if old enough
	if (m_TicksAlive > 80) EndOfLife();
}





void cThrownEnderEyeEntity::HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) {
	static const float radToDegrees = 57.2957763671875;  // Store somewhere else?
	double DtSec = std::chrono::duration_cast<std::chrono::duration<double>>(a_Dt).count();

	// Position calculations
	Vector3d Pos = GetPosition();
	double vectorXZ = sqrt(m_DtVector.x * m_DtVector.x + m_DtVector.z * m_DtVector.z);
	double distRemainX = m_Target.x - Pos.x;
	double distRemainZ = m_Target.z - Pos.z;
	double remainingXZ = sqrt(distRemainX * distRemainX + distRemainZ * distRemainZ);
	double radianValue = atan2(distRemainZ, distRemainX);
	double scaler = vectorXZ + (remainingXZ - vectorXZ) * .05 * DtSec;
	double vectorY = m_DtVector.y;
	if (remainingXZ < 1.0) {
		scaler *= .8;
		vectorY *= .8;
	}
	int riseBool = Pos.y < m_Target.y ? 1 : -1;
	m_DtVector.Set(
		cos(radianValue) * scaler,
		vectorY + (riseBool - vectorY) * .3 * DtSec,
		sin(radianValue) * scaler
	);

	// Angle calculations
	double xRot0 = GetYaw();
	double yRot0 = GetPitch();
	double yRot = atan2(m_DtVector.x, m_DtVector.z) * radToDegrees;
	double xRot = atan2(m_DtVector.y, vectorXZ) * radToDegrees;
	xRot0 = round((xRot - xRot0) / 360.) * 360 + xRot0;
	yRot0 = round((yRot - yRot0) / 360.) * 360 + yRot0;

	Pos += m_DtVector;
	
	SetYaw(xRot0 + (xRot - xRot0) * DtSec * 4);
	SetPitch(yRot0 + (yRot - yRot0) * DtSec * 4);
	SetPosition(Pos);
	SetSpeed(m_DtVector/DtSec);
}





void cThrownEnderEyeEntity::EndOfLife() {
	Vector3d Pos = GetPosition();
	if (m_SurviveAfterDeath) {
		cItems Pickups;
		Pickups.Add(static_cast<ENUM_ITEM_TYPE>(E_ITEM_EYE_OF_ENDER), 1);
		m_World->SpawnItemPickups(Pickups, Pos, {
			GetRandomProvider().RandReal(-2.5, 2.5),
			5,
			GetRandomProvider().RandReal(-2.5, 2.5)
		});
		m_World->BroadcastSoundEffect(SoundEvent::EnderEyeCustomSurvive, Pos, 1.0f, 0.8f);
	} else {
		m_World->BroadcastSoundEffect(SoundEvent::EnderEyeDeath, Pos, 1.0f, 0.8f);
		m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_EYE_OF_ENDER, Pos, 0, 0);
	}
	m_IsInGround = true;
	Destroy();
}
