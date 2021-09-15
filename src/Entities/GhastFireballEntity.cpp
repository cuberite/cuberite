#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "GhastFireballEntity.h"





cGhastFireballEntity::cGhastFireballEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed):
	Super(pkGhastFireball, a_Creator, a_Pos, 1, 1)
{
	SetSpeed(a_Speed);
	SetGravity(0);
	SetAirDrag(0);

	m_Shooter = a_Creator;
}





void cGhastFireballEntity::ReflectFromAttack(cEntity * a_AttackingEnt)
{
	Vector3d Motion = NotchReflect(*a_AttackingEnt);
	SetSpeed(Motion * 20);
}





Vector3d cGhastFireballEntity::NotchReflect(cEntity & a_AttackingEnt)
{
	float f1 = cos(-a_AttackingEnt.GetYaw() * 0.017453292F - 3.1415927F);
	float f2 = sin(-a_AttackingEnt.GetYaw() * 0.017453292F - 3.1415927F);
	float f3 = -cos(-a_AttackingEnt.GetPitch() * 0.017453292F);
	float f4 = sin(-a_AttackingEnt.GetPitch() * 0.017453292F);

	return Vector3d((double) (f2 * f3), (double) f4, (double) (f1 * f3));
}





void cGhastFireballEntity::Explode(Vector3i a_Block)
{
	m_World->DoExplosionAt(1, a_Block.x, a_Block.y, a_Block.z, true, esGhastFireball, this);
}





void cGhastFireballEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	Destroy();
	Explode(a_HitPos.Floor());
}





void cGhastFireballEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	Destroy();
	Explode(a_HitPos.Floor());
}





bool cGhastFireballEntity::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (this->IsInvisible())
	{
		return false;
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPawn())
	{
		ReflectFromAttack(a_TDI.Attacker);
		m_Shooter = a_TDI.Attacker;
		return true;
	}

	return false;
}
