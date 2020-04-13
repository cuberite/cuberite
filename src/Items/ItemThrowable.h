
// Declares the itemhandlers for throwable items: eggs, snowballs, ender pearls, and eyes of ender.

#pragma once





class cItemThrowableHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemThrowableHandler(int a_ItemType, cProjectileEntity::eKind a_ProjectileKind, double a_SpeedCoeff):
		Super(a_ItemType),
		m_ProjectileKind(a_ProjectileKind),
		m_SpeedCoeff(a_SpeedCoeff)
	{
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		Vector3d Pos = a_Player->GetThrowStartPos();
		Vector3d Speed = a_Player->GetLookVector() * m_SpeedCoeff;

		// Play sound
		a_World->BroadcastSoundEffect("entity.arrow.shoot", a_Player->GetPosition() - Vector3d(0, a_Player->GetHeight(), 0), 0.5f, 0.4f / GetRandomProvider().RandReal(0.8f, 1.2f));

		if (a_World->CreateProjectile(Pos.x, Pos.y, Pos.z, m_ProjectileKind, a_Player, &a_Player->GetEquippedItem(), &Speed) == cEntity::INVALID_ID)
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





class cItemEggHandler:
	public cItemThrowableHandler
{
	using Super = cItemThrowableHandler;

public:

	cItemEggHandler():
		Super(E_ITEM_EGG, cProjectileEntity::pkEgg, 30)
	{
	}
} ;




class cItemSnowballHandler:
	public cItemThrowableHandler
{
	using Super = cItemThrowableHandler;

public:

	cItemSnowballHandler():
		Super(E_ITEM_SNOWBALL, cProjectileEntity::pkSnowball, 30)
	{
	}
} ;





class cItemEnderPearlHandler:
	public cItemThrowableHandler
{
	using Super = cItemThrowableHandler;

public:

	cItemEnderPearlHandler():
		Super(E_ITEM_ENDER_PEARL, cProjectileEntity::pkEnderPearl, 30)
	{
	}
} ;





class cItemBottleOEnchantingHandler:
	public cItemThrowableHandler
{
	using Super = cItemThrowableHandler;

public:

	cItemBottleOEnchantingHandler():
		Super(E_ITEM_BOTTLE_O_ENCHANTING, cProjectileEntity::pkExpBottle, 14)
	{
	}
};





class cItemFireworkHandler:
	public cItemThrowableHandler
{
	using Super = cItemThrowableHandler;

public:

	cItemFireworkHandler():
		Super(E_ITEM_FIREWORK_ROCKET, cProjectileEntity::pkFirework, 0)
	{
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_AIR)
		{
			return false;
		}

		if (a_World->CreateProjectile(a_BlockX + 0.5, a_BlockY + 1, a_BlockZ + 0.5, m_ProjectileKind, a_Player, &a_Player->GetEquippedItem()) == 0)
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
