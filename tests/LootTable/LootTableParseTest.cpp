
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
   			"\"\": \"\","
		"}";

	JsonUtils::ParseString(ToolString, JsonObject, & ErrorMessage);

	auto Condition = LootTable::ParseCondition(JsonObject);

	auto Tool = std::get<LootTable::Condition::cMatchTool>(Condition.m_Parameter);

	TEST_TRUE(Tool.IsActive());
	TEST_EQUAL(Tool.m_CountMin, 6);
	TEST_EQUAL(Tool.m_CountMax, 7);
	TEST_EQUAL(Tool.m_DurabilityMin, 8);
	TEST_EQUAL(Tool.m_DurabilityMax, 9);
}





static void ParseRandomChance()
{

}





static void ParseRandomChanceWithLooting()
{

}





static void ParseReference()
{

}





static void ParseSurvivesExplosion()
{

}





static void ParseTableBonus()
{

}





static void ParseTimeCheck()
{

}





static void ParseWeatherCheck()
{

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
})
