
// LootTableTest.cpp

#include "Globals.h"

#include "../TestHelpers.h"
#include "json/json.h"
#include "JsonUtils.h"

#define private public  // Yes - bad but does the job

#include "LootTableParser.h"

Json::Value JsonObject;
AString ErrorMessage;

static void ParseRollsSuccessful()
{
	AString SuccessfulRolls = "{\"rolls\": 1}";
	JsonUtils::ParseString(SuccessfulRolls, JsonObject, & ErrorMessage);

	auto Roll = LootTable::ParseRolls(JsonObject["rolls"]);

	TEST_EQUAL(Roll.m_RollsMin, 1);
	TEST_EQUAL(Roll.m_RollsMax, 1);

	AString SuccessfulRollRange = "{\"rolls\": { \"min\": 2.0, \"max\": 4.0 }}";
	JsonUtils::ParseString(SuccessfulRollRange, JsonObject, & ErrorMessage);

	auto RangeRoll = LootTable::ParseRolls(JsonObject["rolls"]);

	TEST_EQUAL(RangeRoll.m_RollsMin, 2);
	TEST_EQUAL(RangeRoll.m_RollsMax, 4);
}





static void ParseRollMinOnly()
{
	AString MinOnly = "{\"rolls\": { \"min\": 2.0 }}";
	JsonUtils::ParseString(MinOnly, JsonObject, & ErrorMessage);

	auto Rolls = LootTable::ParseRolls(JsonObject["rolls"]);

	TEST_EQUAL(Rolls.m_RollsMin, 2);
	TEST_EQUAL(Rolls.m_RollsMax, 2);
}





static void ParseRollMaxOnly()
{
	AString MinOnly = "{\"rolls\": { \"max\": 2.0 }}";
	JsonUtils::ParseString(MinOnly, JsonObject, & ErrorMessage);

	auto Rolls = LootTable::ParseRolls(JsonObject["rolls"]);

	TEST_EQUAL(Rolls.m_RollsMin, 0);
	TEST_EQUAL(Rolls.m_RollsMax, 2);
}





