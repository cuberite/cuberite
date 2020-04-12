
#pragma once

#include "ItemHandler.h"
#include "../Entities/Player.h"
#include "../LineBlockTracer.h"





class cItemLilypadHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemLilypadHandler(int a_ItemType):
		super(a_ItemType)
	{
		LOGD("Created Lilypad handler");
	}


	virtual bool IsPlaceable(void) override
	{
		return false;  // Set as not placeable so OnItemUse is called
	}

	virtual bool OnItemUse(
        cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
        int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
    ) override
	{
		LOGD("Lilypad OnItemUse");
		LOGD(BlockFaceToString(a_BlockFace).c_str());
		if (a_BlockFace > BLOCK_FACE_NONE)
		{
			LOGD("Trying to place block normally");
			// We clicked on a block; no need to trace, just use normal placement method
			return OnPlayerPlace(*a_World, *a_Player, a_Item, 
				a_BlockX, a_BlockY, a_BlockZ, a_BlockFace,
				0, 0, 0
			);
		}

		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:

			cCallbacks(void) :
				m_HasHitFluid(false)
			{
			}

			virtual bool OnNextBlock(int a_CBBlockX, int a_CBBlockY, int a_CBBlockZ, BLOCKTYPE a_CBBlockType, NIBBLETYPE a_CBBlockMeta, eBlockFace a_CBEntryFace) override
			{	
				if((((a_CBBlockType == E_BLOCK_STATIONARY_WATER) || (a_CBBlockType == E_BLOCK_WATER)) && (a_CBBlockMeta == 0))
			|| (Surface == E_BLOCK_ICE) || (Surface == E_BLOCK_FROSTED_ICE))                                     // Or (frosted) ice
				{	
					m_HasHitFluid = true;
					m_Pos.Set(a_CBBlockX, a_CBBlockY, a_CBBlockZ);
					m_BlockFace = a_CBEntryFace;
					return true;
				
				}
				return false;
			}

			Vector3i m_Pos;
			eBlockFace m_BlockFace;
			bool m_HasHitFluid;
		};

		cCallbacks Callbacks;
		cLineBlockTracer Tracer(*a_World, Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start.x, Start.y, Start.z, End.x, End.y, End.z);

		if (!Callbacks.m_HasHitFluid)
		{
			return false;
		}
		
		LOGD("Found surface %s, trying to place normally", BlockFaceToString(Callbacks.m_BlockFace).c_str());

		// Since we hit fluid, try placing the block on top of the fluid we found
		// Mimick clicking on top of the fluid to do this
		return OnPlayerPlace(*a_World, *a_Player, a_Item,
			Callbacks.m_Pos.x, Callbacks.m_Pos.y, Callbacks.m_Pos.z, Callbacks.m_BlockFace,
			0, 0, 0
		);
	}

	virtual bool OnPlayerPlace(
        cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
        int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
        int a_CursorX, int a_CursorY, int a_CursorZ
    ) override
	{
		LOGD("Called onPlayerPlace");
		
		Vector3i PlacementPos = GetBlockNextTo(Vector3i(a_BlockX, a_BlockY, a_BlockZ), a_BlockFace);
		
		// If the block where we are trying to place is a water block, try placing on top of the water block instead
		BLOCKTYPE Placement = a_World.GetBlock(PlacementPos);
		NIBBLETYPE PlacementMeta = a_World.GetBlockMeta(PlacementPos);
		if(((Placement == E_BLOCK_STATIONARY_WATER) || (Placement == E_BLOCK_WATER)) && (PlacementMeta == 0))  // A water source is in the place of where we want to place the block
		{
			return false;
		}	

		BLOCKTYPE Surface = a_World.GetBlock(PlacementPos - Vector3i(0, 1, 0));
		NIBBLETYPE SurfaceMeta = a_World.GetBlockMeta(PlacementPos - Vector3i(0, 1, 0));
		if(!((((Surface == E_BLOCK_STATIONARY_WATER) || (Surface == E_BLOCK_WATER)) && (SurfaceMeta == 0)) ||  // A water source is below
			(Surface == E_BLOCK_ICE) || (Surface == E_BLOCK_FROSTED_ICE)))                                     // Or (frosted) ice
		{
			return false;
		}

		// Otherwise do the same exact process as cItemHandler::OnPlayerPlace, but ignore build collision

		Vector3i absPos(a_BlockX, a_BlockY, a_BlockZ);

		if (a_BlockFace < 0)
		{
			// Clicked in air
			return false;
		}
	
		if (!cChunkDef::IsValidHeight(absPos.y))
		{
			// The clicked block is outside the world, ignore this call altogether (#128)
			return false;
		}
	
		BLOCKTYPE ClickedBlock;
		NIBBLETYPE ClickedBlockMeta;
		a_World.GetBlockTypeMeta(absPos.x, absPos.y, absPos.z, ClickedBlock, ClickedBlockMeta);

		cChunkInterface ChunkInterface(a_World.GetChunkMap());
		
		absPos = GetBlockNextTo(absPos, a_BlockFace);

		if (!cChunkDef::IsValidHeight(absPos.y))
		{
			// The block is being placed outside the world, ignore this packet altogether (#128)
			return false;
		}

		NIBBLETYPE PlaceMeta;
		BLOCKTYPE PlaceBlock;
		a_World.GetBlockTypeMeta(absPos.x, absPos.y, absPos.z, PlaceBlock, PlaceMeta);

		// Check for another block here, remove it if appropriate.
		if (!BlockHandler(PlaceBlock)->DoesIgnoreBuildCollision(ChunkInterface, absPos, a_Player, PlaceMeta))
		{
			LOGD("IgnoreBuildCollision Failed");
			// Tried to place a block into another?
			// Happens when you place a block aiming at side of block with a torch on it or stem beside it
			return false;
		}

		a_World.DropBlockAsPickups(absPos, &a_Player, nullptr);
	
		// Get all the blocks to place:
		sSetBlockVector blocks;
		if (!GetBlocksToPlace(a_World, a_Player, a_EquippedItem, absPos.x, absPos.y, absPos.z, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, blocks))
		{
			LOGD("GetBlocksToPlace Failed");
			// Handler refused the placement, send that information back to the client:
			for (const auto & blk: blocks)
			{
				a_World.SendBlockTo(blk.GetX(), blk.GetY(), blk.GetZ(), a_Player);
			}
			a_World.SendBlockTo(absPos.x, absPos.y, absPos.z, a_Player);
			a_Player.GetInventory().SendEquippedSlot();
			return false;
		}
	
		// Try to place the blocks:
		if (!a_Player.PlaceBlocks(blocks))
		{
			LOGD("PlaceBlocks Failed");
			// The placement failed, the blocks have already been re-sent, re-send inventory:
			a_Player.GetInventory().SendEquippedSlot();
			return false;
		}
	
		// Remove the "placed" item:
		if (a_Player.IsGameModeSurvival())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
		
		return true;
	}
};






