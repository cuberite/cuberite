#pragma once

#include "Defines.h"
#include "../Entities/EntityEffect.h"
#include "../Enchantments.h"
#include "Mobs/MonsterTypes.h"
#include "Registries/Statistics.h"

namespace NamespaceSerializer
{
	enum class Namespace
	{
		Minecraft,
		Cuberite,
		Unknown
	};

	unsigned DataVersion();

	std::string_view From(Statistic a_ID);
	std::string_view From(eMonsterType a_ID);
	std::string_view From(BannerPattern a_ID);

	Statistic ToCustomStatistic(std::string_view a_ID);
	eMonsterType ToMonsterType(std::string_view a_ID);

	const char * From(cEntityEffect::eType a_ID);
	cEntityEffect::eType ToEntityEffect(std::string_view a_ID);

	const char * From(cEnchantments::eEnchantment a_Enchantment);
	cEnchantments::eEnchantment ToEnchantmentID(std::string_view a_ID);

	std::pair<Namespace, std::string_view> SplitNamespacedID(std::string_view ID);
}
