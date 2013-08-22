
// ProjectileEntity.cpp

// Implements the cProjectileEntity class representing the common base class for projectiles, as well as individual projectile types

#include "Globals.h"
#include "ProjectileEntity.h"
#include "../ClientHandle.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProjectileEntity:

cProjectileEntity::cProjectileEntity(eKind a_Kind, cEntity * a_Creator, double a_X, double a_Y, double a_Z, double a_Width, double a_Height) :
	super(etProjectile, a_X, a_Y, a_Z, a_Width, a_Height),
	m_Creator(a_Creator)
{
}





cProjectileEntity::cProjectileEntity(eKind a_Kind, cEntity * a_Creator, const Vector3d & a_Pos, const Vector3d & a_Speed, double a_Width, double a_Height) :
	super(etProjectile, a_Pos.x, a_Pos.y, a_Pos.z, a_Width, a_Height),
	m_Creator(a_Creator)
{
	SetSpeed(a_Speed);
}





cProjectileEntity * cProjectileEntity::Create(eKind a_Kind, cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d * a_Speed)
{
	Vector3d Speed;
	if (a_Speed != NULL)
	{
		Speed = *a_Speed;
	}

	switch (a_Kind)
	{
		case pkArrow: return new cArrowEntity(a_Creator, a_X, a_Y, a_Z, Speed);
		// TODO: the rest
	}
	
	LOGWARNING("%s: Unknown kind: %d", __FUNCTION__, a_Kind);
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cArrowEntity:

cArrowEntity::cArrowEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d a_Speed) :
	super(pkArrow, a_Creator, a_X, a_Y, a_Z, 0.5, 0.5)
{
	SetSpeed(a_Speed);
}





void cArrowEntity::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnObject(*this, pkArrow, 0, 0, 0);
}




