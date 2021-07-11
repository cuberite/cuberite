
// StatisticsSerializer.cpp


#include "Globals.h"
#include "StatisticsManager.h"
#include "StatisticsSerializer.h"
#include "NamespaceSerializer.h"

#include <json/json.h>





namespace StatisticsSerializer
{
	static auto MakeStatisticsDirectory(const std::string & WorldPath, std::string && FileName)
	{
		// Even though stats are shared between worlds, they are (usually) saved
		// inside the folder of the default world.

		// Path to the world's statistics folder.
		const auto Path = WorldPath + cFile::GetPathSeparator() + "stats";

		// Ensure that the directory exists.
		cFile::CreateFolder(Path);

		return Path + cFile::GetPathSeparator() + std::move(FileName) + ".json";
	}





	static void SaveStatToJSON(const StatisticsManager & Manager, Json::Value & a_Out)
	{
		if (Manager.Custom.empty())
		{
			// Avoid saving "custom": null to disk:
			return;
		}

		auto & Custom = a_Out["custom"];
		for (const auto & [Statistic, Value] : Manager.Custom)
		{
			Custom[NamespaceSerializer::From(Statistic).data()] = Value;
		}
	}





	static void LoadLegacyFromJSON(StatisticsManager & Manager, const Json::Value & In)
	{
		// Upgrade mapping from pre-1.13 names. TODO: remove on 2020-09-18
		static const std::unordered_map<std::string_view, CustomStatistic> LegacyMapping
		{
			{ "achievement.openInventory", CustomStatistic::AchOpenInventory },
			{ "achievement.mineWood", CustomStatistic::AchMineWood },
			{ "achievement.buildWorkBench", CustomStatistic::AchBuildWorkBench },
			{ "achievement.buildPickaxe", CustomStatistic::AchBuildPickaxe },
			{ "achievement.buildFurnace", CustomStatistic::AchBuildFurnace },
			{ "achievement.acquireIron", CustomStatistic::AchAcquireIron },
			{ "achievement.buildHoe", CustomStatistic::AchBuildHoe },
			{ "achievement.makeBread", CustomStatistic::AchMakeBread },
			{ "achievement.bakeCake", CustomStatistic::AchBakeCake },
			{ "achievement.buildBetterPickaxe", CustomStatistic::AchBuildBetterPickaxe },
			{ "achievement.cookFish", CustomStatistic::AchCookFish },
			{ "achievement.onARail", CustomStatistic::AchOnARail },
			{ "achievement.buildSword", CustomStatistic::AchBuildSword },
			{ "achievement.killEnemy", CustomStatistic::AchKillEnemy },
			{ "achievement.killCow", CustomStatistic::AchKillCow },
			{ "achievement.flyPig", CustomStatistic::AchFlyPig },
			{ "achievement.snipeSkeleton", CustomStatistic::AchSnipeSkeleton },
			{ "achievement.diamonds", CustomStatistic::AchDiamonds },
			{ "achievement.portal", CustomStatistic::AchPortal },
			{ "achievement.ghast", CustomStatistic::AchGhast },
			{ "achievement.blazeRod", CustomStatistic::AchBlazeRod },
			{ "achievement.potion", CustomStatistic::AchPotion },
			{ "achievement.theEnd", CustomStatistic::AchTheEnd },
			{ "achievement.theEnd2", CustomStatistic::AchTheEnd2 },
			{ "achievement.enchantments", CustomStatistic::AchEnchantments },
			{ "achievement.overkill", CustomStatistic::AchOverkill },
			{ "achievement.bookcase", CustomStatistic::AchBookcase },
			{ "achievement.exploreAllBiomes", CustomStatistic::AchExploreAllBiomes },
			{ "achievement.spawnWither", CustomStatistic::AchSpawnWither },
			{ "achievement.killWither", CustomStatistic::AchKillWither },
			{ "achievement.fullBeacon", CustomStatistic::AchFullBeacon },
			{ "achievement.breedCow", CustomStatistic::AchBreedCow },
			{ "achievement.diamondsToYou", CustomStatistic::AchDiamondsToYou },
			{ "stat.animalsBred", CustomStatistic::AnimalsBred },
			{ "stat.boatOneCm", CustomStatistic::BoatOneCm },
			{ "stat.climbOneCm", CustomStatistic::ClimbOneCm },
			{ "stat.crouchOneCm", CustomStatistic::CrouchOneCm },
			{ "stat.damageDealt", CustomStatistic::DamageDealt },
			{ "stat.damageTaken", CustomStatistic::DamageTaken },
			{ "stat.deaths", CustomStatistic::Deaths },
			{ "stat.drop", CustomStatistic::Drop },
			{ "stat.fallOneCm", CustomStatistic::FallOneCm },
			{ "stat.fishCaught", CustomStatistic::FishCaught },
			{ "stat.flyOneCm", CustomStatistic::FlyOneCm },
			{ "stat.horseOneCm", CustomStatistic::HorseOneCm },
			{ "stat.jump", CustomStatistic::Jump },
			{ "stat.leaveGame", CustomStatistic::LeaveGame },
			{ "stat.minecartOneCm", CustomStatistic::MinecartOneCm },
			{ "stat.mobKills", CustomStatistic::MobKills },
			{ "stat.pigOneCm", CustomStatistic::PigOneCm },
			{ "stat.playerKills", CustomStatistic::PlayerKills },
			{ "stat.playOneMinute", CustomStatistic::PlayOneMinute },
			{ "stat.sprintOneCm", CustomStatistic::SprintOneCm },
			{ "stat.swimOneCm", CustomStatistic::SwimOneCm },
			{ "stat.talkedToVillager", CustomStatistic::TalkedToVillager },
			{ "stat.timeSinceDeath", CustomStatistic::TimeSinceDeath },
			{ "stat.tradedWithVillager", CustomStatistic::TradedWithVillager },
			{ "stat.walkOneCm", CustomStatistic::WalkOneCm },
			{ "stat.diveOneCm", CustomStatistic::WalkUnderWaterOneCm },
			{ "stat.armorCleaned", CustomStatistic::CleanArmor },
			{ "stat.bannerCleaned", CustomStatistic::CleanBanner },
			{ "stat.cakeSlicesEaten", CustomStatistic::EatCakeSlice },
			{ "stat.itemEnchanted", CustomStatistic::EnchantItem },
			{ "stat.cauldronFilled", CustomStatistic::FillCauldron },
			{ "stat.dispenserInspected", CustomStatistic::InspectDispenser },
			{ "stat.dropperInspected", CustomStatistic::InspectDropper },
			{ "stat.hopperInspected", CustomStatistic::InspectHopper },
			{ "stat.beaconInteraction", CustomStatistic::InteractWithBeacon },
			{ "stat.brewingstandInteraction", CustomStatistic::InteractWithBrewingstand },
			{ "stat.craftingTableInteraction", CustomStatistic::InteractWithCraftingTable },
			{ "stat.furnaceInteraction", CustomStatistic::InteractWithFurnace },
			{ "stat.chestOpened", CustomStatistic::OpenChest },
			{ "stat.enderchestOpened", CustomStatistic::OpenEnderchest },
			{ "stat.noteblockPlayed", CustomStatistic::PlayNoteblock },
			{ "stat.recordPlayed", CustomStatistic::PlayRecord },
			{ "stat.flowerPotted", CustomStatistic::PotFlower },
			{ "stat.trappedChestTriggered", CustomStatistic::TriggerTrappedChest },
			{ "stat.noteblockTuned", CustomStatistic::TuneNoteblock },
			{ "stat.cauldronUsed", CustomStatistic::UseCauldron },
			{ "stat.aviateOneCm", CustomStatistic::AviateOneCm },
			{ "stat.sleepInBed", CustomStatistic::SleepInBed },
			{ "stat.sneakTime", CustomStatistic::SneakTime }
		};

		for (auto Entry = In.begin(); Entry != In.end(); ++Entry)
		{
			const auto & Key = Entry.key().asString();
			const auto FindResult = LegacyMapping.find(Key);

			if ((FindResult != LegacyMapping.end()) && Entry->isInt())
			{
				Manager.Custom[FindResult->second] = Entry->asUInt();
			}
		}
	}





