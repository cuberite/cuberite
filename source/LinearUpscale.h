
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
Specifically, a_Array[x * a_AnchorStepX + y * a_AnchorStepY] contains the anchor value.

Regular upscaling takes two arrays and "moves" the input from src to dst; src is expected packed.
*/




/// Linearly interpolates values in the array between the equidistant anchor points; universal data type
template<typename TYPE> void ArrayLinearUpscale2DInPlace(
	TYPE * a_Array, 
	int a_SizeX, int a_SizeY,  // Dimensions of the array
	int a_AnchorStepX, int a_AnchorStepY  // Distances between the anchor points in each direction
)
{
	// First interpolate columns where the anchor points are:
	int LastYCell = a_SizeY - a_AnchorStepY;
	for (int y = 0; y < LastYCell; y += a_AnchorStepY)
	{
		int Idx = a_SizeX * y;
		for (int x = 0; x < a_SizeX; x += a_AnchorStepX)
		{
			TYPE StartValue = a_Array[Idx];
			TYPE EndValue   = a_Array[Idx + a_SizeX * a_AnchorStepY];
			TYPE Diff = EndValue - StartValue;
			for (int CellY = 1; CellY < a_AnchorStepY; CellY++)
			{
				a_Array[Idx + a_SizeX * CellY] = StartValue + Diff * CellY / a_AnchorStepY;
			}  // for CellY
			Idx += a_AnchorStepX;
		}  // for x
	}  // for y
	
	// Now interpolate in rows, each row has values in the anchor columns
	int LastXCell = a_SizeX - a_AnchorStepX;
	for (int y = 0; y < a_SizeY; y++)
	{
		int Idx = a_SizeX * y;
		for (int x = 0; x < LastXCell; x += a_AnchorStepX)
		{
			TYPE StartValue = a_Array[Idx];
			TYPE EndValue   = a_Array[Idx + a_AnchorStepX];
			TYPE Diff = EndValue - StartValue;
			for (int CellX = 1; CellX < a_AnchorStepX; CellX++)
			{
				a_Array[Idx + CellX] = StartValue + CellX * Diff / a_AnchorStepX;
			}  // for CellY
			Idx += a_AnchorStepX;
		}
	}
}





