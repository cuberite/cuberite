
#pragma once





// fwd:
class cBlockHandler;





// tolua_begin
class cBlockInfo
{
public:

	/** Returns the associated BlockInfo structure for the specified block type. */
	static cBlockInfo & Get(BLOCKTYPE a_Type);


	/** How much light do the blocks emit on their own? */
	NIBBLETYPE m_LightValue;

	/** How much light do the blocks consume? */
	NIBBLETYPE m_SpreadLightFalloff;

	/** Is a block completely transparent? (light doesn't get decreased(?)) */
	bool m_Transparent;

	/** Is a block destroyed after a single hit? */
	bool m_OneHitDig;

	/** Can a piston break this block? */
	bool m_PistonBreakable;

	/** Can this block hold snow atop? */
	bool m_IsSnowable;

	/** Is this block solid (player cannot walk through)? */
	bool m_IsSolid;

	/** Does this block fully occupy its voxel - is it a 'full' block? */
	bool m_FullyOccupiesVoxel;

	// tolua_end

	/** Associated block handler. */
	cBlockHandler * m_Handler;

	// tolua_begin

	inline static NIBBLETYPE GetLightValue        (BLOCKTYPE a_Type) { return Get(a_Type).m_LightValue;          }
	inline static NIBBLETYPE GetSpreadLightFalloff(BLOCKTYPE a_Type) { return Get(a_Type).m_SpreadLightFalloff;  }
	inline static bool IsTransparent              (BLOCKTYPE a_Type) { return Get(a_Type).m_Transparent;         }
	inline static bool IsOneHitDig                (BLOCKTYPE a_Type) { return Get(a_Type).m_OneHitDig;           }
	inline static bool IsPistonBreakable          (BLOCKTYPE a_Type) { return Get(a_Type).m_PistonBreakable;     }
	inline static bool IsSnowable                 (BLOCKTYPE a_Type) { return Get(a_Type).m_IsSnowable;          }
	inline static bool IsSolid                    (BLOCKTYPE a_Type) { return Get(a_Type).m_IsSolid;             }
	inline static bool FullyOccupiesVoxel         (BLOCKTYPE a_Type) { return Get(a_Type).m_FullyOccupiesVoxel;  }

	// tolua_end

	inline static cBlockHandler * GetHandler      (BLOCKTYPE a_Type) { return Get(a_Type).m_Handler;             }

protected:
	/** Storage for all the BlockInfo structures. */
	typedef cBlockInfo cBlockInfoArray[256];

	/** Creates a default BlockInfo structure, initializes all values to their defaults */
	cBlockInfo();

	/** Cleans up the stored values */
	~cBlockInfo();

	/** Initializes the specified BlockInfo structures with block-specific values. */
	static void Initialize(cBlockInfoArray & a_BlockInfos);
};  // tolua_export





// Shortcut to get the blockhandler for a specific block
inline cBlockHandler * BlockHandler(BLOCKTYPE a_BlockType)
{
	return cBlockInfo::Get(a_BlockType).m_Handler;
}




