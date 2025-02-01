#pragma once

#include "Defines.h"
#include "Entities/EntityEffect.h"
#include "Mobs/MonsterTypes.h"
#include "Registries/CustomStatistics.h"
#include "Registries/BlockTypes.h"
#include "Registries/Items.h"

namespace NamespaceSerializer
{
	enum class Namespace
	{
		Minecraft,
		Cuberite,
		Unknown
	};

	unsigned DataVersion();

	std::string_view From(BannerPattern a_ID);
	std::string_view From(BlockType a_ID);
	std::string_view From(CustomStatistic a_ID);
	std::string_view From(eMonsterType a_ID);
	std::string_view From(Item a_ID);
	std::string_view From(cEntityEffect::eType a_ID);

	BlockType ToBlockType(std::string_view a_ID);
	CustomStatistic ToCustomStatistic(std::string_view a_ID);
	Item ToItem(std::string_view a_ID);
	eMonsterType ToMonsterType(std::string_view a_ID);
	cEntityEffect::eType ToEntityEffect(std::string_view a_ID);

	std::pair<Namespace, std::string_view> SplitNamespacedID(std::string_view ID);

	// Examples: Input: "wolf" -> Output: "Wolf", Input: "iron_golem" -> Output: "Iron Golem"
	AString Prettify(AString a_ID);
	AString PrettifyEntityName(const AString & a_ID, const bool a_IsTamed = false);

	// change string from ThisIsAString to this_is_a_string
	AString DePrettify(std::string_view a_ID);
	// Checks is the string is ThisIsAString
	bool IsPretty(std::string_view a_ID);
}
