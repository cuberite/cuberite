
// Vector3i.cpp

// Implements the Vector3i class representing an int-based 3D vector

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "math.h"
#include "Vector3i.h"
#include "Vector3d.h"





Vector3i::Vector3i(const Vector3d & v) :
	x((int)v.x),
	y((int)v.y),
	z((int)v.z)
{
}





Vector3i::Vector3i(void) :
	x(0),
	y(0),
	z(0)
{
}





Vector3i::Vector3i(int a_x, int a_y, int a_z) :
	x(a_x),
	y(a_y),
	z(a_z)
{
}





void Vector3i::Move(int a_MoveX, int a_MoveY, int a_MoveZ)
{
	x += a_MoveX;
	y += a_MoveY;
	z += a_MoveZ;
}




