
#ifndef __C_NOISE_INC__
#define __C_NOISE_INC__

#include <math.h>





/****************
 * Random value generator
 **/
 
float cNoise::IntNoise( int a_X ) const
{
	int x = ((a_X*m_Seed)<<13) ^ a_X;
	return ( 1.0f - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f); 
	// returns a float number in the range of [-1, 1]
}





float cNoise::IntNoise2D( int a_X, int a_Y ) const
{
	int n = a_X + a_Y * 57 + m_Seed*57*57;
	n = (n<<13) ^ n;
	return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);  
	// returns a float number in the range of [-1, 1]
}





float cNoise::IntNoise3D( int a_X, int a_Y, int a_Z ) const
{
	int n = a_X + a_Y * 57 + a_Z * 57*57 + m_Seed*57*57*57;
	n = (n<<13) ^ n;
	return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);  
	// returns a float number in the range of [-1, 1]
}





int cNoise::IntNoise1DInt( int a_X ) const
{
	int x = ((a_X*m_Seed)<<13) ^ a_X;
	return ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff);
}





int cNoise::IntNoise2DInt( int a_X, int a_Y ) const
{
	int n = a_X + a_Y * 57 + m_Seed*57*57;
	n = (n<<13) ^ n;
	return ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
}





int cNoise::IntNoise3DInt( int a_X, int a_Y, int a_Z ) const
{
	int n = a_X + a_Y * 57 + a_Z * 57*57 + m_Seed*57*57*57;
	n = (n<<13) ^ n;
	return ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
}





/****************
 * Interpolation functions
 **/
 
float cNoise::CubicInterpolate( float a_A, float a_B, float a_C, float a_D, float a_Pct ) const
{
	float P = (a_D - a_C) - (a_A - a_B);
	float Q = (a_A - a_B) - P;
	float R = a_C - a_A;
	float S = a_B;

	return ((P * a_Pct + Q) * a_Pct + R) * a_Pct + S;
}





float cNoise::CosineInterpolate( float a_A, float a_B, float a_Pct ) const
{
	const float ft = a_Pct * 3.1415927f;
	const float f = (1.f - cosf(ft)) * 0.5f;
	return  a_A*(1-f) + a_B*f;
}





float cNoise::LinearInterpolate( float a_A, float a_B, float a_Pct ) const
{
	return  a_A*(1.f-a_Pct) + a_B*a_Pct;
}





#endif




