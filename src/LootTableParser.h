
// LootTableParser.h

#pragma once

#include <variant>
#include <Registries/ItemTags.h>
#include "Noise/Noise.h"
#include "Item.h"
#include "Blocks/BlockHandler.h"
#include "Entities/EntityEffect.h"
#include "Entities/Entity.h"
#include "Mobs/MonsterTypes.h"

// fwd:
class cWorld;
enum class Statistic;

// fwd:
namespace Json
{
	class Value;
}

/** This namespace contains all enum, structs, typedefs used in the loot table classes */
namespace LootTable
{
	const AString LootTablePath = "LootTables";




	/** Any available pool entry type as of 1.16.2 */
	enum class ePoolEntryType
	{
		Item,          // Plain item
		Tag,           // Descriptor for multiple items. E.g Music Discs, newer arrows...
		LootTable,     // Another loot table
		Group,         // Child entries
		Alternatives,  // Select one entry from the list
		Sequence,      // Select entries until one cannot be granted
		Dynamic,       // "generate block specific drops": drops the block itself as item
		Empty
	};




	// Declaration of methods in the cpp
	/** Gets the ePoolEntryType from String. Defaults to Empty */
	enum ePoolEntryType ePoolEntryType(const AString & a_Type);
	/** Reads keys min and max and writes them into a_Min and a_Max
	if there are no appropriately named keys there is no change */
	template <typename T>
	void MinMaxRange(const Json::Value & a_Value, T & a_Min, T & a_Max);


	/** Represents a condition for a pool item */
	struct cLootTableCondition;
	using cLootTableConditions = std::vector<cLootTableCondition>;





	namespace Condition
	{
		class cCondition
		{
		public:
			bool IsActive() const { return m_Active; }
		protected:
			bool m_Active = false;
		};

		// Note: this following list is mostly alpha sorted - there are some types included in others so some come earlier
		// The implementation will be in alphabetical order

		/** Checks tool */
		class cMatchTool : public cCondition
		/* predicate: Predicate applied to item, uses same structure as advancements. */
		{
		public:
			cMatchTool() { m_Active = false; }
			cMatchTool(const Json::Value & a_Value);
			/** Default operation. Gets the killers item in hand and checks it */
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
			/** Simply checks the item given */
			bool operator () (const cItem & a_Item) const;
		private:
			int m_CountMin = 0;
			int m_CountMax = 65;  // Chose impossible value

			int m_DurabilityMin = 0;
			int m_DurabilityMax = std::numeric_limits<short>::max();

			// Checks if the values in the item are in between those in min and max
			cEnchantments m_EnchantmentsMin;
			cEnchantments m_EnchantmentsMax;

			// Checks the stored enchantments in books
			cEnchantments m_StoredEnchantmentsMin;
			cEnchantments m_StoredEnchantmentsMax;

			cItem m_Item;
			AString m_NBT;
			// Todo: Potion
			AString m_Tag;
		};


		// Check properties of a block state.
		/*  block: A block ID. The test fails if the block doesn't match.
			properties: (Optional) A map of block property names to values. All values are strings. The test fails if the block doesn't match. */
		class cBlockStateProperty : public cCondition
		{
		public:
			cBlockStateProperty() { m_Active = false; }
			cBlockStateProperty(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const Vector3i & a_Pos) const;
		private:
			BLOCKTYPE m_Block;
			AStringMap m_Properties;
		};


		// Checks if the current location matches.
		/*  offsetX - optional offsets to location
			offsetY - optional offsets to location
			offsetZ - optional offsets to location
			predicate: Predicate applied to location, uses same structure as advancements. */
		class cLocationCheck : public cCondition
		{
		public:
			cLocationCheck() { m_Active = false; }
			cLocationCheck(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;

		private:
			// Offset
			int m_OffsetX = 0;
			int m_OffsetY = 0;
			int m_OffsetZ = 0;

			EMCSBiome m_Biome = EMCSBiome::biInvalidBiome;

			// Block
			cBlockStateProperty m_BlockState;  // Also checks block type
			enum ItemTag::eItemTags m_BlockTag = ItemTag::eItemTags::None;
			AString m_BlockNBT;

			eDimension m_Dimension = eDimension::dimNotSet;
			AString m_Feature;  // A string describing a type of structure

			// Fluid
			cBlockStateProperty m_FluidState;  // Also checks for block type
			AString m_FluidTag;

			// Light
			int m_LightMin = 0;
			int m_LightMax = 100;

			double m_XMin = std::numeric_limits<double>::min();
			double m_XMax = std::numeric_limits<double>::max();

			double m_YMin = std::numeric_limits<double>::min();
			double m_YMax = std::numeric_limits<double>::max();

			double m_ZMin = std::numeric_limits<double>::min();
			double m_ZMax = std::numeric_limits<double>::max();

			// True if the block is closely above a campfire or soul campfire.
			bool m_Smokey;
		};

