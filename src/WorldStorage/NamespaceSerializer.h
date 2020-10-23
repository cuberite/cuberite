#pragma once

#include "../Registries/Statistics.h"
#include "../Entities/EntityEffect.h"
#include "../Enchantments.h"

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

	const char * From(cEntityEffect::eType a_ID);
	cEntityEffect::eType ToEntityEffect(std::string_view a_ID);

	const char * From(cEnchantments::eEnchantment a_Enchantment);
	cEnchantments::eEnchantment ToEnchantmentID(std::string_view a_ID);

	std::pair<Namespace, std::string_view> SplitNamespacedID(std::string_view ID);
}
