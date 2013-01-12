
#pragma once





class cStairs												// tolua_export
{															// tolua_export
public:
	/// Converts player rotation to stair rotation metadata. To get upside-down stairs, OR with 0x4
	static NIBBLETYPE RotationToMetaData(float a_Rotation)  // tolua_export
	{														// tolua_export
    a_Rotation += 90 + 45; // So its not aligned with axis
    NIBBLETYPE result = 0x0;
		if (a_Rotation > 360.f)
		{
			a_Rotation -= 360.f;
		}
		if ((a_Rotation >= 0.f) && (a_Rotation < 90.f))
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
	}														// tolua_export
} ;															// tolua_export




