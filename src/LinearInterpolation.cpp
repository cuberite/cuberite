
// LinearInterpolation.cpp

// Implements methods for linear interpolation over 1D, 2D and 3D arrays

#include "Globals.h"
#include "LinearInterpolation.h"





/*
// Perform an automatic test upon program start (use breakpoints to debug):

extern void Debug3DNoise(float * a_Noise, int a_SizeX, int a_SizeY, int a_SizeZ, const AString & a_FileNameBase);

class Test
{
public:
	Test(void)
	{
		// DoTest1();
		DoTest2();
	}


	void DoTest1(void)
	{
		float In[8] = {0, 1, 2, 3, 1, 2, 2, 2};
		float Out[3 * 3 * 3];
		LinearInterpolate1DArray(In, 4, Out, 9);
		LinearInterpolate2DArray(In, 2, 2, Out, 3, 3);
		LinearInterpolate3DArray(In, 2, 2, 2, Out, 3, 3, 3);
		LOGD("Out[0]: %f", Out[0]);
	}


	void DoTest2(void)
	{
		float In[3 * 3 * 3];
		for (size_t i = 0; i < ARRAYCOUNT(In); i++)
		{
			In[i] = (float)(i % 5);
		}
		float Out[15 * 16 * 17];
		LinearInterpolate3DArray(In, 3, 3, 3, Out, 15, 16, 17);
		Debug3DNoise(Out, 15, 16, 17, "LERP test");
	}
} gTest;
//*/





// Puts linearly interpolated values from one array into another array. 1D version
void LinearInterpolate1DArray(
	float * a_Src,
	int a_SrcSizeX,
	float * a_Dst,
	int a_DstSizeX
)
{
	a_Dst[0] = a_Src[0];
	int DstSizeXm1 = a_DstSizeX - 1;
	int SrcSizeXm1 = a_SrcSizeX - 1;
	float fDstSizeXm1 = static_cast<float>(DstSizeXm1);
	float fSrcSizeXm1 = static_cast<float>(SrcSizeXm1);
	for (int x = 1; x < DstSizeXm1; x++)
	{
		int SrcIdx = x * SrcSizeXm1 / DstSizeXm1;
		float ValLo = a_Src[SrcIdx];
		float ValHi = a_Src[SrcIdx + 1];
		float Ratio = static_cast<float>(x) * fSrcSizeXm1 / fDstSizeXm1 - SrcIdx;
		a_Dst[x] = ValLo + (ValHi - ValLo) * Ratio;
	}
	a_Dst[a_DstSizeX - 1] = a_Src[a_SrcSizeX - 1];
}