	static void LoadCustomStatFromJSON(StatisticsManager & Manager, const Json::Value & a_In)
	{
		for (auto it = a_In.begin(); it != a_In.end(); ++it)
		{
			const auto & Key = it.key().asString();
			const auto StatInfo = NamespaceSerializer::SplitNamespacedID(Key);
			if (StatInfo.first == NamespaceSerializer::Namespace::Unknown)
			{
				// Ignore non-Vanilla, non-Cuberite namespaces for now:
				continue;
			}

			const auto & StatName = StatInfo.second;
			try
			{
				Manager.Custom[NamespaceSerializer::ToCustomStatistic(StatName)] = it->asUInt();
			}
			catch (const std::out_of_range &)
			{
				FLOGWARNING("Invalid statistic type \"{}\"", StatName);
			}
			catch (const Json::LogicError &)
			{
				FLOGWARNING("Invalid statistic value for type \"{}\"", StatName);
			}
		}
	}





	void Load(StatisticsManager & Manager, const std::string & WorldPath, std::string && FileName)
	{
		Json::Value Root;
		InputFileStream(MakeStatisticsDirectory(WorldPath, std::move(FileName))) >> Root;

		LoadLegacyFromJSON(Manager, Root);
		LoadCustomStatFromJSON(Manager, Root["stats"]["custom"]);
	}





	void Save(const StatisticsManager & Manager, const std::string & WorldPath, std::string && FileName)
	{
		Json::Value Root;

		SaveStatToJSON(Manager, Root["stats"]);
		Root["DataVersion"] = NamespaceSerializer::DataVersion();

		OutputFileStream(MakeStatisticsDirectory(WorldPath, std::move(FileName))) << Root;
	}
}