		// Test properties of an entity.
		/*  entity: Specifies the entity to check for the condition. Set to this to use the entity that died or the player that gained the advancement, opened the container or broke the block, killer for the killer, or killer_player for a killer that is a player.
		predicate: Predicate applied to entity, uses same structure as advancements. */
		class cEntityProperties : public cCondition
		{
		public:
			enum class eDest
			{
				This,
				Killer,
				KillerPlayer,
			};
			cEntityProperties() { m_Active = false; }
			cEntityProperties(const Json::Value & a_Value);
			cEntityProperties(const Json::Value & a_Value, enum eDest a_Dest);
			cEntityProperties(const cEntityProperties & a_Other) = default;
			cEntityProperties(cEntityProperties && a_Other) = default;
			cEntityProperties & operator = (const cEntityProperties & a_Other) = default;
			cEntityProperties & operator = (cEntityProperties && a_Other) = default;
			~cEntityProperties();
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;

		private:
			void FromJson(const Json::Value & a_Value);

			eDest m_Dest;

			// Distance
			float m_AbsoluteMin = 0;
			float m_AbsoluteMax = std::numeric_limits<float>::max();
			float m_HorizontalMin = 0;
			float m_HorizontalMax = std::numeric_limits<float>::max();
			float m_XMin = 0;
			float m_XMax = std::numeric_limits<float>::max();
			float m_YMin = 0;
			float m_YMax = std::numeric_limits<float>::max();
			float m_ZMin = 0;
			float m_ZMax = std::numeric_limits<float>::max();
			// Effects
			struct sEffectDesc
			{
				sEffectDesc() = default;
				sEffectDesc(const Json::Value & a_Value);
				int m_AmplifierMin = 0;
				int m_AmplifierMax = std::numeric_limits<int>::max();

				int m_DurationMin = 0;
				int m_DurationMax = std::numeric_limits<int>::max();
			};
			std::unordered_map<cEntityEffect::eType, sEffectDesc> m_Effects;
			// Equipment
			cMatchTool m_MainHand;
			cMatchTool m_OffHand;
			cMatchTool m_Head;
			cMatchTool m_Chest;
			cMatchTool m_Legs;
			cMatchTool m_Feet;

			// Flags
			bool m_IsOnFire = false;
			bool m_IsSneaking = false;
			bool m_IsSprinting = false;
			bool m_IsSwimming = false;
			bool m_IsBaby = false;

			// Location
			cLocationCheck m_Location;

			// NBT
			AString m_NBT;

			// Player
			bool m_Player = false;

			// Advancements
			// TODO: 14.09.2020 - Add when implemented - 12xx12
			// Gamemode
			eGameMode m_Gamemode = eGameMode_NotSet;
			// Level
			int m_LevelMin = 0;
			int m_LevelMax = std::numeric_limits<int>::max();
			// Recipes
			// TODO: 16.09.2020 - Add - 12xx12
			// Stats
			// TODO: 16.09.2020 - Add - 12xx12
			// Team
			AString m_Team;
			// Type
			cEntity::eEntityType m_EntityType;
			cEntityProperties * m_TargetEntity = nullptr;
			cEntityProperties * m_Vehicle = nullptr;
		};

		// Joins conditions from parameter terms with 'or'.
		/* terms: A list of conditions to join using 'or'. */
		class cAlternative : public cCondition
		{
		public:
			cAlternative() { m_Active = false; }
			cAlternative(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource) const;
		private:
			cLootTableConditions m_SubConditions;
		};

