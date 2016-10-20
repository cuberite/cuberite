
#pragma once





// fwd:
class cBlockHandler;




// tolua_begin
class cBlockInfo
{
public:

	/** Returns the associated BlockInfo structure for the specified block type. */

	/** This accessor makes sure that the cBlockInfo structures are properly initialized exactly once.
	It does so by using the C++ singleton approximation - storing the actual singleton as the function's static variable.
	It works only if it is called for the first time before the app spawns other threads. */
	static cBlockInfo & Get(BLOCKTYPE a_Type)
	{
		static cBlockInfo ms_Info[256];
		static bool IsBlockInfoInitialized = false;
		if (!IsBlockInfoInitialized)
		{
			cBlockInfo::Initialize(ms_Info);
			IsBlockInfoInitialized = true;
		}
		return ms_Info[a_Type];
	}


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

	/** Can a spectator interact with this block */
	bool m_UseableBySpectator;

	/** Does this block fully occupy its voxel - is it a 'full' block? */
	bool m_FullyOccupiesVoxel;

	/** Can a finisher change it? */
	bool m_CanBeTerraformed;

	/** Block height */
	float m_BlockHeight;

	/** Sound when placing this block */
	AString m_PlaceSound;

	/** Block's hardness. The bigger the harder the block. */
	float m_Hardness;

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
	inline static bool IsUseableBySpectator       (BLOCKTYPE a_Type) { return Get(a_Type).m_UseableBySpectator;  }
	inline static bool FullyOccupiesVoxel         (BLOCKTYPE a_Type) { return Get(a_Type).m_FullyOccupiesVoxel;  }
	inline static bool CanBeTerraformed           (BLOCKTYPE a_Type) { return Get(a_Type).m_CanBeTerraformed;    }
	inline static float GetBlockHeight            (BLOCKTYPE a_Type) { return Get(a_Type).m_BlockHeight;         }
	inline static AString GetPlaceSound           (BLOCKTYPE a_Type) { return Get(a_Type).m_PlaceSound;          }
	inline static float GetHardness               (BLOCKTYPE a_Type) { return Get(a_Type).m_Hardness;            }

	// tolua_end

	inline static cBlockHandler * GetHandler      (BLOCKTYPE a_Type) { return Get(a_Type).m_Handler;             }

protected:
	/** Storage for all the BlockInfo structures. */
	typedef cBlockInfo cBlockInfoArray[256];

	/** Creates a default BlockInfo structure, initializes all values to their defaults */
	cBlockInfo()
		: m_LightValue(0x00)
		, m_SpreadLightFalloff(0x0f)
		, m_Transparent(false)
		, m_OneHitDig(false)
		, m_PistonBreakable(false)
		, m_IsSnowable(false)
		, m_IsSolid(true)
		, m_UseableBySpectator(false)
		, m_FullyOccupiesVoxel(false)
		, m_CanBeTerraformed(false)
		, m_BlockHeight(1.0)
		, m_PlaceSound("")
		, m_Hardness(0.0f)
		, m_Handler(nullptr)
	{}

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




