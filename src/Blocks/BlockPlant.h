#pragma once


#include "BlockHandler.h"




/** Base class for plants that use light values to decide whether to grow or not.
On block update, the plant decides whether to grow, die or stay as-is, based on the CanGrow() overridable method result. */
template <bool NeedsLightToGrow>
class cBlockPlant :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

protected:

	~cBlockPlant() = default;

	/** The action the plant can take on an update. */
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
	If the plant requires light to grow and there is too little light, it returns paDeath. */
	static PlantAction HasEnoughLight(cChunk & a_Chunk, Vector3i a_RelPos)
	{
		// If the plant requires light to grow, check to see if there is enough light
		// Otherwise, return true
		if (!NeedsLightToGrow)
		{
			return paGrowth;
		}
		NIBBLETYPE Blocklight = a_Chunk.GetBlockLight(a_RelPos);
		NIBBLETYPE SkyLight   = a_Chunk.GetSkyLight  (a_RelPos);
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





	/** Checks whether a plant can grow grow, based on what is returned from cBlockPlant::HasEnoughLight
	and a random check based on what is returned from cBlockPlant::GetGrowthChance.
	Can return three values.
	paGrowth when the conditions are right for the plant to grow.
	paStay when the conditions are not quite right.
	paDeath is returned when there isn't enough light for the plant to survive.
	Plants that don't require light will never have a paDeath returned
	*/
	virtual PlantAction CanGrow(cChunk & a_Chunk, Vector3i a_RelPos) const
	{
		// Plant can grow if it has the required amount of light, and it passes a random chance based on surrounding blocks
		auto action = HasEnoughLight(a_Chunk, a_RelPos);
		if ((action == paGrowth) && !GetRandomProvider().RandBool(1.0 / GetGrowthChance(a_Chunk, a_RelPos)))
		{
			action = paStay;
		}
		return action;
	}





	/** Generates an int value between 4 and 25 based on surrounding blocks that affect how quickly the plant grows.
	The higher the value, the less likely the plant is to grow */
	virtual int GetGrowthChance(cChunk & a_Chunk, Vector3i a_RelPos) const
	{
		float Chance = 1.0f;
		a_RelPos.y -= 1;
		for (int x = -1; x < 2; ++x)
		{
			for (int z = -1; z < 2; ++z)
			{
				float Adjustment = 0.0f;
				BLOCKTYPE Block;
				NIBBLETYPE Meta;

				// If the chunk we are trying to get the block information from is loaded
				if (a_Chunk.UnboundedRelGetBlock(a_RelPos + Vector3i(x, 0, z), Block, Meta))
				{
					// If the block affects growth, add to the adjustment
					if (cBlockHandler::For(Block).CanSustainPlant(m_BlockType))
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

private:

	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		auto Action = CanGrow(a_Chunk, a_RelPos);
		switch (Action)
		{
			case paGrowth:
			{
				Grow(a_Chunk, a_RelPos);
				break;
			}
			case paDeath:
			{
				a_ChunkInterface.SetBlock(a_Chunk.RelativeToAbsolute(a_RelPos), E_BLOCK_AIR, 0);
				break;
			}
			case paStay: break;  // do nothing
		}
	}
};