// Puts linearly interpolated values from one array into another array. 2D version
void LinearInterpolate2DArray(
	float * a_Src,
	int a_SrcSizeX, int a_SrcSizeY,
	float * a_Dst,
	int a_DstSizeX, int a_DstSizeY
)
{
	ASSERT(a_DstSizeX > 0);
	ASSERT(a_DstSizeX < MAX_INTERPOL_SIZEX);
	ASSERT(a_DstSizeY > 0);
	ASSERT(a_DstSizeY < MAX_INTERPOL_SIZEY);

	// Calculate interpolation ratios and src indices along each axis:
	float RatioX[MAX_INTERPOL_SIZEX];
	float RatioY[MAX_INTERPOL_SIZEY];
	int   SrcIdxX[MAX_INTERPOL_SIZEX];
	int   SrcIdxY[MAX_INTERPOL_SIZEY];
	for (int x = 1; x < a_DstSizeX; x++)
	{
		SrcIdxX[x] = x * (a_SrcSizeX - 1) / (a_DstSizeX - 1);
		RatioX[x] = (static_cast<float>(x * (a_SrcSizeX - 1)) / (a_DstSizeX - 1)) - SrcIdxX[x];
	}
	for (int y = 1; y < a_DstSizeY; y++)
	{
		SrcIdxY[y] = y * (a_SrcSizeY - 1) / (a_DstSizeY - 1);
		RatioY[y] = (static_cast<float>(y * (a_SrcSizeY - 1)) / (a_DstSizeY - 1)) - SrcIdxY[y];
	}

	// Special values at the ends. Notice especially the last indices being (size - 2) with ratio set to 1, to avoid index overflow:
	SrcIdxX[0] = 0;
	RatioX[0] = 0;
	SrcIdxY[0] = 0;
	RatioY[0] = 0;
	SrcIdxX[a_DstSizeX - 1] = a_SrcSizeX - 2;
	RatioX[a_DstSizeX - 1] = 1;
	SrcIdxY[a_DstSizeY - 1] = a_SrcSizeY - 2;
	RatioY[a_DstSizeY - 1] = 1;

	// Output all the dst array values using the indices and ratios:
	int idx = 0;
	for (int y = 0; y < a_DstSizeY; y++)
	{
		int idxLoY = a_SrcSizeX * SrcIdxY[y];
		int idxHiY = a_SrcSizeX * (SrcIdxY[y] + 1);
		float ry = RatioY[y];
		for (int x = 0; x < a_DstSizeX; x++)
		{
			// The four src corners of the current "cell":
			float LoXLoY = a_Src[SrcIdxX[x] +     idxLoY];
			float HiXLoY = a_Src[SrcIdxX[x] + 1 + idxLoY];
			float LoXHiY = a_Src[SrcIdxX[x] +     idxHiY];
			float HiXHiY = a_Src[SrcIdxX[x] + 1 + idxHiY];

			// Linear interpolation along the X axis:
			float InterpXLoY = LoXLoY + (HiXLoY - LoXLoY) * RatioX[x];
			float InterpXHiY = LoXHiY + (HiXHiY - LoXHiY) * RatioX[x];

			// Linear interpolation along the Y axis:
			a_Dst[idx] = InterpXLoY + (InterpXHiY - InterpXLoY) * ry;
			idx += 1;
		}
	}
}





