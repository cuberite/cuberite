
#pragma once



#define _USE_MATH_DEFINES  // Enable non-standard math defines (MSVC)
#include <math.h>
#include <list>
#include <vector>





template <typename T>
// tolua_begin
class Vector3
{

	TOLUA_TEMPLATE_BIND((T, int, float, double))

public:

	T x, y, z;


	inline Vector3(void) : x(0), y(0), z(0) {}
	inline Vector3(T a_x, T a_y, T a_z) : x(a_x), y(a_y), z(a_z) {}


	// Hardcoded copy constructors (tolua++ does not support function templates .. yet)
	Vector3(const Vector3<float>  & a_Rhs) : x((T) a_Rhs.x), y((T) a_Rhs.y), z((T) a_Rhs.z) {}
	Vector3(const Vector3<double> & a_Rhs) : x((T) a_Rhs.x), y((T) a_Rhs.y), z((T) a_Rhs.z) {}
	Vector3(const Vector3<int>    & a_Rhs) : x((T) a_Rhs.x), y((T) a_Rhs.y), z((T) a_Rhs.z) {}


	// tolua_end
	template <typename _T>
	Vector3(const Vector3<_T> & a_Rhs) : x(a_Rhs.x), y(a_Rhs.y), z(a_Rhs.z) {}

	template <typename _T>
	Vector3(const Vector3<_T> * a_Rhs) : x(a_Rhs->x), y(a_Rhs->y), z(a_Rhs->z) {}
	// tolua_begin

	inline void Set(T a_x, T a_y, T a_z)
	{
		x = a_x;
		y = a_y;
		z = a_z;
	}

	inline void Normalize(void)
	{
		double Len = 1.0 / Length();

		x = (T)(x * Len);
		y = (T)(y * Len);
		z = (T)(z * Len);
	}

	inline Vector3<T> NormalizeCopy(void) const
	{
		double Len = 1.0 / Length();

		return Vector3<T>(
			(T)(x * Len),
			(T)(y * Len),
			(T)(z * Len)
		);
	}

	inline void NormalizeCopy(Vector3<T> & a_Rhs) const
	{
		double Len = 1.0 / Length();

		a_Rhs.Set(
			(T)(x * Len),
			(T)(y * Len),
			(T)(z * Len)
		);
	}

	inline double Length(void) const
	{
		return sqrt((double)(x * x + y * y + z * z));
	}

	inline double SqrLength(void) const
	{
		return x * x + y * y + z * z;
	}

	inline T Dot(const Vector3<T> & a_Rhs) const
	{
		return x * a_Rhs.x + y * a_Rhs.y + z * a_Rhs.z;
	}

	inline Vector3<T> Cross(const Vector3<T> & a_Rhs) const
	{
		return Vector3<T>(
			y * a_Rhs.z - z * a_Rhs.y,
			z * a_Rhs.x - x * a_Rhs.z,
			x * a_Rhs.y - y * a_Rhs.x
		);
	}

	inline bool Equals(const Vector3<T> & a_Rhs) const
	{
		// Perform a bitwise comparison of the contents - we want to know whether this object is exactly equal
		// To perform EPS-based comparison, use the EqualsEps() function
		return (
			(memcmp(&x, &a_Rhs.x, sizeof(x)) == 0) &&
			(memcmp(&y, &a_Rhs.y, sizeof(y)) == 0) &&
			(memcmp(&z, &a_Rhs.z, sizeof(z)) == 0)
		);
	}
	
	inline bool EqualsEps(const Vector3<T> & a_Rhs, T a_Eps) const
	{
		return (Abs(x - a_Rhs.x) < a_Eps) && (Abs(y - a_Rhs.y) < a_Eps) && (Abs(z - a_Rhs.z) < a_Eps);
	}

	inline void Move(T a_X, T a_Y, T a_Z)
	{
		x += a_X;
		y += a_Y;
		z += a_Z;
	}

	inline void Move(const Vector3<T> & a_Diff)
	{
		x += a_Diff.x;
		y += a_Diff.y;
		z += a_Diff.z;
	}

	/** Runs each value of the vector through std::floor() */
	inline Vector3<int> Floor(void) const
	{
		return Vector3<int>(
			(int)floor(x),
			(int)floor(y),
			(int)floor(z)
		);
	}

	// tolua_end

	inline bool operator != (const Vector3<T> & a_Rhs) const
	{
		return !Equals(a_Rhs);
	}

	inline bool operator == (const Vector3<T> & a_Rhs) const
	{
		return Equals(a_Rhs);
	}

	inline bool operator > (const Vector3<T> & a_Rhs) const
	{
		return (SqrLength() > a_Rhs.SqrLength());
	}

