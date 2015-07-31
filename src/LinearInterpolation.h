
// LinearInterpolation.h

// Declares methods for linear interpolation over 1D, 2D and 3D arrays





#pragma once





// 2D and 3D Interpolation is optimized by precalculating the ratios into static-sized arrays
// These arrays enforce a max size of the dest array, but the limits are settable here:
const int MAX_INTERPOL_SIZEX = 256;  ///< Maximum X-size of the interpolated array
const int MAX_INTERPOL_SIZEY = 512;  ///< Maximum Y-size of the interpolated array
const int MAX_INTERPOL_SIZEZ = 256;  ///< Maximum Z-size of the interpolated array





/** Puts linearly interpolated values from one array into another array. 1D version */
void LinearInterpolate1DArray(
	float * a_Src,   ///< Src array
	int a_SrcSizeX,  ///< Count of the src array
	float * a_Dst,   ///< Src array
	int a_DstSizeX   ///< Count of the dst array
);





/** Puts linearly interpolated values from one array into another array. 2D version */
void LinearInterpolate2DArray(
	float * a_Src,                   ///< Src array, [x + a_SrcSizeX * y]
	int a_SrcSizeX, int a_SrcSizeY,  ///< Count of the src array, in each direction
	float * a_Dst,                   ///< Dst array, [x + a_DstSizeX * y]
	int a_DstSizeX, int a_DstSizeY   ///< Count of the dst array, in each direction
);





/** Puts linearly interpolated values from one array into another array. 3D version */
void LinearInterpolate3DArray(
	float * a_Src,                                   ///< Src array, [x + a_SrcSizeX * y + a_SrcSizeX * a_SrcSizeY * z]
	int a_SrcSizeX, int a_SrcSizeY, int a_SrcSizeZ,  ///< Count of the src array, in each direction
	float * a_Dst,                                   ///< Dst array, [x + a_DstSizeX * y + a_DstSizeX * a_DstSizeY * z]
	int a_DstSizeX, int a_DstSizeY, int a_DstSizeZ   ///< Count of the dst array, in each direction
);




