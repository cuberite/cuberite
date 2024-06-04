#pragma once

#include "Defines.h"
#include "Entities/Entity.h"
#include "Entities/Minecart.h"
#include "Entities/ProjectileEntity.h"
#include "Mobs/MonsterTypes.h"
#include "Registries/CustomStatistics.h"
#include "Registries/DataVersion.h"

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
	Int32 From(eDataVersion a_ID);

	CustomStatistic ToCustomStatistic(std::string_view a_ID);
	eDataVersion ToDataVersion(Int32 a_ID);
	cEntity::eEntityType ToEntityType(std::string_view a_ID);
	cMinecart::ePayload ToMinecartType(std::string_view a_ID);
	cProjectileEntity::eKind ToProjectileType(std::string_view a_ID);
	eMonsterType ToMonsterType(std::string_view a_ID);

	std::pair<Namespace, std::string_view> SplitNamespacedID(std::string_view ID);

	// Examples: Input: "wolf" -> Output: "Wolf", Input: "iron_golem" -> Output: "Iron Golem"
	AString Prettify(AString a_ID);
	AString PrettifyEntityName(const AString & a_ID, const bool a_IsTamed = false);
}