		// Check properties of damage source.
		/*  predicate: Predicate applied to the damage source.
			bypasses_armor: Checks if the damage bypassed the armor of the player (suffocation damage predominantly).
			bypasses_invulnerability: Checks if the damage bypassed the invulnerability status of the player (void or /kill damage).
			bypasses_magic: Checks if the damage was caused by starvation.
			direct_entity: The entity that was the direct cause of the damage.

			is_explosion: Checks if the damage originated from an explosion.
			is_fire: Checks if the damage originated from fire.
			is_magic: Checks if the damage originated from magic.
			is_projectile: Checks if the damage originated from a projectile.
			is_lightning: Checks if the damage originated from lightning.
			source_entity: Checks the entity that was the source of the damage (for example: The skeleton that shot the arrow). */
		class cDamageSourceProperties : public cCondition
		{
		// TODO: 10.09.2020 - Add - 12xx12
		public:
			cDamageSourceProperties(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource) const;
		private:
			bool m_BypassesArmor;
			bool m_BypassesInvulnerability;
			bool m_BypassesMagic;
			// cEntityProperties m_DirectEntity;  // The entity that caused the damage
			bool m_IsFire;
			bool m_IsExplosion;
			bool m_IsMagic;
			bool m_IsProjectile;
			bool m_IsLightning;
			cEntityProperties m_SourceEntity;  // The entity causing the cause of the damage: e.g. skeleton shooting on creeper for disk drop
		};

		// Test the scoreboard scores of an entity.
		/* 	entity: Specifies the entity to check for the condition. Set to this to use the entity that died or the player that gained the advancement, opened the container or broke the block, killer for the killer, or killer_player for a killer that is a player.
			scores: Scores to check. All specified scores must pass for the condition to pass.
			A score: Key name is the objective while the value is the exact score value required for the condition to pass.
			A score: Key name is the objective while the value specifies a range of score values required for the condition to pass.
			min: Minimum score.
			max: Maximum score. */
		class cEntityScores : public cCondition
		{
		public:
			cEntityScores(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
		};

		// Inverts condition from parameter term.
		/* term: The condition to be negated. */
		class cInverted : public cCondition
		{
			// This is the plural because the simple cLootTableCondition can't be used here
			// because the alternative in the condition struct needs the structs of the conditions but this condition needs the condition struct and so on..
		public:
			cInverted(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource) const;
		private:
			cLootTableConditions m_Conditions;
		};

		// Test if a killer_player entity is available.
		/* inverse: If true, the condition passes if killer_player is not available. */
		class cKilledByPlayer : public cCondition
		{
		public:
			cKilledByPlayer(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			bool m_Inverse;
		};

		// Test if a random number 0.0 - 1.0 is less than a specified value.
		/* chance: Success rate as a number 0.0 - 1.0. */
		class cRandomChance : public cCondition
		{
		public:
			cRandomChance(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			float m_Chance;
		};

		// Test if a random number 0.0 - 1.0 is less than a specified value, affected by the level of Looting on the killer entity.
		/*  chance: Base success rate.
			looting_multiplier: Looting adjustment to the base success rate. Formula is chance + (looting_level * looting_multiplier). */
		class cRandomChanceWithLooting : public cCondition
		{
		public:
			cRandomChanceWithLooting(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			float m_Chance;
			float m_LootingMultiplier;
		};

		// Test if another referred condition (predicate) passes.
		/* name: The namespaced ID of the condition (predicate) referred to. A cyclic reference causes a parsing failure. */
		class cReference : public cCondition
		{
		public:
			cReference(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			AString m_Name;
		};

		// Returns true with 1 divided by explosion radius probability.
		class cSurvivesExplosion : public cCondition
		{
		public:
			cSurvivesExplosion() { m_Active = true; };
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
		};

		// Passes with probability picked from table, indexed by enchantment level.
		/*  enchantment: Id of enchantment.
			chances: List of probabilities for enchantment level, indexed from 0. */
		class cTableBonus : public cCondition
		{
		public:
			cTableBonus(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			int m_Enchantment;
			std::unordered_map<int, float> m_Chances;
		};

		// Checks the current time
		/*  value: The time value in ticks.
			min: The minimum value.
			max: The maximum value.
			period: If present, time gets modulo-divided by this value (for example, if set to 24000, value operates on a time period of days). */
		class cTimeCheck : public cCondition
		{
		public:
			cTimeCheck(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			int m_Min;
			int m_Max;
			int m_Period = 24000;  // Defaults to one day
		};

		// Checks for a current weather state
		/*  raining: If true, the condition evaluates to true only if it's raining.
			thundering: If true, the condition evaluates to true only if it's thundering. */
		class cWeatherCheck : public cCondition
		{
		public:
			cWeatherCheck(const Json::Value & a_Value);
			bool operator () (cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			bool m_Raining = false;
			bool m_Thundering = false;
		};

		class cNone : public cCondition {};
	}  // Namespace Condition

	namespace Function
	{
		/** The Base class for Loot Table functions */
		class cFunction
		{
		public:
			bool IsActive(void) { return m_Active; }
		protected:
			bool m_Active = false;
		};





