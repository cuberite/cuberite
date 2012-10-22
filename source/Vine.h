#pragma once

class cVine												//tolua_export
{														//tolua_export
public:
	
	static NIBBLETYPE DirectionToMetaData( char a_Direction )	//tolua_export
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
				return 0x0;
		};
	}													//tolua_export

	static char MetaDataToDirection(NIBBLETYPE a_MetaData )		//tolua_export
	{														//tolua_export
		switch(a_MetaData)
		{
			case 0x1:
				return 0x2;
			case 0x4:
				return 0x3;
			case 0x8:
				return 0x4;
			case 0x2:
				return 0x5;
			default:
				return 0x1;
		};
	}														//tolua_export

};														//tolua_export
