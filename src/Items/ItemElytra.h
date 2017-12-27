
#pragma once





class cItemElytraHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemElytraHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}



	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action)
	{
		switch (a_Action)
		{
			case dlaElytraFly:     return 1;
			default:               return 0;
		}
	}
} ;