void LinearInterpolate3DArray(
	float * a_Src,
	int a_SrcSizeX, int a_SrcSizeY, int a_SrcSizeZ,
	float * a_Dst,
	int a_DstSizeX, int a_DstSizeY, int a_DstSizeZ
)
{
	ASSERT(a_DstSizeX > 0);
	ASSERT(a_DstSizeX < MAX_INTERPOL_SIZEX);
	ASSERT(a_DstSizeY > 0);
	ASSERT(a_DstSizeY < MAX_INTERPOL_SIZEY);
	ASSERT(a_DstSizeZ > 0);
	ASSERT(a_DstSizeZ < MAX_INTERPOL_SIZEZ);

	// Calculate interpolation ratios and src indices along each axis:
	float RatioX[MAX_INTERPOL_SIZEX];
	float RatioY[MAX_INTERPOL_SIZEY];
	float RatioZ[MAX_INTERPOL_SIZEZ];
	int   SrcIdxX[MAX_INTERPOL_SIZEX];
	int   SrcIdxY[MAX_INTERPOL_SIZEY];
	int   SrcIdxZ[MAX_INTERPOL_SIZEZ];
	for (int x = 1; x < a_DstSizeX; x++)
	{
		SrcIdxX[x] = x * (a_SrcSizeX - 1) / (a_DstSizeX - 1);
		RatioX[x] = (static_cast<float>(x * (a_SrcSizeX - 1)) / (a_DstSizeX - 1)) - SrcIdxX[x];
	}
	for (int y = 1; y < a_DstSizeY; y++)
	{
		SrcIdxY[y] = y * (a_SrcSizeY - 1) / (a_DstSizeY - 1);
		RatioY[y] = (static_cast<float>(y * (a_SrcSizeY - 1)) / (a_DstSizeY - 1)) - SrcIdxY[y];
	}
	for (int z = 1; z < a_DstSizeZ; z++)
	{
		SrcIdxZ[z] = z * (a_SrcSizeZ - 1) / (a_DstSizeZ - 1);
		RatioZ[z] = (static_cast<float>(z * (a_SrcSizeZ - 1)) / (a_DstSizeZ - 1)) - SrcIdxZ[z];
	}

	// Special values at the ends. Notice especially the last indices being (size - 2) with ratio set to 1, to avoid index overflow:
	SrcIdxX[0] = 0;
	RatioX[0]  = 0;
	SrcIdxY[0] = 0;
	RatioY[0]  = 0;
	SrcIdxZ[0] = 0;
	RatioZ[0]  = 0;
	SrcIdxX[a_DstSizeX - 1] = a_SrcSizeX - 2;
	RatioX[a_DstSizeX - 1]  = 1;
	SrcIdxY[a_DstSizeY - 1] = a_SrcSizeY - 2;
	RatioY[a_DstSizeY - 1]  = 1;
	SrcIdxZ[a_DstSizeZ - 1] = a_SrcSizeZ - 2;
	RatioZ[a_DstSizeZ - 1]  = 1;

	// Output all the dst array values using the indices and ratios:
	int idx = 0;
	for (int z = 0; z < a_DstSizeZ; z++)
	{
		int idxLoZ = a_SrcSizeX * a_SrcSizeY * SrcIdxZ[z];
		int idxHiZ = a_SrcSizeX * a_SrcSizeY * (SrcIdxZ[z] + 1);
		float rz = RatioZ[z];
		for (int y = 0; y < a_DstSizeY; y++)
		{
			int idxLoY = a_SrcSizeX * SrcIdxY[y];
			int idxHiY = a_SrcSizeX * (SrcIdxY[y] + 1);
			float ry = RatioY[y];
			for (int x = 0; x < a_DstSizeX; x++)
			{
				// The eight src corners of the current "cell":
				float LoXLoYLoZ = a_Src[SrcIdxX[x] +     idxLoY + idxLoZ];
				float HiXLoYLoZ = a_Src[SrcIdxX[x] + 1 + idxLoY + idxLoZ];
				float LoXHiYLoZ = a_Src[SrcIdxX[x] +     idxHiY + idxLoZ];
				float HiXHiYLoZ = a_Src[SrcIdxX[x] + 1 + idxHiY + idxLoZ];
				float LoXLoYHiZ = a_Src[SrcIdxX[x] +     idxLoY + idxHiZ];
				float HiXLoYHiZ = a_Src[SrcIdxX[x] + 1 + idxLoY + idxHiZ];
				float LoXHiYHiZ = a_Src[SrcIdxX[x] +     idxHiY + idxHiZ];
				float HiXHiYHiZ = a_Src[SrcIdxX[x] + 1 + idxHiY + idxHiZ];

				// Linear interpolation along the Z axis:
				float LoXLoYInZ = LoXLoYLoZ + (LoXLoYHiZ - LoXLoYLoZ) * rz;
				float HiXLoYInZ = HiXLoYLoZ + (HiXLoYHiZ - HiXLoYLoZ) * rz;
				float LoXHiYInZ = LoXHiYLoZ + (LoXHiYHiZ - LoXHiYLoZ) * rz;
				float HiXHiYInZ = HiXHiYLoZ + (HiXHiYHiZ - HiXHiYLoZ) * rz;

				// Linear interpolation along the Y axis:
				float LoXInYInZ = LoXLoYInZ + (LoXHiYInZ - LoXLoYInZ) * ry;
				float HiXInYInZ = HiXLoYInZ + (HiXHiYInZ - HiXLoYInZ) * ry;

				// Linear interpolation along the X axis:
				a_Dst[idx] = LoXInYInZ + (HiXInYInZ - LoXInYInZ) * RatioX[x];
				idx += 1;
			}  // for x
		}  // for y
	}  // for z
}





