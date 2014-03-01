
#pragma once






class cBlockInfo
{
public:

	cBlockInfo();

	/** (Re-)Initializes the internal BlockInfo structures. */
	static void Initialize(void);

	/** Returns the associated BlockInfo structure. */
	static cBlockInfo & GetById(unsigned int a_ID);


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

	/** Does this block require a tool to drop? */
	bool m_RequiresSpecialTool;

	/** Is this block solid (player cannot walk through)? */
	bool m_IsSolid;

	/** Does this block fully occupy it's voxel - is it a 'full' block? */
	bool m_FullyOccupiesVoxel;


	inline static NIBBLETYPE GetLightValue        (unsigned int a_ID) { return GetById(a_ID).m_LightValue;          }
	inline static NIBBLETYPE GetSpreadLightFalloff(unsigned int a_ID) { return GetById(a_ID).m_SpreadLightFalloff;  }
	inline static bool IsTransparent              (unsigned int a_ID) { return GetById(a_ID).m_Transparent;         }
	inline static bool IsOneHitDig                (unsigned int a_ID) { return GetById(a_ID).m_OneHitDig;           }
	inline static bool IsPistonBreakable          (unsigned int a_ID) { return GetById(a_ID).m_PistonBreakable;     }
	inline static bool IsSnowable                 (unsigned int a_ID) { return GetById(a_ID).m_IsSnowable;          }
	inline static bool RequiresSpecialTool        (unsigned int a_ID) { return GetById(a_ID).m_RequiresSpecialTool; }
	inline static bool IsSolid                    (unsigned int a_ID) { return GetById(a_ID).m_IsSolid;             }
	inline static bool FullyOccupiesVoxel         (unsigned int a_ID) { return GetById(a_ID).m_FullyOccupiesVoxel;  }


protected:

	// TODO xdot: Change to std::vector to support dynamic block IDs
	static cBlockInfo ms_Info[256];


};