static void ParseAlternative()
{
	AString AlternativeString = "{"
							 	"\"condition\": \"minecraft:alternative\","
							 	"\"Terms\": ["
									"{\"condition\": \"minecraft:survives_explosion\"},"
								"],"
							 	"}";
	JsonUtils::ParseString(AlternativeString , JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto Alternative = std::get<LootTable::Condition::cAlternative>(Condition.m_Parameter);

	TEST_TRUE(Alternative.IsActive());
}





static void ParseBlockStateProperty()
{
	AString BlockStateString =
		"{"
  		"\"condition\": \"minecraft:block_state_property\","
		"\"block\": \"minecraft:stone\","
   		"}";
	JsonUtils::ParseString(BlockStateString , JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto BlockState = std::get<LootTable::Condition::cBlockStateProperty>(Condition.m_Parameter);

	TEST_TRUE(BlockState.IsActive());
	TEST_NOTEQUAL(BlockState.m_Block, 0);
}





static void ParseDamageSourceProperties()
{
	AString BlockStateString =
			"{"
			"\"condition\": \"minecraft:damage_source_properties\","
			"\"predicate\": {"
   				"\"bypasses_armor\": true,"
   				"\"bypasses_invulnerability\": true,"
   				"\"bypasses_magic\": true,"
   				"\"is_explosion\": true,"
   				"\"is_fire\": true,"
   				"\"is_magic\": true,"
   				"\"is_projectile\": true,"
   				"\"is_lightning\": true,"
			"},"
			"}";
	JsonUtils::ParseString(BlockStateString , JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto DamageSource = std::get<LootTable::Condition::cDamageSourceProperties>(Condition.m_Parameter);

	TEST_TRUE(DamageSource.IsActive());
	TEST_TRUE(DamageSource.m_BypassesArmor);
	TEST_TRUE(DamageSource.m_BypassesInvulnerability);
	TEST_TRUE(DamageSource.m_BypassesMagic);
	TEST_TRUE(DamageSource.m_IsExplosion);
	TEST_TRUE(DamageSource.m_IsFire);
	TEST_TRUE(DamageSource.m_IsMagic);
	TEST_TRUE(DamageSource.m_IsProjectile);
	TEST_TRUE(DamageSource.m_IsLightning);
}





static void ParseEntityProperties()
{
	AString EntityPropertiesCondition =
			"{"
			"\"condition\": \"minecraft:entity_properties\","
			"\"predicate\": {"
				"\"distance\": {"
					"\"absolute\": {"
						"\"min\": 1.0,"
						"\"max\": 3.0,"
					"},"
					"\"horizontal\": {"
						"\"min\": 2.0,"
						"\"max\": 4.0,"
					"},"
					"\"x\": {"
						"\"min\": 3.0,"
						"\"max\": 5.0,"
					"},"
					"\"y\": {"
						"\"min\": 4.0,"
						"\"max\": 6.0,"
					"},"
					"\"z\": {"
						"\"min\": 5.0,"
						"\"max\": 7.0,"
					"},"
				"},"
				"\"effects\": {"
					"\"minecraft:saturation\": {"
						"\"amplifier\": {"
							"\"min\": 1.0,"
							"\"max\": 2.0,"
						"},"
						"\"duration\": {"
							"\"min\": 3.0,"
							"\"max\": 4.0,"
						"},"
					"},"
				"},"
				"\"flags\": {"
					"\"is_on_fire\": true,"
					"\"is_sneaking\": true,"
					"\"is_sprinting\": true,"
					"\"is_swimming\": true,"
					"\"is_baby\": true,"
				"},"
				"\"player\": {"
					"\"gamemode\": \"survival\","
					"\"level\": {"
						"\"min\": 1.0,"
						"\"max\": 2.0,"
					"},"
				"},"
				"\"team\": \"I like 42\","
				"\"nbt\": \"Hello World\","
   			"},"
			"\"entity\": \"this\""
			"}";
	JsonUtils::ParseString(EntityPropertiesCondition , JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto EntityProperties = std::get<LootTable::Condition::cEntityProperties>(Condition.m_Parameter);

	TEST_TRUE(EntityProperties.IsActive());

	TEST_EQUAL(EntityProperties.m_AbsoluteMin, 1.0f);
	TEST_EQUAL(EntityProperties.m_AbsoluteMax, 3.0f);

	TEST_EQUAL(EntityProperties.m_HorizontalMin, 2.0f);
	TEST_EQUAL(EntityProperties.m_HorizontalMax, 4.0f);

	TEST_EQUAL(EntityProperties.m_XMin, 3.0f);
	TEST_EQUAL(EntityProperties.m_XMax, 5.0f);

	TEST_EQUAL(EntityProperties.m_YMin, 4.0f);
	TEST_EQUAL(EntityProperties.m_YMax, 6.0f);

	TEST_EQUAL(EntityProperties.m_ZMin, 5.0f);
	TEST_EQUAL(EntityProperties.m_ZMax, 7.0f);

	for (const auto & Effect : EntityProperties.m_Effects)
	{
		TEST_EQUAL(Effect.first, cEntityEffect::effSaturation);
		TEST_EQUAL(Effect.second.m_AmplifierMin, 1);
		TEST_EQUAL(Effect.second.m_AmplifierMax, 2);
		TEST_EQUAL(Effect.second.m_DurationMin, 3);
		TEST_EQUAL(Effect.second.m_DurationMax, 4);
	}

	TEST_TRUE(EntityProperties.m_IsOnFire);
	TEST_TRUE(EntityProperties.m_IsSneaking);
	TEST_TRUE(EntityProperties.m_IsSprinting);
	TEST_TRUE(EntityProperties.m_IsSwimming);
	TEST_TRUE(EntityProperties.m_IsBaby);

	TEST_EQUAL(EntityProperties.m_NBT, "Hello World");

	TEST_TRUE(EntityProperties.m_Player);
	TEST_EQUAL(EntityProperties.m_Gamemode, gmSurvival);
	TEST_EQUAL(EntityProperties.m_LevelMin, 1);
	TEST_EQUAL(EntityProperties.m_LevelMax, 2);

	TEST_EQUAL(EntityProperties.m_Team, "I like 42");

	TEST_EQUAL(EntityProperties.m_Dest, LootTable::Condition::cEntityProperties::eDest::This);

	// TODO: Type
}




static void ParseEntityScores()
{
	AString EntityScoreString =
			"{"
			"\"condition\": \"minecraft:entity_scores\","
			"}";
	JsonUtils::ParseString(EntityScoreString , JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto EntityScore = std::get<LootTable::Condition::cEntityScores>(Condition.m_Parameter);

	TEST_FALSE(EntityScore.IsActive());

	// TODO
}





static void ParseInverted()
{
	AString InvertedString =
			"{"
				"\"condition\": \"minecraft:inverted\","
				"\"term\": {"
					"\"condition\": \"minecraft:survives_explosion\","
				"},"
			"}";
	JsonUtils::ParseString(InvertedString , JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto Inverted = std::get<LootTable::Condition::cInverted>(Condition.m_Parameter);

	TEST_TRUE(Inverted.IsActive());
}





static void ParseKilledByPlayer()
{
	AString InvertedString =
			"{"
			"\"condition\": \"minecraft:killed_by_player\","
			"\"inverse\": true,"
			"}";
	JsonUtils::ParseString(InvertedString , JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto KilledByPlayer = std::get<LootTable::Condition::cKilledByPlayer>(Condition.m_Parameter);

	TEST_TRUE(KilledByPlayer.IsActive());
	TEST_TRUE(KilledByPlayer.m_Inverse);

}





static void ParseLocationCheck()
{
	AString LocationString =
		"{"
			"\"condition\": \"minecraft:location_check\","
			"\"offsetX\": 2.0,"
			"\"offsetY\": 3.0,"
			"\"offsetZ\": 4.0,"
			"\"predicate\": {"
				"\"biome\": \"minecraft:jungle\","
				"\"block\": {"
					"\"block\": {"
						"\"block\": \"minecraft:iron_ingot\","
					"},"
	   				"\"tag\": \"minecraft:wool\","
					"\"nbt\": \"HelloWorld\","
	 			"},"
	 			"\"dimension\": \"the_nether\","
	 			"\"feature\": \"lalala i'm bored\","
				"\"fluid\": {"
					"\"fluid\": \"minecraft:water\","
					"\"tag\": \"Test123\","
				"},"
				"\"light\": {"
					"\"min\": 5.0,"
					"\"max\": 6.0,"
				"},"
				"\"position\": {"
					"\"x\": {"
						"\"min\": 7.0,"
						"\"max\": 8.0,"
					"},"
					"\"y\": {"
						"\"min\": 9.0,"
						"\"max\": 10.0,"
					"},"
					"\"z\": {"
						"\"min\": 11.0,"
						"\"max\": 12.0,"
					"},"
				"},"
	 			"\"smokey\": true,"
			"}"
		"}";

	JsonUtils::ParseString(LocationString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto Location = std::get<LootTable::Condition::cLocationCheck>(Condition.m_Parameter);

	TEST_TRUE(Location.IsActive());
	TEST_EQUAL(Location.m_OffsetX, 2);
	TEST_EQUAL(Location.m_OffsetY, 3);
	TEST_EQUAL(Location.m_OffsetZ, 4);
	TEST_EQUAL(Location.m_Biome, biJungle);
	TEST_EQUAL(Location.m_BlockTag, ItemTag::eItemTags::Wool);
	TEST_TRUE(Location.m_BlockState.IsActive());
	TEST_EQUAL(Location.m_BlockNBT, "HelloWorld");
	TEST_EQUAL(Location.m_Dimension, dimNether);
	TEST_EQUAL(Location.m_Feature, "lalala i'm bored");
	TEST_TRUE(Location.m_FluidState.IsActive());
	TEST_EQUAL(Location.m_FluidTag, "Test123");
	TEST_EQUAL(Location.m_LightMin, 5);
	TEST_EQUAL(Location.m_LightMax, 6);
	TEST_EQUAL(Location.m_XMin, 7);
	TEST_EQUAL(Location.m_XMax, 8);
	TEST_EQUAL(Location.m_YMin, 9);
	TEST_EQUAL(Location.m_YMax, 10);
	TEST_EQUAL(Location.m_ZMin, 11);
	TEST_EQUAL(Location.m_ZMax, 12);
	TEST_TRUE(Location.m_Smokey);
}





static void ParseMatchTool()
{
	AString ToolString =
	"{"
		"\"condition\": \"minecraft:match_tool\","
		"\"predicate\": {"
			"\"count\": {"
				"\"min\": 6.0,"
				"\"max\": 7.0,"
			"},"
			"\"durability\": {"
				"\"min\": 8.0,"
				"\"max\": 9.0,"
			"},"
		"},"
	"}";

	JsonUtils::ParseString(ToolString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto Tool = std::get<LootTable::Condition::cMatchTool>(Condition.m_Parameter);

	TEST_TRUE(Tool.IsActive());
	TEST_EQUAL(Tool.m_CountMin, 6);
	TEST_EQUAL(Tool.m_CountMax, 7);
	TEST_EQUAL(Tool.m_DurabilityMin, 8);
	TEST_EQUAL(Tool.m_DurabilityMax, 9);

	// TODO
}





static void ParseRandomChance()
{
	AString RandomChanceString =
	"{"
		"\"condition\": \"minecraft:random_chance\","
		"\"chance\": 0.3,"
	"}";

	JsonUtils::ParseString(RandomChanceString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto RandomChance = std::get<LootTable::Condition::cRandomChance>(Condition.m_Parameter);

	TEST_TRUE(RandomChance.IsActive());
	TEST_EQUAL(RandomChance.m_Chance, 0.3f);
}





static void ParseRandomChanceWithLooting()
{
	AString RandomChanceLootingString =
	"{"
		"\"condition\": \"minecraft:random_chance_with_looting\","
		"\"chance\": 0.3,"
		"\"looting_multiplier\": 0.5,"
	"}";

	JsonUtils::ParseString(RandomChanceLootingString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto RandomChanceLooting = std::get<LootTable::Condition::cRandomChanceWithLooting>(Condition.m_Parameter);

	TEST_TRUE(RandomChanceLooting.IsActive());
	TEST_LESS_THAN_OR_EQUAL(RandomChanceLooting.m_Chance - 0.300000, 0.0001);
	TEST_LESS_THAN_OR_EQUAL(RandomChanceLooting.m_LootingMultiplier - 0.500000, 0.0001);
}





static void ParseReference()
{
	AString ReferenceString =
	"{"
	"\"condition\": \"minecraft:reference\","
	"}";

	JsonUtils::ParseString(ReferenceString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto Reference = std::get<LootTable::Condition::cReference>(Condition.m_Parameter);

	TEST_TRUE(Reference.IsActive());
}





static void ParseSurvivesExplosion()
{
	AString SurvivesExplosionString =
	"{"
		"\"condition\": \"minecraft:survives_explosion\","
	"}";

	JsonUtils::ParseString(SurvivesExplosionString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto SurvivesExplosion = std::get<LootTable::Condition::cSurvivesExplosion>(Condition.m_Parameter);

	TEST_TRUE(SurvivesExplosion.IsActive());
}





static void ParseTableBonus()
{
	AString LootTableBonusString =
	"{"
		"\"condition\": \"minecraft:table_bonus\","
  		"\"enchantment\": 3,"
		"\"chances\": ["
			"0, 0.1,"
			"0, 0.2,"
			"0, 0.3,"
		"],"
	"}";

	JsonUtils::ParseString(LootTableBonusString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto TableBonus = std::get<LootTable::Condition::cTableBonus>(Condition.m_Parameter);

	TEST_TRUE(TableBonus.IsActive());
	TEST_EQUAL(TableBonus.m_Enchantment, 3);
	TEST_LESS_THAN_OR_EQUAL(TableBonus.m_Chances[0] - 0.1, 0.0000001);
	TEST_LESS_THAN_OR_EQUAL(TableBonus.m_Chances[1] - 0.2, 0.0000001);
	TEST_LESS_THAN_OR_EQUAL(TableBonus.m_Chances[2] - 0.3, 0.0000001);
}





static void ParseTimeCheck()
{
	AString TimeString =
	"{"
		"\"condition\": \"minecraft:time_check\","
		"\"value\": {"
			"\"min\": 100,"
			"\"max\": 1000,"
  		"},"
		"\"period\": 10000"
	"}";

	JsonUtils::ParseString(TimeString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto Time = std::get<LootTable::Condition::cTimeCheck>(Condition.m_Parameter);

	TEST_TRUE(Time.IsActive());
	TEST_EQUAL(Time.m_Min, 100);
	TEST_EQUAL(Time.m_Max, 1000);
	TEST_EQUAL(Time.m_Period, 10000);
}





static void ParseWeatherCheck()
{
	AString WeatherString =
	"{"
		"\"condition\": \"minecraft:weather_check\","
		"\"raining\": true,"
		"\"thundering\": true,"
	"}";

	JsonUtils::ParseString(WeatherString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto Weather = std::get<LootTable::Condition::cWeatherCheck>(Condition.m_Parameter);

	TEST_TRUE(Weather.IsActive());
	TEST_TRUE(Weather.m_Raining);
	TEST_TRUE(Weather.m_Thundering);
}





static void ParseApplyBonus()
{
	// Mode: BinomialWithBonusCount
	AString BonusString =
	"{"
		"\"function\": \"minecraft:apply_bonus\","
		"\"enchantment\": 2,"
		"\"formula\": \"binomial_with_bonus_count\","
		"\"parameters\": {"
			"\"extra\": 3,"
			"\"probability\": 0.3,"
		"},"
	"}";

	JsonUtils::ParseString(BonusString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto ApplyBonus = std::get<LootTable::Function::cApplyBonus>(Function.m_Function);

	TEST_TRUE(ApplyBonus.IsActive());
	TEST_EQUAL(ApplyBonus.m_Formula, LootTable::Function::cApplyBonus::eFormula::BinomialWithBonusCount);
	TEST_EQUAL(ApplyBonus.m_Enchantment, 2);
	TEST_EQUAL(ApplyBonus.m_Extra, 3);
	TEST_LESS_THAN_OR_EQUAL(ApplyBonus.m_Probability - 0.3, 0.0000001);

	// Mode: UniformBonusCount
	BonusString =
	"{"
		"\"function\": \"minecraft:apply_bonus\","
		"\"enchantment\": 10,"
		"\"formula\": \"uniform_bonus_count\","
		"\"parameters\": {"
			"\"bonusMultiplier\": 0.5,"
		"},"
	"}";

	JsonUtils::ParseString(BonusString, JsonObject, & ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	ApplyBonus = std::get<LootTable::Function::cApplyBonus>(Function.m_Function);

	TEST_TRUE(ApplyBonus.IsActive());
	TEST_EQUAL(ApplyBonus.m_Formula, LootTable::Function::cApplyBonus::eFormula::UniformBonusCount);
	TEST_EQUAL(ApplyBonus.m_Enchantment, 10);
	TEST_LESS_THAN_OR_EQUAL(ApplyBonus.m_BonusMultiplier - 0.5, 0.0000001);

	//
	BonusString =
	"{"
		"\"function\": \"minecraft:apply_bonus\","
		"\"enchantment\": 15,"
		"\"formula\": \"ore_drops\","
	"}";

	JsonUtils::ParseString(BonusString, JsonObject, & ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	ApplyBonus = std::get<LootTable::Function::cApplyBonus>(Function.m_Function);

	TEST_TRUE(ApplyBonus.IsActive());
	TEST_EQUAL(ApplyBonus.m_Formula, LootTable::Function::cApplyBonus::eFormula::OreDrops);
	TEST_EQUAL(ApplyBonus.m_Enchantment, 15);
}





static void ParseCopyName()
{
	AString CopyNameString =
	"{"
		"\"function\": \"minecraft:copy_name\","
		"\"source\": \"block_entity\","
	"}";

	JsonUtils::ParseString(CopyNameString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto CopyName = std::get<LootTable::Function::cCopyName>(Function.m_Function);

	TEST_TRUE(CopyName.IsActive());
}





static void ParseCopyNbt()
{
	AString CopyNBTString =
	"{"
		"\"function\": \"minecraft:copy_nbt\","
	"}";

	JsonUtils::ParseString(CopyNBTString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto CopyNBT = std::get<LootTable::Function::cCopyNbt>(Function.m_Function);

	// TODO: 13.10.2020 - Add test when implemented - 12xx12
}





static void ParseCopyState()
{
	AString CopyStateString =
	"{"
		"\"function\": \"minecraft:copy_state\","
	"}";

	JsonUtils::ParseString(CopyStateString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto CopyState = std::get<LootTable::Function::cCopyState>(Function.m_Function);

	// TODO: 13.10.2020 - Add test when implemented - 12xx12
}





static void ParseEnchantRandomly()
{
	AString EnchantRandomlyString =
	"{"
		"\"function\": \"minecraft:enchant_randomly\","
		"\"enchantments\": ["
			"\"minecraft:efficiency\","
			"\"minecraft:smite\","
		"],"
	"}";

	JsonUtils::ParseString(EnchantRandomlyString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto EnchantRandomly = std::get<LootTable::Function::cEnchantRandomly>(Function.m_Function);

	TEST_TRUE(EnchantRandomly.IsActive());
	TEST_EQUAL(EnchantRandomly.m_Enchantments[0], cEnchantments::enchEfficiency);
	TEST_EQUAL(EnchantRandomly.m_Enchantments[1], cEnchantments::enchSmite);
}





static void ParseEnchantWithLevels()
{
	AString EnchantWithLevelsString =
	"{"
		"\"function\": \"minecraft:enchant_with_levels\","
		"\"treasure\": true,"
		"\"levels\": {"
			"\"min\": 1,"
			"\"max\": 3,"
		"},"
	"}";

	JsonUtils::ParseString(EnchantWithLevelsString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto EnchantWithLevels = std::get<LootTable::Function::cEnchantWithLevels>(Function.m_Function);

	TEST_TRUE(EnchantWithLevels.IsActive());
	TEST_TRUE(EnchantWithLevels.m_Treasure);
	TEST_EQUAL(EnchantWithLevels.m_LevelsMin, 1);
	TEST_EQUAL(EnchantWithLevels.m_LevelsMax, 3);
}





static void ParseExplorationMap()
{
	AString ExplorationMapString =
	"{"
		"\"function\": \"minecraft:exploration_map\","
		"\"destination\": \"Destination is unknown!\","
		"\"decoration\": \"Decoration looks nice\","
		"\"zoom\": 10,"
		"\"search_radius\": 20,"
		"\"skip_existing_chunks\": true,"
	"}";

	JsonUtils::ParseString(ExplorationMapString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto ExplorationMap = std::get<LootTable::Function::cExplorationMap>(Function.m_Function);

	// TODO: 13.10.2020 - Add active test when implemented - 12xx12
	TEST_EQUAL(ExplorationMap.m_Destination, "DestinationIsUnknown!");
	TEST_EQUAL(ExplorationMap.m_Decoration, "DecorationLooksNice");
	TEST_EQUAL(ExplorationMap.m_Zoom, 10);
	TEST_EQUAL(ExplorationMap.m_SearchRadius, 20);
	TEST_TRUE(ExplorationMap.m_SkipExistingChunks);
}





static void ParseExplosionDecay()
{
	AString ExplosionDecayString =
	"{"
		"\"function\": \"minecraft:explosion_decay\","
	"}";

	JsonUtils::ParseString(ExplosionDecayString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto ExplosionDecay = std::get<LootTable::Function::cExplosionDecay>(Function.m_Function);

	TEST_TRUE(ExplosionDecay.IsActive());
}





static void ParseFurnaceSmelt()
{
	AString FurnaceSmeltString =
	"{"
		"\"function\": \"minecraft:furnace_smelt\","
	"}";

	JsonUtils::ParseString(FurnaceSmeltString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto FurnaceSmelt = std::get<LootTable::Function::cFurnaceSmelt>(Function.m_Function);

	TEST_TRUE(FurnaceSmelt.IsActive());
}





static void ParseFillPlayerHead()
{
	// Dest = this
	AString FillPlayerHeadString =
	"{"
		"\"function\": \"minecraft:fill_player_head\","
		"\"entity\": \"this\""
	"}";

	JsonUtils::ParseString(FillPlayerHeadString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto FillPlayerHead = std::get<LootTable::Function::cFillPlayerHead>(Function.m_Function);

	TEST_TRUE(FillPlayerHead.IsActive());
	TEST_EQUAL(FillPlayerHead.m_Dest, LootTable::Function::cFillPlayerHead::eDest::This);

	// Dest = killer
	FillPlayerHeadString =
	"{"
		"\"function\": \"minecraft:fill_player_head\","
		"\"entity\": \"killer\""
	"}";

	JsonUtils::ParseString(FillPlayerHeadString, JsonObject, & ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	FillPlayerHead = std::get<LootTable::Function::cFillPlayerHead>(Function.m_Function);

	TEST_TRUE(FillPlayerHead.IsActive());
	TEST_EQUAL(FillPlayerHead.m_Dest, LootTable::Function::cFillPlayerHead::eDest::Killer);

	// Dest = killer_player
	FillPlayerHeadString =
	"{"
		"\"function\": \"minecraft:fill_player_head\","
		"\"entity\": \"killer_player\""
	"}";

	JsonUtils::ParseString(FillPlayerHeadString, JsonObject, & ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	FillPlayerHead = std::get<LootTable::Function::cFillPlayerHead>(Function.m_Function);

	TEST_TRUE(FillPlayerHead.IsActive());
	TEST_EQUAL(FillPlayerHead.m_Dest, LootTable::Function::cFillPlayerHead::eDest::KillerPlayer);

}





static void ParseLimitCount()
{
	AString FillPlayerHeadString =
	"{"
		"\"function\": \"minecraft:limit_count\","
		"\"limit\": {"
			"\"min\": 10,"
			"\"max\": 20,"
		"},"
	"}";

	JsonUtils::ParseString(FillPlayerHeadString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto LimitCount = std::get<LootTable::Function::cLimitCount>(Function.m_Function);

	TEST_TRUE(LimitCount.IsActive());
	TEST_EQUAL(LimitCount.m_LimitMin, 10);
	TEST_EQUAL(LimitCount.m_LimitMax, 20);
}





static void ParseLootingEnchant()
{
	AString FillPlayerHeadString =
	"{"
		"\"function\": \"minecraft:looting_enchant\","
		"\"count\": {"
  			"\"min\": 10,"
  			"\"max\": 20,"
		"},"
  		"\"limit\": 30,"
	"}";

	JsonUtils::ParseString(FillPlayerHeadString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto LootingEnchant = std::get<LootTable::Function::cLootingEnchant>(Function.m_Function);

	TEST_TRUE(LootingEnchant.IsActive());
	TEST_EQUAL(LootingEnchant.m_CountMin, 10);
	TEST_EQUAL(LootingEnchant.m_CountMax, 20);
	TEST_EQUAL(LootingEnchant.m_Limit, 30);
}





static void ParseSetAttributes()
{
	AString SetAttributesString =
	"{"
		"\"function\": \"minecraft:set_attributes\","
	"}";

	JsonUtils::ParseString(SetAttributesString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto SetAttributes = std::get<LootTable::Function::cSetAttributes>(Function.m_Function);

	// TODO: 13.10.2020 - Add test when implemented - 12xx12
}





static void ParseSetContents()
{
	AString SetContentsString =
	"{"
		"\"function\": \"minecraft:set_contents\","
	"}";

	JsonUtils::ParseString(SetContentsString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto SetContents = std::get<LootTable::Function::cSetContents>(Function.m_Function);

	// TODO: 13.10.2020 - Add test when implemented - 12xx12
}





static void ParseSetCount()
{
	// Static limit
	AString SetCountString =
	"{"
		"\"function\": \"minecraft:set_count\","
  		"\"count\": 10,"
	"}";

	JsonUtils::ParseString(SetCountString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto SetCount = std::get<LootTable::Function::cSetCount>(Function.m_Function);

	TEST_TRUE(SetCount.IsActive());
	TEST_EQUAL(SetCount.m_Count, 10);

	// Uniform limit
	SetCountString =
	"{"
		"\"function\": \"minecraft:set_count\","
  		"\"count\": {"
			"\"type\": \"uniform\","
			"\"min\": 10,"
			"\"max\": 20,"
		"},"
	"}";

	JsonUtils::ParseString(SetCountString, JsonObject, & ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	SetCount = std::get<LootTable::Function::cSetCount>(Function.m_Function);

	TEST_TRUE(SetCount.IsActive());
	TEST_EQUAL(SetCount.m_Type, LootTable::Function::cSetCount::eType::Uniform);
	TEST_EQUAL(SetCount.m_UniformMin, 10);
	TEST_EQUAL(SetCount.m_UniformMax, 20);

	// Binomial limit
	SetCountString =
	"{"
		"\"function\": \"minecraft:set_count\","
  		"\"count\": {"
			"\"type\": \"binomial\","
			"\"n\": 10,"
			"\"p\": 0.5,"
		"},"
	"}";

	JsonUtils::ParseString(SetCountString, JsonObject, & ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	SetCount = std::get<LootTable::Function::cSetCount>(Function.m_Function);

	TEST_TRUE(SetCount.IsActive());
	TEST_EQUAL(SetCount.m_Type, LootTable::Function::cSetCount::eType::Binomial);
	TEST_EQUAL(SetCount.m_N, 10);
	TEST_EQUAL(SetCount.m_P, 0.5f);
}





static void ParseSetDamage()
{
	AString SetDamageString =
	"{"
		"\"function\": \"minecraft:set_damage\","
		"\"damage\": {"
			"\"min\": 0.2,"
			"\"max\": 0.8,"
		"},"
	"}";

	JsonUtils::ParseString(SetDamageString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto SetDamage = std::get<LootTable::Function::cSetDamage>(Function.m_Function);

	TEST_TRUE(SetDamage.IsActive());
	TEST_EQUAL(SetDamage.m_Min, 0.2f);
	TEST_EQUAL(SetDamage.m_Max, 0.8f);
}





static void ParseSetLootTable()
{
	AString SetLootTableString =
	"{"
		"\"function\": \"minecraft:set_loot_table\","
		"\"name\": \"Hello World!\","
		"\"Seed\": 42,"
	"}";

	JsonUtils::ParseString(SetLootTableString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto SetLootTable = std::get<LootTable::Function::cSetLootTable>(Function.m_Function);

	// TODO: 13.10.2020 - Add active test when implemented - 12xx12
	TEST_EQUAL(SetLootTable.m_LootTable, "HelloWorld!");
	TEST_EQUAL(SetLootTable.m_Seed, 42);
}





static void ParseSetLore()
{
	// This
	AString SetLoreString =
	"{"
		"\"function\": \"minecraft:set_lore\","
		"\"lore\": [\"Hello World!\", \"Hello World!2\"],"
		"\"entity\": \"this\","
		"\"replace\": true,"
	"}";

	JsonUtils::ParseString(SetLoreString, JsonObject, &ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto SetLore = std::get<LootTable::Function::cSetLore>(Function.m_Function);

	TEST_TRUE(SetLore.IsActive());
	TEST_EQUAL(SetLore.m_Lore[0], "Hello World!");
	TEST_EQUAL(SetLore.m_Lore[1], "Hello World!2");
	TEST_EQUAL(SetLore.m_Type, LootTable::Function::cSetLore::eType::This);
	TEST_TRUE(SetLore.m_Replace);

	// killer
	SetLoreString =
	"{"
		"\"function\": \"minecraft:set_lore\","
		"\"lore\": [\"Hello World!\", \"Hello World!2\"],"
		"\"entity\": \"killer\","
		"\"replace\": true,"
	"}";

	JsonUtils::ParseString(SetLoreString, JsonObject, &ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	SetLore = std::get<LootTable::Function::cSetLore>(Function.m_Function);

	TEST_TRUE(SetLore.IsActive());
	TEST_EQUAL(SetLore.m_Lore[0], "Hello World!");
	TEST_EQUAL(SetLore.m_Lore[1], "Hello World!2");
	TEST_EQUAL(SetLore.m_Type, LootTable::Function::cSetLore::eType::Killer);
	TEST_TRUE(SetLore.m_Replace);

	// This
	SetLoreString =
	"{"
		"\"function\": \"minecraft:set_lore\","
		"\"lore\": [\"Hello World!\", \"Hello World!2\"],"
		"\"entity\": \"killer_player\","
		"\"replace\": true,"
	"}";

	JsonUtils::ParseString(SetLoreString, JsonObject, &ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	SetLore = std::get<LootTable::Function::cSetLore>(Function.m_Function);

	TEST_TRUE(SetLore.IsActive());
	TEST_EQUAL(SetLore.m_Lore[0], "Hello World!");
	TEST_EQUAL(SetLore.m_Lore[1], "Hello World!2");
	TEST_EQUAL(SetLore.m_Type, LootTable::Function::cSetLore::eType::KillerPlayer);
	TEST_TRUE(SetLore.m_Replace);
}





static void ParseSetName()
{
	// This
	AString SetNameString =
	"{"
		"\"function\": \"minecraft:set_name\","
		"\"name\": \"Hello World!\","
  		"\"entity\": \"this\","
	"}";

	JsonUtils::ParseString(SetNameString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto SetName = std::get<LootTable::Function::cSetName>(Function.m_Function);

	TEST_TRUE(SetName.IsActive());
	TEST_EQUAL(SetName.m_Name, "Hello World!");
	TEST_EQUAL(SetName.m_Type, LootTable::Function::cSetName::eType::This);

	// Killer
	SetNameString =
	"{"
		"\"function\": \"minecraft:set_name\","
		"\"name\": \"Hello World!\","
		"\"entity\": \"killer\","
	"}";

	JsonUtils::ParseString(SetNameString, JsonObject, & ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	SetName = std::get<LootTable::Function::cSetName>(Function.m_Function);

	TEST_TRUE(SetName.IsActive());
	TEST_EQUAL(SetName.m_Name, "Hello World!");
	TEST_EQUAL(SetName.m_Type, LootTable::Function::cSetName::eType::Killer);

	// Killer
	SetNameString =
	"{"
		"\"function\": \"minecraft:set_name\","
		"\"name\": \"Hello World!\","
		"\"entity\": \"killer_player\","
	"}";

	JsonUtils::ParseString(SetNameString, JsonObject, & ErrorMessage);

	Function = LootTable::ParseFunction(JsonObject);

	SetName = std::get<LootTable::Function::cSetName>(Function.m_Function);

	TEST_TRUE(SetName.IsActive());
	TEST_EQUAL(SetName.m_Name, "Hello World!");
	TEST_EQUAL(SetName.m_Type, LootTable::Function::cSetName::eType::KillerPlayer);
}





static void ParseSetNbt()
{
	AString SetNBTString =
	"{"
		"\"function\": \"minecraft:set_nbt\","
		"\"tag\": \"Hello World!\","
	"}";

	JsonUtils::ParseString(SetNBTString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto SetNBT = std::get<LootTable::Function::cSetNbt>(Function.m_Function);

	// TODO: 13.10.2020 - Add active test when implemented - 12xx12
	TEST_EQUAL(SetNBT.m_Tag, "Hello World!");
}





static void ParseSetStewEffect()
{
	AString SetStewEffectString =
	"{"
		"\"function\": \"minecraft:set_stew_effect\","
	"}";

	JsonUtils::ParseString(SetStewEffectString, JsonObject, & ErrorMessage);

	auto Function = LootTable::ParseFunction(JsonObject);

	auto SetStewEffect = std::get<LootTable::Function::cSetStewEffect>(Function.m_Function);

	// TODO: 13.10.2020 - Add test when implemented - 12xx12
}





IMPLEMENT_TEST_MAIN("LootTableParseTest",
{
	ParseRollsSuccessful();
	ParseRollMinOnly();
	ParseRollMaxOnly();

	ParseAlternative();
	ParseBlockStateProperty();
	ParseDamageSourceProperties();
	ParseEntityProperties();
	ParseEntityScores();
	ParseInverted();
	ParseKilledByPlayer();
	ParseLocationCheck();
	ParseMatchTool();
	ParseRandomChance();
	ParseRandomChanceWithLooting();
	ParseReference();
	ParseSurvivesExplosion();
	ParseTableBonus();
	ParseTimeCheck();
	ParseWeatherCheck();

	ParseApplyBonus();
	ParseCopyName();
	ParseCopyNbt();
	ParseCopyState();
	ParseEnchantRandomly();
	ParseEnchantWithLevels();
	ParseExplorationMap();
	ParseExplosionDecay();
	ParseFurnaceSmelt();
	ParseFillPlayerHead();
	ParseLimitCount();
	ParseLootingEnchant();
	ParseSetAttributes();
	ParseSetContents();
	ParseSetCount();
	ParseSetDamage();
	ParseSetLootTable();
	ParseSetLore();
	ParseSetName();
	ParseSetNbt();
	ParseSetStewEffect();
})
