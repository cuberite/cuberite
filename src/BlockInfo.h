
#pragma once






class BlockInfo
{
public:

	BlockInfo();

	/** (Re-)Initializes the internal BlockInfo structures. */
	static void Initialize(void);

	/** Returns the associated BlockInfo structure. */
	static BlockInfo & GetById(unsigned int a_ID);

	NIBBLETYPE m_LightValue;
	NIBBLETYPE m_SpreadLightFalloff;

	bool m_Transparent;
	bool m_OneHitDig;
	bool m_PistonBreakable;
	bool m_IsSnowable;
	bool m_RequiresSpecialTool;
	bool m_IsSolid;
	bool m_FullyOccupiesVoxel;


	inline static NIBBLETYPE GetLightValue        (unsigned int a_ID) { return GetById(a_ID).m_LightValue;          }
	inline static NIBBLETYPE GetSpreadLightFalloff(unsigned int a_ID) { return GetById(a_ID).m_SpreadLightFalloff;  }
	inline static bool IsTransparent              (unsigned int a_ID) { return GetById(a_ID).m_Transparent;         }
	inline static bool IsOneHitDig                (unsigned int a_ID) { return GetById(a_ID).m_OneHitDig;           }
	inline static bool IsPistoneBreakable         (unsigned int a_ID) { return GetById(a_ID).m_PistonBreakable;     }
	inline static bool IsSnowable                 (unsigned int a_ID) { return GetById(a_ID).m_IsSnowable;          }
	inline static bool RequiresSpecialTool        (unsigned int a_ID) { return GetById(a_ID).m_RequiresSpecialTool; }
	inline static bool IsSolid                    (unsigned int a_ID) { return GetById(a_ID).m_IsSolid;             }
	inline static bool FullyOccupiesVoxel         (unsigned int a_ID) { return GetById(a_ID).m_FullyOccupiesVoxel;  }


protected:

	// TODO xdot: Change to std::vector to support dynamic block IDs
	static BlockInfo ms_Info[256];


};




