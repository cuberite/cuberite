#pragma once

#include "Vector3i.h"
#include "Vector3d.h"

class cCuboid	//tolua_export
{				//tolua_export
public:			//tolua_export
	cCuboid() {}			//tolua_export
	cCuboid( const cCuboid & a_Cuboid ) : p1( a_Cuboid.p1 ), p2( a_Cuboid.p2 ) {} //tolua_export
	cCuboid( const Vector3i & a_p1, const Vector3i & a_p2 ) : p1( a_p1 ), p2( a_p2 ) {} //tolua_export


	Vector3i p1, p2;		//tolua_export

	void Sort();			//tolua_export

	bool IsInside( const Vector3i & v )	const //tolua_export
	{									//tolua_export
		if(		v.x >= p1.x && v.x <= p2.x 
			&&	v.y >= p1.y && v.y <= p2.y
			&&	v.z >= p1.z && v.z <= p2.z )
		{
			return true;
		}
		return false;
	}									//tolua_export

	bool IsInside( const Vector3d & v ) const //tolua_export
	{									//tolua_export
		if(		v.x >= p1.x && v.x <= p2.x 
			&&	v.y >= p1.y && v.y <= p2.y
			&&	v.z >= p1.z && v.z <= p2.z )
		{
			return true;
		}
		return false;
	}									//tolua_export

};	//tolua_export