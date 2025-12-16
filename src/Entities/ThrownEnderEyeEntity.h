// ThrownEnderPearlEntity.h
// Declares the cThrownEnderPeralEntity class representing an ender pearl being thrown

#pragma once

#include "ProjectileEntity.h"

// tolua_begin

class cThrownEnderEyeEntity :
	public cProjectileEntity
{
	// tolua_end
	using Super = cProjectileEntity;

public:  // tolua_export

	CLASS_PROTODEF(cThrownEnderEyeEntity)

	/** Different from other thrown entities : 3rd parameter is the target position */
	cThrownEnderEyeEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_TargetPos);

private:
	bool m_SurviveAfterDeath;
	Vector3d m_DtVector;
	Vector3d m_Target;

	// cProjectileEntity overrides:
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	/** Called when EnderEye reaches the end of life. */
	void EndOfLife();
};  // tolua_export
