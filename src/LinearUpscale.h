
// LinearUpscale.h

// Declares the functions for linearly upscaling arrays

/*
Upscaling means that the array is divided into same-size "cells", and each cell is
linearly interpolated between its corners. The array's dimensions are therefore
1 + CellSize * NumCells, for each direction.

Upscaling is more efficient than linear interpolation, because the cell sizes are integral
and therefore the cells' boundaries are on the array points.

However, upscaling usually requires generating the "1 +" in each direction.

Upscaling is implemented in templates, so that it's compatible with multiple datatypes.
Therefore, there is no cpp file.

InPlace upscaling works on a single array and assumes that the values to work on have already
been interspersed into the array to the cell boundaries.
Specifically, a_Array[x * AnchorStepX + y * AnchorStepY] contains the anchor value.

Regular upscaling takes two arrays and "moves" the input from src to dst; src is expected packed.
*/





#pragma once





/**
Linearly interpolates values in the array between the equidistant anchor points (upscales).
Works in-place (input is already present at the correct output coords)
Uses templates to make it possible for the compiler to further optimizer the loops
*/
template <
	int SizeX, int SizeY,  // Dimensions of the array
	int AnchorStepX, int AnchorStepY,
	typename TYPE
>
void LinearUpscale2DArrayInPlace(TYPE * a_Array)
{
	// First interpolate columns where the anchor points are:
	int LastYCell = SizeY - AnchorStepY;
	for (int y = 0; y < LastYCell; y += AnchorStepY)
	{
		int Idx = SizeX * y;
		for (int x = 0; x < SizeX; x += AnchorStepX)
		{
			TYPE StartValue = a_Array[Idx];
			TYPE EndValue   = a_Array[Idx + SizeX * AnchorStepY];
			TYPE Diff = EndValue - StartValue;
			for (int CellY = 1; CellY < AnchorStepY; CellY++)
			{
				a_Array[Idx + SizeX * CellY] = StartValue + Diff * CellY / AnchorStepY;
			}  // for CellY
			Idx += AnchorStepX;
		}  // for x
	}  // for y

	// Now interpolate in rows, each row has values in the anchor columns
	int LastXCell = SizeX - AnchorStepX;
	for (int y = 0; y < SizeY; y++)
	{
		int Idx = SizeX * y;
		for (int x = 0; x < LastXCell; x += AnchorStepX)
		{
			TYPE StartValue = a_Array[Idx];
			TYPE EndValue   = a_Array[Idx + AnchorStepX];
			TYPE Diff = EndValue - StartValue;
			for (int CellX = 1; CellX < AnchorStepX; CellX++)
			{
				a_Array[Idx + CellX] = StartValue + CellX * Diff / AnchorStepX;
			}  // for CellY
			Idx += AnchorStepX;
		}
	}
}





