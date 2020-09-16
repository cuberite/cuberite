
#pragma once





// fwd:
class cBlockHandler;




// tolua_begin
class cBlockInfo
{
public:
	// tolua_end

	/** The block type associated with this cBlockInfo. Needed for DeprecatedBindings.cpp */
	BLOCKTYPE m_BlockType;

	/** Returns the associated BlockInfo structure for the specified block type.
	This accessor makes sure that the cBlockInfo structures are properly initialized exactly once.
	It does so by using the C++ singleton approximation - storing the actual singleton as the function's static variable. */
	inline static const cBlockInfo & Get(BLOCKTYPE a_Type);

	// tolua_begin

	inline static NIBBLETYPE GetLightValue        (const BLOCKTYPE a_Type) { return Get(a_Type).m_LightValue;          }
	inline static NIBBLETYPE GetSpreadLightFalloff(const BLOCKTYPE a_Type) { return Get(a_Type).m_SpreadLightFalloff;  }
	inline static bool IsTransparent              (const BLOCKTYPE a_Type) { return Get(a_Type).m_Transparent;         }
	inline static bool IsOneHitDig                (const BLOCKTYPE a_Type) { return Get(a_Type).m_OneHitDig;           }
	inline static bool IsPistonBreakable          (const BLOCKTYPE a_Type) { return Get(a_Type).m_PistonBreakable;     }
	inline static bool IsRainBlocker              (const BLOCKTYPE a_Type) { return Get(a_Type).m_IsRainBlocker;       }
	inline static bool IsSkylightDispersant       (const BLOCKTYPE a_Type)
	{
		return ((Get(a_Type).m_IsSkylightDispersant) || (Get(a_Type).m_SpreadLightFalloff > 1));
	}
	static bool IsSnowable(BLOCKTYPE a_Type);
	inline static bool IsSolid                    (const BLOCKTYPE a_Type) { return Get(a_Type).m_IsSolid;             }
	inline static bool IsUseableBySpectator       (const BLOCKTYPE a_Type) { return Get(a_Type).m_UseableBySpectator;  }
	inline static bool FullyOccupiesVoxel         (const BLOCKTYPE a_Type) { return Get(a_Type).m_FullyOccupiesVoxel;  }
	inline static bool CanBeTerraformed           (const BLOCKTYPE a_Type) { return Get(a_Type).m_CanBeTerraformed;    }
	inline static float GetBlockHeight            (const BLOCKTYPE a_Type) { return Get(a_Type).m_BlockHeight;         }
	inline static float GetHardness               (const BLOCKTYPE a_Type) { return Get(a_Type).m_Hardness;            }

	// tolua_end

	/** Returns how much of an explosion Destruction Lazor's (tm) intensity the given block attenuates.
	See Physics\Explodinator.cpp for details of explosion block destruction. */
	static float GetExplosionAbsorption(BLOCKTYPE Block);

	inline static cBlockHandler * GetHandler      (BLOCKTYPE a_Type) { return Get(a_Type).m_Handler.get();       }

	/** Creates a default BlockInfo structure, initializes all values to their defaults */
	cBlockInfo();


private:
	/** Storage for all the BlockInfo structures. */
	class cBlockInfoArray;

	/** How much light do the blocks emit on their own? */
	NIBBLETYPE m_LightValue;

	/** How much light do the blocks consume? */
	NIBBLETYPE m_SpreadLightFalloff;

	/** Is a block transparent? (https://minecraft.gamepedia.com/Opacity) */
	bool m_Transparent;

	/** Is a block destroyed after a single hit? */
	bool m_OneHitDig;

	/** Can a piston break this block? */
	bool m_PistonBreakable;

	/** Does this block block the passage of rain? */
	bool m_IsRainBlocker;

	/** Does this block disperse sky light? (only relevant for transparent blocks) */
	bool m_IsSkylightDispersant;

	/** Is this block solid (player cannot walk through)? */
	bool m_IsSolid;

	/** Can a spectator interact with this block */
	bool m_UseableBySpectator;

	/** Does this block fully occupy its voxel - is it a 'full' block? */
	bool m_FullyOccupiesVoxel;

	/** Can a finisher change it? */
	bool m_CanBeTerraformed;

	/** Block height */
	float m_BlockHeight;

	/** Block's hardness. The greater the value the longer the player needs to break the block. */
	float m_Hardness;

	/** Custom deleter allows cBlockHandler to be an incomplete type. */
	struct sHandlerDeleter
	{
		void operator () (cBlockHandler * a_Handler);
	};

	/** Associated block handler. */
	std::unique_ptr<cBlockHandler, sHandlerDeleter> m_Handler;
};  // tolua_export





bool IsBlockWater(const BLOCKTYPE a_BlockType);

bool IsBlockIce(const BLOCKTYPE a_BlockType);

bool IsBlockWaterOrIce(const BLOCKTYPE a_BlockType);

bool IsBlockLava(const BLOCKTYPE a_BlockType);

bool IsBlockLiquid(const BLOCKTYPE a_BlockType);

bool IsBlockRail(const BLOCKTYPE a_BlockType);

bool IsBlockTypeOfDirt(const BLOCKTYPE a_BlockType);

bool IsBlockFence(const BLOCKTYPE a_BlockType);

bool IsBlockMaterialWood(const BLOCKTYPE a_BlockType);

bool IsBlockMaterialPlants(const BLOCKTYPE a_BlockType);

bool IsBlockMaterialVine(const BLOCKTYPE a_BlockType);

bool IsBlockMaterialIron(const BLOCKTYPE a_BlockType);

bool IsBlockMaterialLeaves(const BLOCKTYPE a_BlockType);

bool IsBlockMaterialGourd(const BLOCKTYPE a_BlockType);

bool IsBlockMaterialRock(const BLOCKTYPE a_BlockType);

bool IsBlockMaterialTerracotta(const BLOCKTYPE a_BlockType);




class cBlockInfo::cBlockInfoArray:
	public std::array<cBlockInfo, 256>
{
public:
	/** Initializes the contained BlockInfo structures with block-specific values. */
	cBlockInfoArray();
};





inline const cBlockInfo & cBlockInfo::Get(BLOCKTYPE a_Type)
{
	static const cBlockInfoArray ms_Info;
	return ms_Info[a_Type];
}





// Shortcut to get the blockhandler for a specific block
inline cBlockHandler * BlockHandler(BLOCKTYPE a_BlockType)
{
	return cBlockInfo::GetHandler(a_BlockType);
}
