#pragma once

#include <Bindings/BlockTypePalette.h>
#include "BlockState.h"
#include "Protocol/Protocol.h"



namespace BlockMap
{
	class cBlockMap
	{
public:
		void AddVersion(cProtocol::Version a_Version);

		/** This function expects that the BlockStates hardcoded match the latest version supported.
		*  It also expects that the a_target Version was previously added.
		*  Instead of doing multiple looks ups we could at startup for each supported version make a BlockState <-> Version_Specific_Id map and just have a single lookup
		*  Perhaps a compile option or a config option?
		*/
		UInt32 GetProtocolBlockId(cProtocol::Version a_target, BlockState a_block);
		const BlockTypePalette & GetPalette(cProtocol::Version a_target) const;

		void LoadAll();

		bool IsVersionLoaded(cProtocol::Version a_Version) const
		{
			return m_PerVersionMap.count(a_Version) != 0;
		}
private:
		/** Maps each protocol to its corresponding palette */
		std::map<cProtocol::Version, BlockTypePalette> m_PerVersionMap;
	};
}
