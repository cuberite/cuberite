
#include "Globals.h"

#include "Simulator.h"
#include "../Chunk.h"
#include "../World.h"





std::array<Vector3i, 5> cSimulator::GetLinkedOffsets(const Vector3i Offset)
{
	if (Offset.x == -1)
	{
		return
		{
			{
				{ -2,  0,  0 },
				{ -1, -1,  0 },
				{ -1,  1,  0 },
				{ -1,  0, -1 },
				{ -1,  0,  1 }
			}
		};
	}
	else if (Offset.x == 1)
	{
		return
		{
			{
				{  2,  0,  0 },
				{  1, -1,  0 },
				{  1,  1,  0 },
				{  1,  0, -1 },
				{  1,  0,  1 }
			}
		};
	}
	else if (Offset.y == -1)
	{
		return
		{
			{
				{  0, -2,  0 },
				{ -1, -1,  0 },
				{  1, -1,  0 },
				{  0, -1, -1 },
				{  0, -1,  1 }
			}
		};
	}
	else if (Offset.y == 1)
	{
		return
		{
			{
				{  0,  2,  0 },
				{ -1,  1,  0 },
				{  1,  1,  0 },
				{  0,  1, -1 },
				{  0,  1,  1 }
			}
		};
	}
	else if (Offset.z == -1)
	{
		return
		{
			{
				{  0,  0, -2 },
				{ -1,  0, -1 },
				{  1,  0, -1 },
				{  0, -1, -1 },
				{  0,  1, -1 }
			}
		};
	}

	return
	{
		{
			{  0,  0,  2 },
			{ -1,  0,  1 },
			{  1,  0,  1 },
			{  0, -1,  1 },
			{  0,  1,  1 }
		}
	};
}





void cSimulator::Simulate(float a_Dt)
{
	UNUSED(a_Dt);
}





void cSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
{
	ASSERT(a_Chunk.IsValid());

	AddBlock(a_Chunk, a_Position, a_Block);
}





void cSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, Vector3i a_Offset, BlockState a_Block)
{
	ASSERT(a_Chunk.IsValid());

	WakeUp(a_Chunk, a_Position, a_Block);
}