	inline bool operator < (const Vector3<T> & a_Rhs) const
	{
		return (SqrLength() < a_Rhs.SqrLength());
	}

	inline void operator += (const Vector3<T> & a_Rhs)
	{
		x += a_Rhs.x;
		y += a_Rhs.y;
		z += a_Rhs.z;
	}

	inline void operator -= (const Vector3<T> & a_Rhs)
	{
		x -= a_Rhs.x;
		y -= a_Rhs.y;
		z -= a_Rhs.z;
	}

	inline void operator *= (const Vector3<T> & a_Rhs)
	{
		x *= a_Rhs.x;
		y *= a_Rhs.y;
		z *= a_Rhs.z;
	}

	inline void operator *= (T a_v)
	{
		x *= a_v;
		y *= a_v;
		z *= a_v;
	}

	inline Vector3<T> & operator = (const Vector3<T> & a_Rhs)
	{
		x = a_Rhs.x;
		y = a_Rhs.y;
		z = a_Rhs.z;
		return *this;
	}

	// tolua_begin
	
	inline Vector3<T> operator + (const Vector3<T>& a_Rhs) const
	{
		return Vector3<T>(
			x + a_Rhs.x,
			y + a_Rhs.y,
			z + a_Rhs.z
		);
	}

	inline Vector3<T> operator - (const Vector3<T>& a_Rhs) const
	{
		return Vector3<T>(
			x - a_Rhs.x,
			y - a_Rhs.y,
			z - a_Rhs.z
		);
	}

	inline Vector3<T> operator * (const Vector3<T>& a_Rhs) const
	{
		return Vector3<T>(
			x * a_Rhs.x,
			y * a_Rhs.y,
			z * a_Rhs.z
		);
	}

	inline Vector3<T> operator * (T a_v) const
	{
		return Vector3<T>(
			x * a_v,
			y * a_v,
			z * a_v
		);
	}

	inline Vector3<T> operator / (T a_v) const
	{
		return Vector3<T>(
			x / a_v,
			y / a_v,
			z / a_v
		);
	}

	/** Returns the coefficient for the (a_OtherEnd - this) line to reach the specified Z coord.
	The result satisfies the following equation:
	(*this + Result * (a_OtherEnd - *this)).z = a_Z
	If the line is too close to being parallel, this function returns NO_INTERSECTION
	*/
	inline double LineCoeffToXYPlane(const Vector3<T> & a_OtherEnd, T a_Z) const
	{
		if (Abs(z - a_OtherEnd.z) < EPS)
		{
			return NO_INTERSECTION;
		}

		return (a_Z - z) / (a_OtherEnd.z - z);
	}

	/** Returns the coefficient for the (a_OtherEnd - this) line to reach the specified Y coord.
	The result satisfies the following equation:
	(*this + Result * (a_OtherEnd - *this)).y = a_Y
	If the line is too close to being parallel, this function returns NO_INTERSECTION
	*/
	inline double LineCoeffToXZPlane(const Vector3<T> & a_OtherEnd, T a_Y) const
	{
		if (Abs(y - a_OtherEnd.y) < EPS)
		{
			return NO_INTERSECTION;
		}

		return (a_Y - y) / (a_OtherEnd.y - y);
	}

	/** Returns the coefficient for the (a_OtherEnd - this) line to reach the specified X coord.
	The result satisfies the following equation:
	(*this + Result * (a_OtherEnd - *this)).x = a_X
	If the line is too close to being parallel, this function returns NO_INTERSECTION
	*/
	inline double LineCoeffToYZPlane(const Vector3<T> & a_OtherEnd, T a_X) const
	{
		if (Abs(x - a_OtherEnd.x) < EPS)
		{
			return NO_INTERSECTION;
		}

		return (a_X - x) / (a_OtherEnd.x - x);
	}

	/** The max difference between two coords for which the coords are assumed equal. */
	static const double EPS;

	/** Return value of LineCoeffToPlane() if the line is parallel to the plane. */
	static const double NO_INTERSECTION;
	
protected:

	/** Returns the absolute value of the given argument.
	Templatized because the standard library differentiates between abs() and fabs(). */
	static T Abs(T a_Value)
	{
		return (a_Value < 0) ? -a_Value : a_Value;
	}

};
// tolua_end





template <> inline Vector3<int> Vector3<int>::Floor(void) const
{
	return *this;
}





template <typename T>
const double Vector3<T>::EPS = 0.000001;

template <typename T>
const double Vector3<T>::NO_INTERSECTION = 1e70;





// tolua_begin
typedef Vector3<double> Vector3d;
typedef Vector3<float>  Vector3f;
typedef Vector3<int>    Vector3i;
// tolua_end





typedef std::list<Vector3i>   cVector3iList;
typedef std::vector<Vector3i> cVector3iArray;