/**
Linearly interpolates values in the array between the equidistant anchor points (upscales).
Works on two arrays, input is packed and output is to be completely constructed.
*/
template <typename TYPE> void LinearUpscale2DArray(
	TYPE * a_Src,                    ///< Source array of size a_SrcSizeX x a_SrcSizeY
	int a_SrcSizeX, int a_SrcSizeY,  ///< Dimensions of the src array
	TYPE * a_Dst,                    ///< Dest array, of size (a_SrcSizeX * a_UpscaleX + 1) x (a_SrcSizeY * a_UpscaleY + 1)
	int a_UpscaleX, int a_UpscaleY   ///< Upscale factor for each direction
)
{
	// For optimization reasons, we're storing the upscaling ratios in a fixed-size arrays of these sizes
	// Feel free to enlarge them if needed, but keep in mind that they're on the stack
	const int MAX_UPSCALE_X = 129;
	const int MAX_UPSCALE_Y = 129;

	ASSERT(a_Src != nullptr);
	ASSERT(a_Dst != nullptr);
	ASSERT(a_SrcSizeX > 0);
	ASSERT(a_SrcSizeY > 0);
	ASSERT(a_UpscaleX > 0);
	ASSERT(a_UpscaleY > 0);
	ASSERT(a_UpscaleX < MAX_UPSCALE_X);
	ASSERT(a_UpscaleY < MAX_UPSCALE_Y);

	// Pre-calculate the upscaling ratios:
	TYPE RatioX[MAX_UPSCALE_X];
	TYPE RatioY[MAX_UPSCALE_Y];
	for (int x = 0; x <= a_UpscaleX; x++)
	{
		RatioX[x] = static_cast<TYPE>(x) / a_UpscaleX;
	}
	for (int y = 0; y <= a_UpscaleY; y++)
	{
		RatioY[y] = static_cast<TYPE>(y) / a_UpscaleY;
	}

	// Interpolate each XY cell:
	int DstSizeX = (a_SrcSizeX - 1) * a_UpscaleX + 1;
	#ifdef _DEBUG
		int DstSizeY = (a_SrcSizeY - 1) * a_UpscaleY + 1;
	#endif
	for (int y = 0; y < (a_SrcSizeY - 1); y++)
	{
		int DstY = y * a_UpscaleY;
		int idx = y * a_SrcSizeX;
		for (int x = 0; x < (a_SrcSizeX - 1); x++, idx++)
		{
			int DstX = x * a_UpscaleX;
			TYPE LoXLoY = a_Src[idx];
			TYPE LoXHiY = a_Src[idx + a_SrcSizeX];
			TYPE HiXLoY = a_Src[idx + 1];
			TYPE HiXHiY = a_Src[idx + 1 + a_SrcSizeX];
			for (int CellY = 0; CellY <= a_UpscaleY; CellY++)
			{
				int DestIdx = (DstY + CellY) * DstSizeX + DstX;
				ASSERT(DestIdx + a_UpscaleX < DstSizeX * DstSizeY);
				TYPE LoXInY = LoXLoY + (LoXHiY - LoXLoY) * RatioY[CellY];
				TYPE HiXInY = HiXLoY + (HiXHiY - HiXLoY) * RatioY[CellY];
				for (int CellX = 0; CellX <= a_UpscaleX; CellX++, DestIdx++)
				{
					a_Dst[DestIdx] = LoXInY + (HiXInY - LoXInY) * RatioX[CellX];
				}
			}  // for CellY
		}  // for x
	}  // for y
}





