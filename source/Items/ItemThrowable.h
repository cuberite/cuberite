
// ItemThrowable.h

// Declares the itemhandlers for throwable items: eggs, snowballs and ender pearls





#pragma once





class cItemThrowableHandler :
	public cItemHandler
{
	typedef cItemHandler super;
public:
	cItemThrowableHandler(int a_ItemType, cProjectileEntity::eKind a_ProjectileKind, double a_SpeedCoeff) :
		super(a_ItemType),
		m_ProjectileKind(a_ProjectileKind),
		m_SpeedCoeff(a_SpeedCoeff)
	{
	}
	

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		Vector3d Pos = a_Player->GetThrowStartPos();
		Vector3d Speed = a_Player->GetLookVector() * m_SpeedCoeff;
		a_World->CreateProjectile(Pos.x, Pos.y, Pos.z, m_ProjectileKind, a_Player, &Speed);
		return true;
	}
	
protected:
	cProjectileEntity::eKind m_ProjectileKind;
	double m_SpeedCoeff;
} ;





class cItemEggHandler :
	public cItemThrowableHandler
{
	typedef cItemThrowableHandler super;
public:
	cItemEggHandler(void) :
		super(E_ITEM_EGG, cProjectileEntity::pkEgg, 30)
	{
	}
} ;




class cItemSnowballHandler :
	public cItemThrowableHandler
{
	typedef cItemThrowableHandler super;
	
public:
	cItemSnowballHandler(void) :
		super(E_ITEM_SNOWBALL, cProjectileEntity::pkSnowball, 30)
	{
	}
} ;





class cItemEnderPearlHandler :
	public cItemThrowableHandler
{
	typedef cItemThrowableHandler super;
	
public:
	cItemEnderPearlHandler(void) :
		super(E_ITEM_ENDER_PEARL, cProjectileEntity::pkEnderPearl, 30)
	{
	}
} ;




