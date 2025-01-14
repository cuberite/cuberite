
// PieceModifier.cpp

// Implements the various classes descending from cPiece::cPieceModifier

#include "Globals.h"
#include "PieceModifier.h"
#include "../Noise/Noise.h"
// #include "../Protocol/Palettes/Upgrade.h"
#include "BlockInfo.h"




// Constant that is added to seed
static const int SEED_OFFSET = 135 * 13;



////////////////////////////////////////////////////////////////////////////////
/** A modifier which is pseudo-randomly replacing blocks to other types and metas. */
class cPieceModifierRandomizeBlocks:
	public cPiece::cPieceModifier
{
public:
	cPieceModifierRandomizeBlocks(void) :
		m_Seed()
	{
	}

	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		m_AllWeights = 0;
		AString Params = a_Params;


		/** BlocksToReplace parsing */
		auto idxPipe = Params.find('|');
		if (idxPipe != AString::npos)
		{
			AString blocksToReplaceStr = Params.substr(0, idxPipe);
			auto blocksToReplace = StringSplitAndTrim(blocksToReplaceStr, ",");
			for (size_t i = 0; i < blocksToReplace.size(); i++)
			{
				auto Block = PaletteUpgrade::FromBlock(static_cast<unsigned char>(BlockStringToType(blocksToReplace[i])), 0);
				if (IsBlockAir(Block) && !NoCaseCompare(blocksToReplace[i], "Air"))
				{
					CONDWARNING(a_LogWarnings, "Cannot parse block type from string \"%s\"!", blocksToReplace[i].c_str());
					return false;
				}
				m_BlocksToReplace[Block.Type()] = 1;
			}

			Params = Params.substr(idxPipe + 1, Params.length() - 1);
		}
		if (m_BlocksToReplace.size() == 0)
		{
			CONDWARNING(a_LogWarnings, "You must specify at least one block to replace%s!", "");
			return false;
		}


		/** Meta params parsing */
		auto idxSqBracketStart = Params.find('[');
		auto idxSqBracketStartLast = Params.find_last_of('[');

		bool isMultiMeta = false;
		if ((idxSqBracketStart != idxSqBracketStartLast) && (idxSqBracketStartLast != Params.length() - 1))
		{
			// Meta per block type
			isMultiMeta = true;
		}
		else
		{
			auto idxSqBracketEnd = Params.find(']');
			if ((idxSqBracketEnd == Params.length() - 1) && (idxSqBracketStart != AString::npos))
			{
				AString metaParamsStr = Params.substr(idxSqBracketStart + 1, Params.length() - idxSqBracketStart - 2);
				std::array<int, 4> metaParamsInt;
				if (!ParseMeta(metaParamsStr, metaParamsInt, a_LogWarnings))
				{
					return false;
				}

				m_MinMeta = metaParamsInt[0];
				m_MaxMeta = metaParamsInt[1];
				m_MinNoiseMeta = metaParamsInt[2];
				m_MaxNoiseMeta = metaParamsInt[3];

				Params = Params.substr(0, idxSqBracketStart);
			}
		}


		// BlocksToRandomize parsing
		auto BlocksToRandomize = StringSplitAndTrim(Params, ";");
		for (size_t i = 0; i < BlocksToRandomize.size(); i++)
		{
			AString block = BlocksToRandomize[i];

			cRandomizedBlock Block{};

			if (isMultiMeta)
			{
				auto sqBrStart = block.find('[');
				if (sqBrStart != AString::npos)
				{
					auto sqBrEnd = block.find(']');
					if (sqBrEnd != block.size() - 1)
					{
						CONDWARNING(a_LogWarnings, "If present, block meta params must be at the end of block to randomize definition \"%s\"!", block.c_str());
						return false;

					}
					AString metaParamsStr = block.substr(sqBrStart + 1, block.size() - sqBrStart - 2);

					std::array<int, 4> metaParamsInt;
					if (!ParseMeta(metaParamsStr, metaParamsInt, a_LogWarnings))
					{
						return false;
					}

					Block.m_MinMeta = metaParamsInt[0];
					Block.m_MaxMeta = metaParamsInt[1];
					Block.m_MinNoiseMeta = metaParamsInt[2];
					Block.m_MaxNoiseMeta = metaParamsInt[3];

					block = block.substr(0, sqBrStart);

				}
				// No meta randomization for this block
			}

			auto BlockParams = StringSplitAndTrim(block, ",");
			if (BlockParams.size() < 2)
			{
				CONDWARNING(a_LogWarnings, "Block weight is required param \"%s\"!", BlockParams[0].c_str());
				return false;
			}

			auto SetBlock = PaletteUpgrade::FromBlock(static_cast<unsigned char>(BlockStringToType(BlockParams[0])), 0);
			int BlockWeight = 0;
			if ((SetBlock.Type() != BlockType::Air) && !NoCaseCompare(BlockParams[0], "Air"))
			{
				// Failed to parse block type
				CONDWARNING(
					a_LogWarnings, "Cannot parse block type from string \"%s\"!",
					BlockParams[0].c_str());
				return false;
			}

			if (!StringToInteger(BlockParams[1], BlockWeight))
			{
				// Failed to parse the crop weight:
				CONDWARNING(
					a_LogWarnings,
					"Cannot parse block weight from string \"%s\"!",
					BlockParams[1].c_str());
				return false;
			}


			Block.m_Type = SetBlock.Type();
			Block.m_Weight = BlockWeight;
			m_AllWeights += BlockWeight;

			m_BlocksToRandomize.push_back(Block);
		}
		if (m_BlocksToRandomize.size() == 0)
		{
			CONDWARNING(a_LogWarnings, "You must specify at least one block to randomize%s!", "");
			return false;
		}

		return true;
	}





	bool ParseMeta(const AString & a_Meta, std::array<int, 4> & a_ParsedMeta, bool a_LogWarnings)
	{
		auto MetaParams = StringSplitAndTrim(a_Meta, ",");

		for (size_t i = 0; i < MetaParams.size(); i++)
		{
			int Value;
			if (!StringToInteger(MetaParams[i], Value))
			{
				// Failed to parse meta parameter from string:
				CONDWARNING(a_LogWarnings, "Cannot parse meta param from string \"%s\", meta: %s!", MetaParams[i].c_str(), a_Meta.c_str());
				return false;
			}

			if (i > 3)
			{
				CONDWARNING(a_LogWarnings, "Unsupported meta param \"%d\"!", Value);
				return false;
			}

			a_ParsedMeta[i] = Value;
		}

		if (MetaParams.size() == 1)
		{
			// Noise is not used for meta
			a_ParsedMeta[1] = a_ParsedMeta[0];
		}
		else if (MetaParams.size() == 2)
		{
			// Noise range is same as meta range
			a_ParsedMeta[2] = a_ParsedMeta[0];
			a_ParsedMeta[3] = a_ParsedMeta[1];
		}
		else if (MetaParams.size() == 3)
		{
			a_ParsedMeta[3] = a_ParsedMeta[1];
		}

		return true;
	}





	virtual void Modify(cBlockArea & a_Image, const Vector3i a_PiecePos, const int a_PieceRot) override
	{
		cNoise Noise(m_Seed);
		cNoise PieceNoise(Noise.IntNoise3DInt(a_PiecePos));

		const size_t NumBlocks = a_Image.GetBlockCount();
		auto BlockTypes = a_Image.GetBlocks();
		std::vector<unsigned char> BlockMetas;

		for (size_t i = 0; i < NumBlocks; i++)
		{
			if (m_BlocksToReplace.count(BlockTypes[i].Type()))
			{
				float BlockRnd = PieceNoise.IntNoise2DInRange(a_PieceRot, static_cast<int>(i), 0.0F, static_cast<float>(m_AllWeights));

				int weightDelta = 0;
				for (auto & blockToRnd : m_BlocksToRandomize)
				{
					weightDelta += blockToRnd.m_Weight;
					if (BlockRnd <= weightDelta)
					{
						BlockTypes[i] = blockToRnd.m_Type;

						// Per block meta params
						if (blockToRnd.m_MinMeta < blockToRnd.m_MaxMeta)
						{
							int BlockMetaRnd = std::clamp(static_cast<int>(PieceNoise.IntNoise2DInRange(a_PieceRot*2, static_cast<int>(i), static_cast<float>(blockToRnd.m_MinNoiseMeta), static_cast<float>(blockToRnd.m_MaxNoiseMeta))), blockToRnd.m_MinMeta, blockToRnd.m_MaxMeta);
							BlockMetas[i] = static_cast<unsigned char>(BlockMetaRnd);
						}
						else if ((blockToRnd.m_MaxMeta > -1) && (blockToRnd.m_MaxMeta == blockToRnd.m_MinMeta))
						{
							// Change meta if at least minimum meta was specified
							BlockMetas[i] = static_cast<unsigned char>(blockToRnd.m_MaxMeta);
						}
						break;
					}
				}

				// All blocks meta params
				if (m_MaxMeta > m_MinMeta)
				{
					int BlockMetaRnd = std::clamp(static_cast<int>(PieceNoise.IntNoise2DInRange(a_PieceRot * 2, static_cast<int>(i), static_cast<float>(m_MinNoiseMeta), static_cast<float>(m_MaxNoiseMeta))), m_MinMeta, m_MaxMeta);
					BlockMetas[i] = static_cast<unsigned char>(BlockMetaRnd);
				}
				else if ((m_MaxMeta > -1) && (m_MaxMeta == m_MinMeta))
				{
					// Change meta if at least minimum meta was specified
					BlockMetas[i] = static_cast<unsigned char>(m_MaxMeta);
				}
			}
		}  // for i - BlockTypes[]
	}

	virtual void AssignSeed(int a_Seed) override
	{
		m_Seed = a_Seed + SEED_OFFSET;
	}
