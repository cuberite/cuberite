
#pragma once





// tolua_begin

class cStairs
{
public:
	/// Converts player rotation to stair rotation metadata. To get upside-down stairs, OR with 0x4
	static NIBBLETYPE RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45; // So its not aligned with axis
		NIBBLETYPE result = 0x0;
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}
		if ((a_Rotation >= 0) && (a_Rotation < 90))
		{
			return 0x0;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return 0x1;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return 0x2;
		}
		else
		{
			return 0x3;
		}
	}
} ;

// tolua_end



