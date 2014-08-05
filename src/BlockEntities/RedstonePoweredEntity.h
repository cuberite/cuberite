

// Interface class representing a blockEntity that responds to redstone
class cRedstonePoweredEntity
{
public:
/// Sets the internal redstone power flag to "on" or "off", depending on the parameter. Calls Activate() if appropriate
	virtual void SetRedstonePower(bool a_IsPowered) = 0;
};