protected:
	int m_Seed;
	int m_AllWeights = 0;


	/** Block types of a blocks which are being replaced by this strategy */
	std::map<BlockType, int> m_BlocksToReplace;

	/** Randomized blocks with their weights and meta params */
	cRandomizedBlocks m_BlocksToRandomize;

	/** Minimum meta to randomize */
	int m_MinMeta = 0;

	/** Maximum meta to randomize */
	int m_MaxMeta = -1;

	/** Maximum meta in noise range */
	int m_MaxNoiseMeta = 0;

	/** Minimum meta in noise range */
	int m_MinNoiseMeta = 0;
};





////////////////////////////////////////////////////////////////////////////////
// CreatePieceModifierFromString:

bool CreatePieceModifierFromString(const AString & a_Definition, std::shared_ptr<cPiece::cPieceModifiers> & a_Modifiers, bool a_LogWarnings)
{

	auto idxCurlyStart = a_Definition.find('{');
	auto idxCurlyFirstEnd = a_Definition.find('}');
	if ((idxCurlyStart == AString::npos) && (idxCurlyFirstEnd == AString::npos))
	{
		CONDWARNING(a_LogWarnings, "Piece metadata \"Modifiers\" needs at least one valid modifier definition \"%s\"!", a_Definition.c_str());
		return false;
	}

	auto modifiersStr = StringSplitAndTrim(a_Definition, "{");

	for (size_t i = 0; i < modifiersStr.size(); i++)
	{
		AString modifierStr = TrimString(modifiersStr[i]);

		if (modifierStr.size() == 0)
		{
			continue;
		}
		auto idxCurlyEnd = modifierStr.find('}');
		if (idxCurlyEnd == AString::npos)
		{
			CONDWARNING(a_LogWarnings, "Modifier definition must end with curly bracket \"%s\"!!", modifierStr.c_str());
			return false;
		}

		modifierStr = modifierStr.substr(0, idxCurlyEnd);

		// Break apart the modifier class, the first parameter before the first pipe char:
		auto idxPipe = modifierStr.find('|');
		if (idxPipe == AString::npos)
		{
			idxPipe = modifierStr.length();
		}
		AString ModifierClass = modifierStr.substr(0, idxPipe);

		// Create a modifier class based on the class string:
		cPiece::cPieceModifierPtr Modifier;
		if (NoCaseCompare(ModifierClass, "RandomizeBlocks") == 0)
		{
			Modifier = std::make_shared<cPieceModifierRandomizeBlocks>();
		}

		if (Modifier == nullptr)
		{
			CONDWARNING(a_LogWarnings, "Unknown modifier class \"%s\" %s!", ModifierClass.c_str(), modifierStr.c_str());
			return false;
		}

		// Initialize the modifier's parameters:
		AString Params;
		if (idxPipe < modifierStr.length())
		{
			Params = modifierStr.substr(idxPipe + 1);
		}

		if (!Modifier->InitializeFromString(Params, a_LogWarnings))
		{
			CONDWARNING(a_LogWarnings, "InitializeFromString error \"%s\" -- %!", Params.c_str(), modifierStr.c_str());
			return false;
		}

		a_Modifiers->push_back(Modifier);
	}

	return true;
}




