#pragma once

class cStep												//tolua_export
{															//tolua_export
public:
	static char DirectionToMetaData( int a_Direction )		//tolua_export
	{														//tolua_export
    int result = 0x0;
    if( a_Direction == 0)
    {
      result = 0x8;
    }
    return result;
	}														//tolua_export
};															//tolua_export
