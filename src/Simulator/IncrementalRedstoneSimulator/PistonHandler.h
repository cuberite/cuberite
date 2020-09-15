
#pragma once

#include "../../Blocks/BlockButton.h"
#include "../../Blocks/BlockLever.h"
#include "../../Blocks/BlockPiston.h"




// Without this compiler falls back to PistonHandler's version (Even with namespace prefix)
extern inline PowerLevel RedstoneHandler::GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked);




namespace PistonHandler
{
	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		UNUSED(IsLinked);
		return 0;
	}

	inline void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating pisty the piston (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		// Needs to be editable
		auto EffectivePower = (PowerLevel) Power;

		// If isn't powered directly, try get power using quasi-connectivity
		if (Power == 0)
		{
			const auto QCSocketPos  = a_Position.addedY(1);
			const auto QCSocketType = a_Chunk.GetBlock(QCSocketPos);

			for (const auto SourceOffset : RelativeAdjacents)
			{
				// Don't check the piston itself
				if (SourceOffset.y == -1)
				{
					continue;
				}

				const auto SourceBlockPos  = QCSocketPos + SourceOffset;
				const auto SourceBlockType = a_Chunk.GetBlock(SourceBlockPos);

				// Solid blocks get power from further away and thus require block update
				if (cBlockInfo::IsSolid(SourceBlockType))
				{
					// Except for one (maybe more, this is just what I found with testing)
					if (SourceBlockType != E_BLOCK_TRAPPED_CHEST)
					{
						// Skip block of redstone as it uses block update detection
						if (SourceBlockType == E_BLOCK_TRAPPED_CHEST)
						{
							continue;
						}

						// Query block above for attachments
						const auto SourceBlockAttachmentPos  = SourceBlockPos.addedY(1);
						const auto SourceBlockAttachmentType = a_Chunk.GetBlock(SourceBlockAttachmentPos);

						// Attachments must be diagonal to QC Socket in order to provide block update
						switch (SourceBlockAttachmentType)
						{
							// Detector rails - can only be put on top
							case E_BLOCK_DETECTOR_RAIL: break;

							// Levers
							case E_BLOCK_LEVER:
							{
								const auto LeverMeta = a_Chunk.GetMeta(SourceBlockAttachmentPos);

								if (cBlockLeverHandler::BlockMetaDataToBlockFace(LeverMeta) != BLOCK_FACE_YP)
								{
									continue;
								}
							}
							break;

							// Buttons
							case E_BLOCK_STONE_BUTTON:
							case E_BLOCK_WOODEN_BUTTON:
							{
								const auto ButtonMeta = a_Chunk.GetMeta(SourceBlockAttachmentPos);

								if (cBlockButtonHandler::BlockMetaDataToBlockFace(ButtonMeta) != BLOCK_FACE_YP)
								{
									continue;
								}
							}
							break;

							// Pressure plates - can only be put on top
							case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
							case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
							case E_BLOCK_STONE_PRESSURE_PLATE:
							case E_BLOCK_WOODEN_PRESSURE_PLATE: break;

							// No suitable attachments on this source - look further
							default: continue;
						}
					}
				}
				else
				{
					auto BlockFaceToOffset = [](eBlockFace a_BlockFace)
					{
						// Axial vector of length 0
						const auto O = Vector3i({0, 0, 0});

						switch (a_BlockFace)
						{
							case BLOCK_FACE_XP: return O.addedX(1);
							case BLOCK_FACE_XM: return O.addedX(-1);
							case BLOCK_FACE_YP: return O.addedY(1);
							case BLOCK_FACE_YM: return O.addedY(-1);
							case BLOCK_FACE_ZP: return O.addedZ(1);
							case BLOCK_FACE_ZM: return O.addedZ(-1);

							// Only evaluate for adjacent blocks
							default: return O;
						}
					};

					// If the source block is itself attacheable, it must be attached to QC socket
					switch (SourceBlockType)
					{
						// Levers
						case E_BLOCK_LEVER:
						{
							const auto LeverMeta   = a_Chunk.GetMeta(SourceBlockPos);
							const auto LeverOffset = BlockFaceToOffset(cBlockLeverHandler::BlockMetaDataToBlockFace(LeverMeta));

							if (SourceOffset != LeverOffset)
							{
								continue;
							}
						}
						break;

						// Buttons
						case E_BLOCK_STONE_BUTTON:
						case E_BLOCK_WOODEN_BUTTON:
						{
							const auto ButtonMeta   = a_Chunk.GetMeta(SourceBlockPos);
							const auto ButtonOffset = BlockFaceToOffset(cBlockButtonHandler::BlockMetaDataToBlockFace(ButtonMeta));

							if (SourceOffset != ButtonOffset)
							{
								continue;
							}
						}
						break;
					}
				}

				// Query how much power the source can give
				EffectivePower = RedstoneHandler::GetPowerDeliveredToPosition(
					a_Chunk,
					SourceBlockPos,
					SourceBlockType,
					QCSocketPos,
					QCSocketType,
					false);

				if (EffectivePower != 0)
				{
					break;
				}
			}
		}

		const bool ShouldBeExtended = EffectivePower != 0;
		if (ShouldBeExtended == cBlockPistonHandler::IsExtended(a_Meta))
		{
			return;
		}

		a_Position = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());

		if (ShouldBeExtended)
		{
			cBlockPistonHandler::ExtendPiston(a_Position, *a_Chunk.GetWorld());
		}
		else
		{
			cBlockPistonHandler::RetractPiston(a_Position, *a_Chunk.GetWorld());
		}

		// It is necessary to delay after a signal to prevent an infinite loop (#3168)
		// However, this delay is already present: as a side effect of the implementation of piston animation in Blocks\BlockPiston.cpp
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);

		const auto Face = cBlockPistonHandler::MetaDataToDirection(a_Meta);
		const auto FrontOffset = AddFaceDirection(Vector3i(), Face);

		for (const auto Offset : RelativeAdjacents)
		{
			if (Offset != FrontOffset)
			{
				Callback(a_Position + Offset);
			}
		}
	}
};