/**
Linearly interpolates values in the array between the equidistant anchor points (upscales).
Works on two arrays, input is packed and output is to be completely constructed.
*/
template <typename TYPE> void LinearUpscale3DArray(
	TYPE * a_Src,                                    ///< Source array of size a_SrcSizeX x a_SrcSizeY x a_SrcSizeZ
	int a_SrcSizeX, int a_SrcSizeY, int a_SrcSizeZ,  ///< Dimensions of the src array
	TYPE * a_Dst,                                    ///< Dest array, of size (a_SrcSizeX * a_UpscaleX + 1) x (a_SrcSizeY * a_UpscaleY + 1) x (a_SrcSizeZ * a_UpscaleZ + 1)
	int a_UpscaleX, int a_UpscaleY, int a_UpscaleZ   ///< Upscale factor for each direction
)
{
	// For optimization reasons, we're storing the upscaling ratios in a fixed-size arrays of these sizes
	// Feel free to enlarge them if needed, but keep in mind that they're on the stack
	const int MAX_UPSCALE_X = 128;
	const int MAX_UPSCALE_Y = 128;
	const int MAX_UPSCALE_Z = 128;

	ASSERT(a_Src != nullptr);
	ASSERT(a_Dst != nullptr);
	ASSERT(a_SrcSizeX > 0);
	ASSERT(a_SrcSizeY > 0);
	ASSERT(a_SrcSizeZ > 0);
	ASSERT(a_UpscaleX > 0);
	ASSERT(a_UpscaleY > 0);
	ASSERT(a_UpscaleZ > 0);
	ASSERT(a_UpscaleX <= MAX_UPSCALE_X);
	ASSERT(a_UpscaleY <= MAX_UPSCALE_Y);
	ASSERT(a_UpscaleZ <= MAX_UPSCALE_Z);

	// Pre-calculate the upscaling ratios:
	TYPE RatioX[MAX_UPSCALE_X];
	TYPE RatioY[MAX_UPSCALE_Y];
	TYPE RatioZ[MAX_UPSCALE_Z];
	for (int x = 0; x <= a_UpscaleX; x++)
	{
		RatioX[x] = static_cast<TYPE>(x) / a_UpscaleX;
	}
	for (int y = 0; y <= a_UpscaleY; y++)
	{
		RatioY[y] = static_cast<TYPE>(y) / a_UpscaleY;
	}
	for (int z = 0; z <= a_UpscaleZ; z++)
	{
		RatioZ[z] = static_cast<TYPE>(z) / a_UpscaleZ;
	}

	// Interpolate each XYZ cell:
	int DstSizeX = (a_SrcSizeX - 1) * a_UpscaleX + 1;
	int DstSizeY = (a_SrcSizeY - 1) * a_UpscaleY + 1;
	#ifdef _DEBUG
		int DstSizeZ = (a_SrcSizeZ - 1) * a_UpscaleZ + 1;
	#endif
	for (int z = 0; z < (a_SrcSizeZ - 1); z++)
	{
		int DstZ = z * a_UpscaleZ;
		for (int y = 0; y < (a_SrcSizeY - 1); y++)
		{
			int DstY = y * a_UpscaleY;
			int idx = y * a_SrcSizeX + z * a_SrcSizeX * a_SrcSizeY;
			for (int x = 0; x < (a_SrcSizeX - 1); x++, idx++)
			{
				int DstX = x * a_UpscaleX;
				TYPE LoXLoYLoZ = a_Src[idx];
				TYPE LoXLoYHiZ = a_Src[idx + a_SrcSizeX * a_SrcSizeY];
				TYPE LoXHiYLoZ = a_Src[idx + a_SrcSizeX];
				TYPE LoXHiYHiZ = a_Src[idx + a_SrcSizeX + a_SrcSizeX * a_SrcSizeY];
				TYPE HiXLoYLoZ = a_Src[idx + 1];
				TYPE HiXLoYHiZ = a_Src[idx + 1 + a_SrcSizeX * a_SrcSizeY];
				TYPE HiXHiYLoZ = a_Src[idx + 1 + a_SrcSizeX];
				TYPE HiXHiYHiZ = a_Src[idx + 1 + a_SrcSizeX + a_SrcSizeX * a_SrcSizeY];
				for (int CellZ = 0; CellZ <= a_UpscaleZ; CellZ++)
				{
					TYPE LoXLoYInZ = LoXLoYLoZ + (LoXLoYHiZ - LoXLoYLoZ) * RatioZ[CellZ];
					TYPE LoXHiYInZ = LoXHiYLoZ + (LoXHiYHiZ - LoXHiYLoZ) * RatioZ[CellZ];
					TYPE HiXLoYInZ = HiXLoYLoZ + (HiXLoYHiZ - HiXLoYLoZ) * RatioZ[CellZ];
					TYPE HiXHiYInZ = HiXHiYLoZ + (HiXHiYHiZ - HiXHiYLoZ) * RatioZ[CellZ];
					for (int CellY = 0; CellY <= a_UpscaleY; CellY++)
					{
						int DestIdx = (DstZ + CellZ) * DstSizeX * DstSizeY + (DstY + CellY) * DstSizeX + DstX;
						ASSERT(DestIdx + a_UpscaleX < DstSizeX * DstSizeY * DstSizeZ);
						TYPE LoXInY = LoXLoYInZ + (LoXHiYInZ - LoXLoYInZ) * RatioY[CellY];
						TYPE HiXInY = HiXLoYInZ + (HiXHiYInZ - HiXLoYInZ) * RatioY[CellY];
						for (int CellX = 0; CellX <= a_UpscaleX; CellX++, DestIdx++)
						{
							a_Dst[DestIdx] = LoXInY + (HiXInY - LoXInY) * RatioX[CellX];
						}
					}  // for CellY
				}  // for CellZ
			}  // for x
		}  // for y
	}  // for z
}