		/** Applies a predefined bonus formula. */
		class cApplyBonus : public cFunction
		/*
		enchantment: Enchantment ID used for level calculation.
		formula: Can be binomial_with_bonus_count for a binomial distribution (with n=level + extra, p=probability), uniform_bonus_count for uniform distribution (from 0 to level * bonusMultiplier), or ore_drops for a special function used for ore drops in the vanilla game (Count * (max(0; random(0..Level + 2) - 1)+1)).
		parameters: Values required for the formula.

		extra: For formula 'binomial_with_bonus_count', the extra value.
		probability : For formula 'binomial_with_bonus_count', the probability.
		bonusMultiplier : For formula 'uniform_bonus_count', the bonus multiplier.
		*/
		{
		public:
			cApplyBonus(const Json::Value & a_Value);
			void operator () (cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			int m_Enchantment = -1;
			enum class eFormula
			{
				BinomialWithBonusCount,
				UniformBonusCount,
				OreDrops,
				None
			};
			eFormula m_Formula = eFormula::None;
			int m_Extra = 0;
			float m_Probability = 0.0f;
			float m_BonusMultiplier = 1.0f;
		};




		/** Copies a block entity's CustomName tag into the item's CustomName. */
		class cCopyName : public cFunction
		/*
		source: "block_entity" - Needs to be set to 'block_entity'.
		*/
		{
		public:
			cCopyName(const Json::Value & a_Value);
			void operator () (cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			bool m_Active = false;
		};




		/** Copies nbt to the item's tag tag. */
		class cCopyNbt : public cFunction
		/*
		source: Specifies the source. Set to block_entity for the block entity of the destroyed block, this to use the entity that died or the player that gained the advancement, opened the container or broke the block, killer for the killer, or killer_player for a killer that is a player.
		ops: A list of copy operations
			source: The nbt path to copy from.
			target: The nbt path to copy to, starting from the item's tag tag.
			op: Can be replace to replace any existing contents of the target, append to append to a list, or merge to merge into a compound tag. */
		{
		public:
			cCopyNbt(const Json::Value & a_Value);
			void operator () (cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			enum class eSource
			{
				BlockEntity,
				This,
				Killer,
				KillerPlayer
			};
			eSource m_Source;
			enum class eOperation
			{
				Replace,
				Append,
				Merge
			};
			struct sOperation
			{
				AString m_Source;
				AString m_Target;
				eOperation m_Operation;
			};
			std::vector<sOperation> m_Operations;
		};





		/** Copies state properties from dropped block to the item's BlockStateTag tag. */
		class cCopyState : public cFunction
		/*
		block: A block ID. Function fails if the block doesn't match.
		properties: A list of property names to copy.
		A block state name to copy.
		*/
		{
		public:
			cCopyState(const Json::Value & a_Value);
			void operator () (cItem & a_Item, cWorld & a_World, const Vector3i & a_Pos) const;
		private:
			short m_Block;
			AStringList m_Properties;
		};




		/**  Enchants the item with one randomly-selected enchantment. The level of the enchantment, if applicable, is random. */
		class cEnchantRandomly : public cFunction
		/* enchantments: List of enchantment IDs to choose from. If omitted, all enchantments applicable to the item are possible. */
		{
		public:
			cEnchantRandomly(const Json::Value & a_Value);
			void operator () (cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const;
		private:
			std::vector<int> m_Enchantments;
		};




		/** Enchants the item, with the specified enchantment level (roughly equivalent to using an enchantment table at that level). */
		class cEnchantWithLevels : public cFunction
		/* treasure: Determines whether treasure enchantments are allowed on this item.
		levels: Specifies the exact enchantment level to use.
		levels: Specifies a random enchantment level within a range.

		min: Minimum level to use.
		max: Maximum level to use. */
		{
		public:
			cEnchantWithLevels(const Json::Value & a_Value);
			void operator () (cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const;
		private:
			bool m_Treasure = false;
			int m_LevelsMin = 0;
			int m_LevelsMax = std::numeric_limits<int>::max();
		};




		/** Converts an empty map into an explorer map leading to a nearby generated structure. */
		class cExplorationMap : public cFunction
		/* destination: The type of generated structure to locate. Accepts any of the StructureTypes used by the /locate command (case insensitive).
		decoration: The icon used to mark the destination on the map. Accepts any of the map icon text IDs (case insensitive). If mansion or monument is used, the color of the lines on the item texture changes to match the corresponding explorer map.
		zoom: The zoom level of the resulting map. Defaults to 2.
		search_radius: The size, in chunks, of the area to search for structures. The area checked is square, not circular. Radius 0 causes only the current chunk to be searched, radius 1 causes the current chunk and eight adjacent chunks to be searched, and so on. Defaults to 50.
		skip_existing_chunks: Don't search in chunks that have already been generated. Defaults to true. */
		{
		public:
			cExplorationMap(const Json::Value & a_Value);
			void operator () (cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			AString m_Destination;
			AString m_Decoration;
			int m_Zoom = 2;
			int m_SearchRadius = 50;
			bool m_SkipExistingChunks = true;
		};



		/** For loot tables of type 'block', removes some items from a stack, if there was an explosion.
		Each item has a chance of 1 divided by explosion radius to be lost. */
		class cExplosionDecay : public cFunction
		/*  */
		{
		public:
			cExplosionDecay() { m_Active = true; }
			void operator () (cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		};




		/** Smelts the item as it would be in a furnace. */
		class cFurnaceSmelt : public cFunction
		/*  */
		{
		public:
			cFurnaceSmelt() { m_Active = true; }
			void operator () (cItem & a_Item) const;
		};




		/**  Adds required item tags of a player head  */
		class cFillPlayerHead : public cFunction
		/*  entity: Specifies an entity to be used for the player head. Set to this to use the entity that died or the player that gained the advancement, opened the container or broke the block, killer for the killer, or killer_player for a killer that is a player. */
		{
		public:
			cFillPlayerHead(const Json::Value & a_Value);
			void operator () (cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			enum class eDest { This, Killer, KillerPlayer };
			enum eDest m_Dest = eDest::This;
			static void MobTypeToHead(eMonsterType a_Type, cItem & a_Item);
		};




		/** Limits the count of every item stack. */
		class cLimitCount : public cFunction
		/* limit: Specifies the exact limit to use.
		limit: Specifies a random limit within a range.

		min: Minimum limit to use.
		max: Maximum limit to use. */
		{
		public:
			cLimitCount(const Json::Value & a_Value);
			void operator () (cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const;
		private:
			bool m_Active = false;
			int m_LimitMin = 0;
			int m_LimitMax = std::numeric_limits<int>::max();
		};




		/** Adjusts the stack size based on the level of the Looting enchantment on the killer entity. */
		class cLootingEnchant : public cFunction
		/* count: Specifies an exact number of additional items per level of looting.
		count: Specifies a random number (within a range) of additional items per level of looting. Note the random number generated may be fractional, rounded after multiplying by the looting level.

		min: Minimum increase.
		max: Maximum increase.

		limit: Specifies the maximum amount of items in the stack after the looting calculation. If the value is 0, no limit is applied. */
		{
		public:
			cLootingEnchant(const Json::Value & a_Value);
			void operator () (cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KillerID) const;
		private:
			int m_CountMin = 0;
			int m_CountMax = std::numeric_limits<int>::max();
			int m_Limit = 0;
		};




		/** Add attribute modifiers to the item. */
		class cSetAttributes : public cFunction
		{
		public:
			cSetAttributes(const Json::Value & a_Value);
			void operator () (cItem & a_Item) const;
		};

		/** For loot tables of type 'block', sets the contents of a container block item to a list of entries. */
		class cSetContents : public cFunction
		/* entries: The entries to use as contents. */
		{
		public:
			cSetContents(const Json::Value & a_Value);
			void operator () (cItem & a_Item) const;
		};




		/** Sets the stack size. */
		class cSetCount : public cFunction
		/* count: Specifies the exact stack size to set.
		count: Specifies a random stack size within a range.

		type: The distribution type. Arguments lie in the count compound.
		uniform: Uniform distribution. A random integer is chosen with probability of each number being equal.
			min: Minimum stack size.
			max: Maximum stack size.
		binomial: Binomial distribution. Roll a number of times, each having a chance of adding 1 to the stack size.
			n: Number of rolls.
			p: Chance of each roll.
		*/
		{
		public:
			cSetCount(const Json::Value & a_Value);
			void operator () (cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const;
		private:
			int m_Count = 0;
			enum class eType
			{
				Uniform,
				Binomial
			};
			eType m_Type;
			int m_UniformMin = 0;
			int m_UniformMax = std::numeric_limits<int>::max();

			int m_N = 1;
			float m_P = 1.0f;
		};



		/** Sets the item's damage value (durability) for tools. */
		class cSetDamage : public cFunction
		/* damage: Specifies the damage fraction to set (1.0 is undamaged, 0.0 is zero durability left).
		damage: Specifies a random damage fraction within a range.

		min: Minimum value.
		max: Maximum value. */
		{
		public:
			cSetDamage(const Json::Value & a_Value);
			void operator () (cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const;
		private:
			float m_Min = 0.0f;
			float m_Max = std::numeric_limits<float>::max();
		};




		/** Sets the loot table for a container (chest etc.). */
		class cSetLootTable : public cFunction
		/* name: Specifies the resource location of the loot table to be used.
		seed: Optional. Specifies the loot table seed. If absent or set to 0, a random seed will be used. */
		{
		public:
			cSetLootTable(const Json::Value & a_Value);
			void operator () (cItem & a_Item) const;
		private:
			AString m_LootTable;
			int m_Seed;
		};




		/**  Adds lore to the item  */
		class cSetLore : public cFunction
		/* lore: List of JSON text components. Each list entry represents one line of the lore.
		entity: Specifies the entity to act as the source @s in the JSON text component. Set to this to use the entity that died or the player that gained the advancement, opened the container or broke the block, killer for the killer, or killer_player for a killer that is a player.
		replace: If true, replaces all existing lines of lore, if false appends the list. */
		{
		public:
			cSetLore(const Json::Value & a_Value);
			void operator () (cItem & a_Item, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			enum class eType
			{
				This,
				Killer,
				KillerPlayer
			};
			eType m_Type = eType::This;

			AStringVector m_Lore;
			bool m_Replace;
		};




		/** Adds display name of the item. */
		class cSetName : public cFunction
		/* name: A JSON text component name, allowing color, translations, etc.
		entity: Specifies an entity to act as source @s in the JSON text component. Set to this to use the entity that died or the player that gained the advancement, opened the container or broke the block, killer for the killer, or killer_player for a killer that is a player. */
		{
		public:
			cSetName(const Json::Value & a_Value);
			void operator () (cItem & a_Item, UInt32 a_KilledID, UInt32 a_KillerID) const;
		private:
			enum class eType
			{
				This,
				Killer,
				KillerPlayer
			};
			AString m_Name;
			eType m_Type = eType::This;
		};




		/** Adds NBT data to the item. */
		class cSetNbt : public cFunction
		/* tag: Tag string to add, similar to those used by commands. Note that the first bracket is required and quotation marks need to be escaped using a backslash (\). */
		{
		public:
			cSetNbt(const Json::Value & a_Value);
			void operator () (cItem & a_Item) const;
		private:
			AString m_Tag;
		};




		/** Sets the status effects for suspicious stew. */
		class cSetStewEffect : public cFunction
		/* effects: The effects to apply.

		An effect.
			type: The effect ID.
			duration: The duration of the effect.
		*/
		{
		public:
			cSetStewEffect(const Json::Value & a_Value);
			void operator () (cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const;
		};




		/** No Function. just there for errors in parsing */
		class cNone {};
	}  // Namespace Function

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

#define VISITCONDITION \
	overloaded { \
		[&] (const LootTable::Condition::cAlternative & a_Cond)             { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource); }, \
		[&] (const LootTable::Condition::cBlockStateProperty & a_Cond)      { return a_Cond(a_World, a_Pos); }, \
		[&] (const LootTable::Condition::cDamageSourceProperties & a_Cond)  { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource); }, \
		[&] (const LootTable::Condition::cEntityProperties & a_Cond)        { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cEntityScores & a_Cond)            { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cInverted & a_Cond)                { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource); }, \
		[&] (const LootTable::Condition::cKilledByPlayer & a_Cond)          { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cLocationCheck & a_Cond)           { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cMatchTool & a_Cond)               { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cRandomChance & a_Cond)            { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cRandomChanceWithLooting & a_Cond) { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cReference & a_Cond)	            { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cSurvivesExplosion & a_Cond)	    { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cTableBonus & a_Cond)              { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cTimeCheck & a_Cond)               { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cWeatherCheck & a_Cond)            { return a_Cond(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Condition::cNone & a_Cond)                    { return true; } \
	} \

#define VISITFUNCTION \
	overloaded { \
		[&] (const LootTable::Function::cApplyBonus        & a_Function) { a_Function(a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Function::cCopyName          & a_Function) { a_Function(a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Function::cCopyNbt           & a_Function) { a_Function(a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Function::cCopyState         & a_Function) { a_Function(a_Item, a_World, a_Pos); }, \
		[&] (const LootTable::Function::cEnchantRandomly   & a_Function) { a_Function(a_Item, a_Noise, a_Pos); }, \
		[&] (const LootTable::Function::cEnchantWithLevels & a_Function) { a_Function(a_Item, a_Noise, a_Pos); }, \
		[&] (const LootTable::Function::cExplorationMap    & a_Function) { a_Function(a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Function::cExplosionDecay    & a_Function) { a_Function(a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Function::cFurnaceSmelt      & a_Function) { a_Function(a_Item); }, \
		[&] (const LootTable::Function::cFillPlayerHead    & a_Function) { a_Function(a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Function::cLimitCount        & a_Function) { a_Function(a_Item, a_Noise, a_Pos); }, \
		[&] (const LootTable::Function::cLootingEnchant    & a_Function) { a_Function(a_Item, a_World, a_Noise, a_Pos, a_KillerID); }, \
		[&] (const LootTable::Function::cSetAttributes     & a_Function) { a_Function(a_Item); }, \
		[&] (const LootTable::Function::cSetContents       & a_Function) { a_Function(a_Item); }, \
		[&] (const LootTable::Function::cSetCount          & a_Function) { a_Function(a_Item, a_Noise, a_Pos); }, \
		[&] (const LootTable::Function::cSetDamage         & a_Function) { a_Function(a_Item, a_Noise, a_Pos); }, \
		[&] (const LootTable::Function::cSetLootTable      & a_Function) { a_Function(a_Item); }, \
		[&] (const LootTable::Function::cSetLore           & a_Function) { a_Function(a_Item, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Function::cSetName           & a_Function) { a_Function(a_Item, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Function::cSetNbt            & a_Function) { a_Function(a_Item); }, \
		[&] (const LootTable::Function::cSetStewEffect     & a_Function) { a_Function(a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID); }, \
		[&] (const LootTable::Function::cNone              & a_Function) {} \
	} \


	struct cLootTableCondition
	{
		using Alternative = std::variant<
			Condition::cAlternative,
			Condition::cBlockStateProperty,
			Condition::cDamageSourceProperties,
			Condition::cEntityProperties,
			Condition::cEntityScores,
			Condition::cInverted,
			Condition::cKilledByPlayer,
			Condition::cLocationCheck,
			Condition::cMatchTool,
			Condition::cRandomChance,
			Condition::cRandomChanceWithLooting,
			Condition::cReference,
			Condition::cSurvivesExplosion,
			Condition::cTableBonus,
			Condition::cTimeCheck,
			Condition::cWeatherCheck,
			Condition::cNone>;


		cLootTableCondition(
			Alternative a_Parameter
		):
			m_Parameter(std::move(a_Parameter))
		{
		}

		Alternative m_Parameter;
	};




	/** Represents a function for a pool item */
	struct cLootTableFunction;
	using cLootTableFunctions = std::vector<cLootTableFunction>;
	struct cLootTableFunction
	{
		using Alternative = std::variant<
			Function::cApplyBonus,
			Function::cCopyName,
			Function::cCopyNbt,
			Function::cCopyState,
			Function::cEnchantRandomly,
			Function::cEnchantWithLevels,
			Function::cExplorationMap,
			Function::cExplosionDecay,
			Function::cFurnaceSmelt,
			Function::cFillPlayerHead,
			Function::cLimitCount,
			Function::cLootingEnchant,
			Function::cSetAttributes,
			Function::cSetContents,
			Function::cSetCount,
			Function::cSetDamage,
			Function::cSetLootTable,
			Function::cSetLore,
			Function::cSetName,
			Function::cSetNbt,
			Function::cSetStewEffect,
			Function::cNone>;

		cLootTableFunction(
			Alternative a_Function,
			cLootTableConditions a_Conditions
		):
			m_Function(std::move(a_Function)),
			m_Conditions(std::move(a_Conditions))
		{
		}

		Alternative m_Function;
		cLootTableConditions m_Conditions;
	};




	/** Stores the rolls of a pool */
	struct cLootTablePoolRolls
	{
		cLootTablePoolRolls()
		{
			m_RollsMin = 0;
			m_RollsMax = 0;
		}




		/** Pool rolls. If min = max -> steady roll count */
		cLootTablePoolRolls(
			int a_RollsMin,
			int a_RollsMax
		):
			m_RollsMin(a_RollsMin),
			m_RollsMax(a_RollsMax)
		{
		}




		/** Returns the rolls result */
		int operator()(const cNoise & a_Noise, const Vector3i & a_Pos) const
		{
			if (m_RollsMax == m_RollsMin)
			{
				return m_RollsMin;
			}
			return (a_Noise.IntNoise3DInt(a_Pos) / 7) % (m_RollsMax - m_RollsMin) + m_RollsMin;
		}
		// Note: The loot tables specify another value (type) this is usually "Uniform".
		// I think this is the probability distribution for the random value.
		// The wiki doesn't have any information in it. So this is left out for now
		int m_RollsMin;
		int m_RollsMax;
	};




	/** Represents a pool entry */
	struct cLootTablePoolEntry;
	using cLootTablePoolEntries = std::vector<cLootTablePoolEntry>;
	struct cLootTablePoolEntry
	{
		cLootTablePoolEntry()
		{
			m_Type = ePoolEntryType::Empty;
		}





		cLootTablePoolEntry(
			cLootTableConditions a_Conditions,
			cLootTableFunctions a_Functions,
			enum ePoolEntryType a_Type,
			cItem a_Item,
			int a_Weight,
			int a_Quality
		):
			m_Conditions(std::move(a_Conditions)),
			m_Functions(std::move(a_Functions)),
			m_Type(a_Type),
			m_Content(std::move(a_Item)),
			m_Weight(a_Weight),
			m_Quality(a_Quality)
		{
		}





		cLootTablePoolEntry(
			cLootTableConditions a_Conditions,
			cLootTableFunctions a_Functions,
			enum ePoolEntryType a_Type,
			AString a_Name,
			bool a_Expand,
			int a_Weight,
			int a_Quality
		):
			m_Conditions(std::move(a_Conditions)),
			m_Functions(std::move(a_Functions)),
			m_Type(a_Type),
			m_Content(std::move(a_Name)),
			m_Expand(a_Expand),
			m_Weight(a_Weight),
			m_Quality(a_Quality)
		{
		}





		cLootTablePoolEntry(
			cLootTableConditions a_Conditions,
			cLootTableFunctions a_Functions,
			enum ePoolEntryType a_Type,
			cLootTablePoolEntries a_Children,
			int a_Weight,
			int a_Quality
		):
			m_Conditions(std::move(a_Conditions)),
			m_Functions(std::move(a_Functions)),
			m_Type(a_Type),
			m_Content(std::move(a_Children)),
			m_Weight(a_Weight),
			m_Quality(a_Quality)
		{
		}

		cLootTableConditions m_Conditions;
		cLootTableFunctions m_Functions;
		enum ePoolEntryType m_Type;
		std::variant<cItem, AString, cLootTablePoolEntries> m_Content;
		bool m_Expand;
		int m_Weight;
		int m_Quality;
	};




	/** Represents a loot table pool */
	struct cLootTablePool;
	using cLootTablePools = std::vector<cLootTablePool>;
	struct cLootTablePool
	{
		cLootTablePool(
			cLootTablePoolRolls a_Rolls,
			cLootTablePoolEntries a_Entries,
			cLootTableConditions a_Conditions
		):
			m_Rolls(a_Rolls),
			m_Entries(std::move(a_Entries)),
			m_Conditions(std::move(a_Conditions))
		{
			for (const auto & Entry : m_Entries)
			{
				m_TotalWeight += Entry.m_Weight;
				m_TotalQuality += Entry.m_Quality;
			}
		}





		cLootTablePool(
			cLootTablePoolRolls a_Rolls,
			cLootTablePoolRolls a_BonusRolls,
			cLootTablePoolEntries a_Entries,
			cLootTableConditions a_Conditions
		):
			m_Rolls(a_Rolls),
			m_BonusRolls(a_BonusRolls),
			m_Entries(std::move(a_Entries)),
			m_Conditions(std::move(a_Conditions))
		{
			for (const auto & Entry : m_Entries)
			{
				m_TotalWeight += Entry.m_Weight;
				m_TotalQuality += Entry.m_Weight;
			}
		}

		cLootTablePoolRolls m_Rolls;
		cLootTablePoolRolls m_BonusRolls;
		cLootTablePoolEntries m_Entries;
		cLootTableConditions m_Conditions;
		int m_TotalWeight = 0;
		int m_TotalQuality = 0;
	};

	/** Reads Loot Table pool from Json */
	extern cLootTablePool ParsePool(const Json::Value & a_Value);

	/** Reads a loot table function from Json */
	extern cLootTableFunction ParseFunction(const Json::Value & a_Value);

	/** Reads a loot table condition from Json */
	extern cLootTableCondition ParseCondition(const Json::Value & a_Value);

	/** Reads a loot table pool entry from Json */
	extern cLootTablePoolEntry ParsePoolEntry(const Json::Value & a_Value);

	/** Reads a loot table pool roll from Json */
	extern cLootTablePoolRolls ParseRolls(const Json::Value & a_Value);
}
