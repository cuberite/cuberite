
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
	
	/// Returns true if the piston (specified by blocktype) is a sticky piston
	static bool IsSticky(BLOCKTYPE a_BlockType);
	
	/// Returns true if the piston (with the specified meta) is extended
	static bool IsExtended(NIBBLETYPE a_PistonMeta);
	
	/// Returns true if the specified block can be pushed by a piston (and left intact)
	static bool CanPush(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/// Returns true if the specified block can be pushed by a piston and broken / replaced
	static bool CanBreakPush(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/// Returns true if the specified block can be pulled by a sticky piston
	static bool CanPull(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/// Updates the coords by the specified amount in the direction a piston of the specified meta is facing
	static void AddDir(int & a_BlockX, int & a_BlockY, int & a_BlockZ, NIBBLETYPE a_PistonMeta, int a_Amount);
	
	
	cWorld * m_World;

private:
	void ChainMove( int, int, int, char, unsigned short * );
	
	/// Returns how many blocks the piston has to push (where the first free space is); <0 when unpushable
	int FirstPassthroughBlock(int a_PistonX, int a_PistonY, int a_PistonZ, NIBBLETYPE a_PistonMeta);
} ;




