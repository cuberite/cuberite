#include "Globals.h"
#include <Bindings/BlockTypePalette.h>
#include "BlockMap.h"
#include "Protocol/ProtocolRecognizer.h"
#include "filesystem"

namespace BlockMap
{
	void cBlockMap::AddVersion(cProtocol::Version a_Version)
	{
		if (m_PerVersionMap.count(a_Version) != 0)
		{
			LOGWARNING(fmt::format(FMT_STRING("Tried to add version {} twice, ignoring"), cMultiVersionProtocol::GetVersionTextFromInt(a_Version)));
			return;
		}
		AString file_name = "Protocol/" + cMultiVersionProtocol::GetVersionTextFromInt(a_Version) + "/blocks.json";
		std::fstream file;
		file.open(file_name, std::ios::in);
		if (!file.is_open())
		{
			LOGERROR(fmt::format(FMT_STRING("Failed to open block.json file for version {}. Check if it exists at Protocol\\{}\\blocks.json. THIS VERSION WILL NOT WORK"), cMultiVersionProtocol::GetVersionTextFromInt(a_Version), cMultiVersionProtocol::GetVersionTextFromInt(a_Version)));
			return;
		}
		if (file.fail())
		{
			LOGERROR(fmt::format(FMT_STRING("Failed to read block.json file for version {}. THIS VERSION WILL NOT WORK"), cMultiVersionProtocol::GetVersionTextFromInt(a_Version)));
			return;
		}
		LOGD(fmt::format(FMT_STRING("Loading block palette for version {}"), cMultiVersionProtocol::GetVersionTextFromInt(a_Version)));
		const auto sz = std::filesystem::file_size(file_name);
		std::string result(sz, '\0');
		file.read(result.data(), static_cast<std::streamsize>(sz));
		m_PerVersionMap[a_Version] = BlockTypePalette();
		m_PerVersionMap.at(a_Version).loadFromString(result);
	}





	UInt32 cBlockMap::GetProtocolBlockId(cProtocol::Version a_target, BlockState a_block)
	{
		const BlockTypePalette & latest = m_PerVersionMap[cProtocol::Version::Latest];
		BlockTypePalette & target_ver = m_PerVersionMap[a_target];

		const auto & entry = latest.entry(a_block.ID);
		const auto ret = target_ver.maybeIndex(entry.first, entry.second);
		if (ret.second)
		{
			return ret.first;
		}
		return 0;
	}
}
