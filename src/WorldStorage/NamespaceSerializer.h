#pragma once

#include "Defines.h"
#include "Mobs/MonsterTypes.h"
#include "Registries/CustomStatistics.h"
#include "Registries/PoiTypes.h"

namespace NamespaceSerializer
{
	enum class Namespace
	{
		Minecraft,
		Cuberite,
		Unknown
	};

	unsigned DataVersion();

	std::string_view From(CustomStatistic a_ID);
	std::string_view From(eMonsterType a_ID);
	std::string_view From(BannerPattern a_ID);
	std::string_view From(ePoiType a_ID);

	CustomStatistic ToCustomStatistic(std::string_view a_ID);
	eMonsterType ToMonsterType(std::string_view a_ID);
	ePoiType ToPoiType(std::string_view a_ID);

	std::pair<Namespace, std::string_view> SplitNamespacedID(std::string_view ID);

	// Examples: Input: "wolf" -> Output: "Wolf", Input: "iron_golem" -> Output: "Iron Golem"
	AString Prettify(AString a_ID);
	AString PrettifyEntityName(const AString & a_ID, const bool a_IsTamed = false);
}
