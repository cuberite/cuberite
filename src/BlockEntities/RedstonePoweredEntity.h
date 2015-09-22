
// RedstonePoweredEntity.h

// Declares the cRedstonePoweredEntity class representing a mix-in for block entities that respond to redstone





#pragma once





/** Interface class representing a mix-in for block entities that respond to redstone */
class cRedstonePoweredEntity
{
public:

	virtual ~cRedstonePoweredEntity() {}

	/** Sets the internal redstone power flag to "on" or "off", depending on the parameter.
	Calls Activate() if appropriate */
	virtual void SetRedstonePower(bool a_IsPowered) = 0;
};




