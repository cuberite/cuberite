
#pragma once



#define _USE_MATH_DEFINES  // Enable non-standard math defines (MSVC)
#include <math.h>





template <typename T>
// tolua_begin
class Matrix4
{

	TOLUA_TEMPLATE_BIND((T, float, double))

	// tolua_end

public:

	T cell[16];

	// tolua_begin

	inline Matrix4(void)
	{
		Identity();
	}

	inline Matrix4(const Matrix4 & a_Rhs)
	{
		*this = a_Rhs;
	}

	// tolua_end

	inline Matrix4 & operator = (const Matrix4 & a_Rhs)
	{
		for (unsigned int i = 0; i < 16; ++i)
		{
			cell[i] = a_Rhs.cell[i];
		}
		return *this;
	}

	// tolua_begin

	inline T & operator [] (int a_N)
	{
		ASSERT(a_N < 16);
		return cell[a_N];
	}

	inline void Identity()
	{
		cell[1]  = cell[2]  = cell[3]  = cell[4]  = 0;
		cell[6]  = cell[7]  = cell[8]  = cell[9]  = 0;
		cell[11] = cell[12] = cell[13] = cell[14] = 0;

		cell[0] = cell[5] = cell[10] = cell[15] = 1;
	}

	inline void Init(const Vector3<T> & a_Pos, T a_RX, T a_RY, T a_RZ)
	{
		Matrix4<T> t;
		t.RotateX(a_RZ);
		RotateY(a_RY);
		Concatenate(t);
		t.RotateZ(a_RX);
		Concatenate(t);
		Translate(a_Pos);
	}

	inline void RotateX(T a_RX)
	{
		T sx = static_cast<T>(sin(a_RX * M_PI / 180));
		T cx = static_cast<T>(cos(a_RX * M_PI / 180));

		Identity();

		cell[5] = cx;  cell[6]  = sx;
		cell[9] = -sx; cell[10] = cx;
	}

	inline void RotateY(T a_RY)
	{
		T sy = static_cast<T>(sin(a_RY * M_PI / 180));
		T cy = static_cast<T>(cos(a_RY * M_PI / 180));

		Identity();

		cell[0] = cy; cell[2] = -sy;
		cell[8] = sy; cell[10] = cy;
	}

	inline void RotateZ(T a_RZ)
	{
		T sz = static_cast<T>(sin(a_RZ * M_PI / 180));
		T cz = static_cast<T>(cos(a_RZ * M_PI / 180));

		Identity();

		cell[0] = cz;  cell[1] = sz;
		cell[4] = -sz; cell[5] = cz;
	}

	inline void Translate(const Vector3<T> & a_Pos)
	{
		cell[3]  += a_Pos.x;
		cell[7]  += a_Pos.y;
		cell[11] += a_Pos.z;
	}

	inline void SetTranslation(const Vector3<T> & a_Pos)
	{
		cell[3]  = a_Pos.x;
		cell[7]  = a_Pos.y;
		cell[11] = a_Pos.z;
	}

	inline void Concatenate(const Matrix4 & m2)
	{
		Matrix4 res;

		for (unsigned int c = 0; c < 4; ++c)
		{
			for (unsigned int r = 0; r < 4; ++r)
			{
				res.cell[r * 4 + c] = (
					cell[r * 4 + 0] * m2.cell[c + 0] +
					cell[r * 4 + 1] * m2.cell[c + 4] +
					cell[r * 4 + 2] * m2.cell[c + 8] +
					cell[r * 4 + 3] * m2.cell[c + 12]
				);
			}
		}

		*this = res;
	}

	inline Vector3<T> Transform(const Vector3<T> & v) const
	{
		T x  = cell[0] * v.x + cell[1] * v.y + cell[2]  * v.z + cell[3];
		T y  = cell[4] * v.x + cell[5] * v.y + cell[6]  * v.z + cell[7];
		T z  = cell[8] * v.x + cell[9] * v.y + cell[10] * v.z + cell[11];

		return Vector3<T>(x, y, z);
	}

	inline void Invert(void)
	{
		Matrix4 t;

		T tx = -cell[3];
		T ty = -cell[7];
		T tz = -cell[11];

		for (unsigned int h = 0; h < 3; ++h)
		{
			for (unsigned int v = 0; v < 3; ++v)
			{
				t.cell[h + v * 4] = cell[v + h * 4];
			}
		}

		for (unsigned int i = 0; i < 11; ++i)
		{
			cell[i] = t.cell[i];
		}

		cell[3]  = tx * cell[0] + ty * cell[1] + tz * cell[2];
		cell[7]  = tx * cell[4] + ty * cell[5] + tz * cell[6];
		cell[11] = tx * cell[8] + ty * cell[9] + tz * cell[10];
	}

	inline Vector3<T> GetXColumn(void) const
	{
		return Vector3<T>(cell[0], cell[1], cell[2]);
	}

	inline Vector3<T> GetYColumn(void) const
	{
		return Vector3<T>(cell[4], cell[5], cell[6]);
	}

	inline Vector3<T> GetZColumn(void) const
	{
		return Vector3<T>(cell[8], cell[9], cell[10]);
	}

	inline void SetXColumn(const Vector3<T> & a_X)
	{
		cell[0] = a_X.x;
		cell[1] = a_X.y;
		cell[2] = a_X.z;
	}

	inline void SetYColumn(const Vector3<T> & a_Y)
	{
		cell[4] = a_Y.x;
		cell[5] = a_Y.y;
		cell[6] = a_Y.z;
	}

	inline void SetZColumn(const Vector3<T> & a_Z)
	{
		cell[8]  = a_Z.x;
		cell[9]  = a_Z.y;
		cell[10] = a_Z.z;
	}
};
// tolua_end




// tolua_begin
typedef Matrix4<double> Matrix4d;
typedef Matrix4<float>  Matrix4f;
// tolua_end





