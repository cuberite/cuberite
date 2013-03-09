
#pragma once





// tolua_begin
class cSign
{
public:
	static char RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 180 + (180 / 16);  // So it's not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}
		
		a_Rotation = (a_Rotation / 360) * 16;
		
		return ((char)a_Rotation) % 16;
	}
	
	
	static char DirectionToMetaData(char a_Direction)
	{
		switch (a_Direction)
		{
			case 0x2: return 0x2;
			case 0x3: return 0x3;
			case 0x4: return 0x4;
			case 0x5: return 0x5;
			default:
				break;
		};
		return 0x2;
	}	
} ;
// tolua_end




