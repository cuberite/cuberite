#pragma once

#include "Defines.h"
#include "Mobs/MonsterTypes.h"
#include "Registries/CustomStatistics.h"

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

	CustomStatistic ToCustomStatistic(std::string_view a_ID);
	eMonsterType ToMonsterType(std::string_view a_ID);

	std::pair<Namespace, std::string_view> SplitNamespacedID(std::string_view ID);
}
