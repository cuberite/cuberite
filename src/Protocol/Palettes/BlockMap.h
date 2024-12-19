#pragma once

#include <Bindings/BlockTypePalette.h>
#include "BlockState.h"
#include "Protocol/Protocol.h"



namespace BlockMap
{
	static class cBlockMap
	{
public:
		void AddVersion(cProtocol::Version a_Version);

		/** This function expects that the BlockStates hardcoded match the latest version supported.
		*  It also expects that the a_target Version was previously added.
		*  Instead of doing multiple looks ups we could at startup for each supported version make a BlockState <-> Version_Specific_Id map and just have a single lookup
		*  Perhaps a compile option or a config option?
		*/
		UInt32 GetProtocolBlockId(cProtocol::Version a_target, BlockState a_block);

		bool IsVersionLoaded(cProtocol::Version a_Version)
		{
			return PerVersionMap.count(a_Version) != 0;
		}
private:
		/** Maps each protocol to its corrpesoing plaette */
		std::map<cProtocol::Version, BlockTypePalette> PerVersionMap;
	};
}
