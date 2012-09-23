#pragma once

class cVine												//tolua_export
{														//tolua_export
public:
	
	static char DirectionToMetaData( char a_Direction )	//tolua_export
	{													//tolua_export
		switch (a_Direction)
		{
			case 0x2:
				return 0x1;
			case 0x3:
				return 0x4;
			case 0x4:
				return 0x8;
			case 0x5:
				return 0x2;
			default:
				return 0xf;
		};
	}													//tolua_export
};														//tolua_export