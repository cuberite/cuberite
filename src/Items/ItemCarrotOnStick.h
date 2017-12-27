
// ItemCarrotOnStick.h

// Declares the carrot on a stick ItemHandler





#pragma once





class cItemCarrotOnStickHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemCarrotOnStickHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}



	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action)
	{
		switch (a_Action)
		{
			case dlaCarrotOnStickBoost:  return 7;
			default:                     return 0;
		}
	}
} ;
