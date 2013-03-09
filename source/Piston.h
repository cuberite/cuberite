
#pragma once





// fwd: World.h
class cWorld;





class cPiston
{
public:

	cPiston(cWorld * a_World);

	static NIBBLETYPE RotationPitchToMetaData(double a_Rotation, double a_Pitch)
	{
		if (a_Pitch >= 50)
		{
			return 0x1;
		}
		else if (a_Pitch <= -50)
		{
			return 0x0;
		}
		else
		{
			a_Rotation += 90 + 45;  // So its not aligned with axis

			if (a_Rotation > 360)
			{
				a_Rotation -= 360;
			}
			if ((a_Rotation >= 0) && (a_Rotation < 90))
			{
				return 0x4;
			}
			else if ((a_Rotation >= 180) && (a_Rotation < 270))
			{
				return 0x5;
			}
			else if ((a_Rotation >= 90) && (a_Rotation < 180))
			{
				return 0x2;
			}
			else
			{
				return 0x3;
			}
		}
	}

	void ExtendPiston( int, int, int );
	void RetractPiston( int, int, int );

	cWorld * m_World;

private:
	void ChainMove( int, int, int, char, unsigned short * );
	unsigned short FirstPassthroughBlock( int, int, int, char );
} ;




