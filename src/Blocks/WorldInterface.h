
#pragma once

class cWorldInterface
{
public:

	virtual Int64 GetTimeOfDay(void) const;
	
	virtual eDimension GetDimension(void) const;
	
	virtual void BroadcastUseBed             (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ );
};
