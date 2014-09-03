
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
	

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		Vector3d Pos = a_Player->GetThrowStartPos();
		Vector3d Speed = a_Player->GetLookVector() * m_SpeedCoeff;

		// Play sound
		cFastRandom Random;
		a_World->BroadcastSoundEffect("random.bow", a_Player->GetPosX(), a_Player->GetPosY() - a_Player->GetHeight(), a_Player->GetPosZ(), 0.5f, 0.4f / (Random.NextFloat(1.0f) * 0.4f + 0.8f));

		if (a_World->CreateProjectile(Pos.x, Pos.y, Pos.z, m_ProjectileKind, a_Player, &a_Player->GetEquippedItem(), &Speed) < 0)
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

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





class cItemBottleOEnchantingHandler :
	public cItemThrowableHandler
{
	typedef cItemThrowableHandler super;
public:
	cItemBottleOEnchantingHandler(void) :
		super(E_ITEM_BOTTLE_O_ENCHANTING, cProjectileEntity::pkExpBottle, 10)
	{
	}
};





class cItemFireworkHandler :
	public cItemThrowableHandler
{
	typedef cItemThrowableHandler super;
public:
	cItemFireworkHandler(void) :
		super(E_ITEM_FIREWORK_ROCKET, cProjectileEntity::pkFirework, 0)
	{
	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		if (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_AIR)
		{
			return false;
		}

		if (a_World->CreateProjectile(a_BlockX + 0.5, a_BlockY + 1, a_BlockZ + 0.5, m_ProjectileKind, a_Player, &a_Player->GetEquippedItem()) < 0)
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}

};
