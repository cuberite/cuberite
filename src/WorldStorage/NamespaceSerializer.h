#pragma once

#include "../Registries/Statistics.h"

namespace NamespaceSerializer
{
	enum class Namespace
	{
		Minecraft,
		Cuberite,
		Unknown
	};

	unsigned DataVersion();

	const char * From(Statistic ID);

	Statistic ToCustomStatistic(std::string_view ID);

	std::pair<Namespace, std::string_view> SplitNamespacedID(std::string_view ID);
}
