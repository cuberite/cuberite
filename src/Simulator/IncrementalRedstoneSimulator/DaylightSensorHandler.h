
#pragma Once
#include <cmath>





namespace DaylightSensorHandler
{
	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(IsLinked);

    float PI = 3.14159;
    int TimeOfDay = a_Chunk.GetWorld()->GetTimeOfDay();
    bool IsInverted = (a_Chunk.GetBlock(a_Position) == E_BLOCK_DAYLIGHT_SENSOR) ? 1 : 0;
    int Output = Clamp(FloorC(8 * std::sin((TimeOfDay * PI) / 12000) + 8), 0, 15);

    if(IsInverted)
    {
      Output = 15 - Output;
    }

    return static_cast<int>(Output);
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
    
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(Callback);
	}
};
