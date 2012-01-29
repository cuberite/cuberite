
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Vector3f.h"
#include "Vector3d.h"
#include "Vector3i.h"

Vector3f::Vector3f( const Vector3d & v )
	: x( (float)v.x )
	, y( (float)v.y )
	, z( (float)v.z )
{
}

Vector3f::Vector3f( const Vector3d * v )
	: x( (float)v->x )
	, y( (float)v->y )
	, z( (float)v->z )
{
}

Vector3f::Vector3f( const Vector3i & v )
	: x( (float)v.x )
	, y( (float)v.y )
	, z( (float)v.z )
{
}

Vector3f::Vector3f( const Vector3i * v )
	: x( (float)v->x )
	, y( (float)v->y )
	, z( (float)v->z )
{
}