
// BlockPlant.h

// Base class for any growing block





#pragma once

#include "BlockHandler.h"

class cBlockPlant : public cBlockHandler
{
	typedef cBlockHandler Super;
	bool m_NeedLightToGrow;
public:
	cBlockPlant(BLOCKTYPE a_BlockType, bool a_LightToGrow)
		: Super(a_BlockType), m_NeedLightToGrow(a_LightToGrow){}

protected:
	enum PlantAction
	{
		paDeath,
		paGrowth,
		paStay
	};

	/** Checks whether there is enough light for the plant to grow.
	If the plant doesn't require light to grow, then it returns paGrowth.
	If the plant requires light to grow and there is enough light, it returns paGrowth.
	If the plant requires light to grow and there isn't enough light, it returns paStay.
	If the plant requires light to grow and there is too little light, it returns paDeath.
	*/
	PlantAction HasEnoughLight(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
	{
		// If the plant requires light to grow, check to see if there is enough light
		// Otherwise, return true
		if (m_NeedLightToGrow)
		{
			NIBBLETYPE Blocklight = a_Chunk.GetBlockLight(a_RelX, a_RelY, a_RelZ);
			NIBBLETYPE SkyLight = a_Chunk.GetSkyLight  (a_RelX, a_RelY, a_RelZ);
			NIBBLETYPE Light = a_Chunk.GetTimeAlteredLight(SkyLight);

			// If the amount of light provided by blocks is greater than the sky light, use it instead
			if (Blocklight > Light)
			{
				Light = Blocklight;
			}

			// Based on light levels, decide between growth, stay and death:
			if (Light > 8)
			{
				return paGrowth;
			}
			else if ((Blocklight < 9) && (SkyLight < 9))
			{
				return paDeath;
			}

			return paStay;
		}

		return paGrowth;
	}

	/** Checks whether a plant can grow grow, based on what is returned from cBlockPlant::HasEnoughLight
	and a random check based on what is returned from cBlockPlant::GetGrowthChance.
	Can return three values.
	paGrowth when the conditions are right for the plant to grow.
	paStay when the conditions are not quite right.
	paDeath is returned when there isn't enough light for the plant to survive.
	Plants that don't require light will never have a paDeath returned
	*/
	virtual PlantAction CanGrow(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
	{
		// Plant can grow if it has the required amount of light, and it passes a random chance based on surrounding blocks
		PlantAction Action = HasEnoughLight(a_Chunk, a_RelX, a_RelY, a_RelZ);
		if ((Action == paGrowth) && !GetRandomProvider().RandBool(1.0 / GetGrowthChance(a_Chunk, a_RelX, a_RelY, a_RelZ)))
		{
			Action = paStay;
		}
		return Action;
	}

	/** Generates a int value between 4 and 25 based on surrounding blocks that affect how quickly the plant grows.
	The higher the value, the less likely the plant is to grow */
	virtual int GetGrowthChance(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
	{
		float Chance = 1.0f;
		a_RelY -= 1;
		for (int x = -1; x < 2; ++x)
		{
			for (int z = -1; z < 2; ++z)
			{
				float Adjustment = 0.0f;
				BLOCKTYPE Block;
				NIBBLETYPE Meta;

				// If the chunk we are trying to get the block information from is loaded
				if (a_Chunk.UnboundedRelGetBlock(a_RelX + x, a_RelY, a_RelZ + z, Block, Meta))
				{
					cBlockHandler * Handler = BlockHandler(Block);

					// If the block affects growth, add to the adjustment
					if (Handler->CanSustainPlant(m_BlockType))
					{
						Adjustment = 1.0f;

						// Farmland  alters the chance further if it is watered
						if ((Block == E_BLOCK_FARMLAND) && (Meta != 0))
						{
							Adjustment = 3.0f;
						}
					}
				}

				// If this is not the block right underneath the plant, it has little effect on the growth
				if ((x != 0) || (z != 0))
				{
					Adjustment /= 4.0f;
				}

				Chance += Adjustment;
			}
		}
		return FloorC(24.0f / Chance) + 1;
	}
};
