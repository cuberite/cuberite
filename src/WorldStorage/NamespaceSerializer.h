#pragma once

#include "../Registries/Statistics.h"
#include "../Mobs/MonsterTypes.h"

namespace NamespaceSerializer
{
	enum class Namespace
	{
		Minecraft,
		Cuberite,
		Unknown
	};

	unsigned DataVersion();

	const char * From(Statistic a_ID);
	const char * From(eMonsterType a_ID);

	Statistic ToCustomStatistic(std::string_view a_ID);
	eMonsterType ToMonsterType(std::string_view a_ID);

	std::pair<Namespace, std::string_view> SplitNamespacedID(std::string_view ID);
}
