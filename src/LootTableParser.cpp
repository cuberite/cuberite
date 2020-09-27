
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LootTableParser.h"
#include "Entities/Player.h"
#include "Entities/Entity.h"
#include "BlockEntities/BlockEntity.h"
#include "FurnaceRecipe.h"
#include "Registries/ItemTags.h"
#include "Root.h"
#include "Mobs/Monster.h"
#include "json/json.h"

#include "World.h"

namespace LootTable
{
	enum ePoolEntryType ePoolEntryType(const AString & a_Type)
	{

		if (NoCaseCompare(a_Type, "Item") == 0)
		{
			return ePoolEntryType::Item;
		}
		else if (NoCaseCompare(a_Type, "Tag") == 0)
		{
			return ePoolEntryType::Tag;
		}
		else if (NoCaseCompare(a_Type, "LootTable") == 0)
		{
			return ePoolEntryType::LootTable;
		}
		else if (NoCaseCompare(a_Type, "Group") == 0)
		{
			return ePoolEntryType::Group;
		}
		else if (NoCaseCompare(a_Type, "Alternatives") == 0)
		{
			return ePoolEntryType::Alternatives;
		}
		else if (NoCaseCompare(a_Type, "Sequence") == 0)
		{
			return ePoolEntryType::Sequence;
		}
		else if (NoCaseCompare(a_Type, "Dynamic") == 0)
		{
			return ePoolEntryType::Dynamic;
		}
		else if (NoCaseCompare(a_Type, "Empty") == 0)
		{
			return ePoolEntryType::Empty;
		}
		else
		{
			LOGWARNING("Unknown loot table pool entry type provided: %s. Defaulting to Empty", a_Type);
			return ePoolEntryType::Empty;
		}
	}





	template <typename T>
	void MinMaxRange(const Json::Value & a_Value, T & a_Min, T & a_Max)
	{
		if (a_Value.isNumeric())
		{
			a_Min = a_Max = a_Value.as<T>();
			return;
		}
		if (a_Value.isMember("min"))
		{
			a_Min = a_Value["min"].as<T>();
		}
		else if (a_Value.isMember("Min"))
		{
			a_Min = a_Value["Min"].as<T>();
		}

		if (a_Value.isMember("max"))
		{
			a_Max = a_Value["max"].as<T>();
		}
		else if (a_Value.isMember("Max"))
		{
			a_Max = a_Value["Max"].as<T>();
		}
		if (a_Min > a_Max)
		{
			a_Max = std::max(a_Min, a_Max);
		}
	}





	namespace Condition
	{
#define ACTIVECHECK if (!m_Active) { return true; }

////////////////////////////////////////////////////////////////////////////////
// cAlternative

	cAlternative::cAlternative(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (!a_Value.isObject()))
		{
			LOGWARNING("Loot table: Condition \"Alternative\" encountered a Json problem, dropping condition!");
			return;
		}
		Json::Value Terms;
		if (a_Value.isMember("terms"))
		{
			Terms = a_Value["terms"];
		}
		else if (a_Value.isMember("Terms"))
		{
			Terms = a_Value["Terms"];
		}
		else
		{
			LOGWARNING("Loot table condition \"Alternative\" is missing sub - conditions. Dropping condition!");
			return;
		}
		if (!Terms.isArray())
		{
			LOGWARNING("Loot table condition \"Alternative\" encountered a Json problem in it's sub - conditions. Dropping condition!");
			return;
		}
		m_Active = true;
		for (unsigned int i = 0; i < Terms.size(); i++)
		{
			m_SubConditions.emplace_back(ParseCondition(Terms[i]));
		}
	}





	bool cAlternative::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource) const
	{
		ACTIVECHECK
		bool Success = false;
		for (const auto & SubCondition : m_SubConditions)
		{
			Success |= std::visit(VISITCONDITION, SubCondition.m_Parameter);
		}
		return Success;
	}


////////////////////////////////////////////////////////////////////////////////
// cBlockStateProperty

	cBlockStateProperty::cBlockStateProperty(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"BlockStateProperty\" encountered a Json problem, dropping condition!");
			return;
		}
		m_Active = true;
		for (const auto & ParameterName : a_Value.getMemberNames())
		{
			if ((NoCaseCompare(ParameterName, "block") == 0) ||
				(NoCaseCompare(ParameterName, "fluid") == 0))
			// block and fluid is accepted here to prevent duplicated code in the cLocationCheck
			{
				cItem Item;

				if ((a_Value[ParameterName].isString()) && (StringToItem(a_Value[ParameterName].asString(), Item)))
				{
					m_Block = Item.m_ItemType;
				}
				else
				{
					LOGWARNING("Loot table: Failed to parse Block in Loot table in condition \"BlockStateProperty\"");
				}
			}
			else if (NoCaseCompare(ParameterName, "properties") == 0)
			{
				LOGWARNING("Loot table: \"BlockStateProperty\" is not yet supported!");
				// TODO: 06.09.2020 - Add when implemented - 12xx12
				/*
				Json::Value Properties = a_Value[ParameterName];
				for (const auto & Property : Properties.getMemberNames())
				{
					if (Properties[Property].isString())
					{
						LOGWARNING("Loot table: Encountered an Json error in condition \"BlockStateProperty - Properties\"");
					}
					m_Properties[Property] = Properties[Property].asString();
				}
				*/
			}
		}
	}

	bool cBlockStateProperty::operator()(cWorld & a_World, const Vector3i & a_Pos) const
	{
		ACTIVECHECK
		bool Res = true;
		// Check if block is the same
		Res &= (a_World.GetBlock(a_Pos) == m_Block);
		// Check if the properties match
		// TODO: 06.09.2020 - Add when implemented - 12xx12
		/*
		NOTE: This is just a guessed implementation Do it right
		auto BlockProperties = a_World.GetBlockProperties(a_Pos);
		for (const auto & Property : m_Properties)
		{
			Res &= (BlockProperties.at(Property.first) == Property.second);
		}
		*/
		return Res;
	}


	////////////////////////////////////////////////////////////////////////////////
	// cDamageSourceProperties

	cDamageSourceProperties::cDamageSourceProperties(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"DamageSourceProperties\" encountered a Json problem, dropping function!");
			return;
		}
		Json::Value Predicates;
		if (a_Value.isMember("predicate"))
		{
			Predicates = a_Value["predicate"];
		}
		else if (a_Value.isMember("Predicate"))
		{
			Predicates = a_Value["Predicate"];
		}
		else
		{
			LOGWARNING("Loot table: Condition \"DamageSourceProperties\" is missing it's predicates, dropping condition!");
			return;
		}
		m_Active = true;
		for (const auto & Key : Predicates.getMemberNames())
		{
			if ((NoCaseCompare(Key, "bypasses_armor") == 0) ||
				(NoCaseCompare(Key, "BypassesArmor") == 0))
			{
				m_BypassesArmor = Predicates[Key].asBool();
			}
			else if (
				(NoCaseCompare(Key, "bypasses_invulnerability") == 0) ||
				(NoCaseCompare(Key, "BypassesInvulnerability") == 0))
			{
				m_BypassesInvulnerability = Predicates[Key].asBool();
			}
			else if (
				(NoCaseCompare(Key, "bypasses_magic") == 0) ||
				(NoCaseCompare(Key, "BypassesMagic") == 0))
			{
				m_BypassesMagic = Predicates[Key].asBool();
			}
			else if (
				(NoCaseCompare(Key, "direct_entity") == 0) ||
				(NoCaseCompare(Key, "DirectEntity") == 0))
			{
				// m_DirectEntity = cEntityProperties(Predicates[Key]);
			}
			else if (
				(NoCaseCompare(Key, "is_explosion") == 0) ||
				(NoCaseCompare(Key, "IsExplosion") == 0))
			{
				m_IsExplosion = Predicates[Key].asBool();
			}
			else if (
				(NoCaseCompare(Key, "is_fire") == 0) ||
				(NoCaseCompare(Key, "IsFire") == 0))
			{
				m_IsFire = Predicates[Key].asBool();
			}
			else if (
				(NoCaseCompare(Key, "is_magic") == 0) ||
				(NoCaseCompare(Key, "IsMagic") == 0))
			{
				m_IsMagic = Predicates[Key].asBool();
			}
			else if (
				(NoCaseCompare(Key, "is_projectile") == 0) ||
				(NoCaseCompare(Key, "IsProjectile") == 0))
			{
				m_IsProjectile = Predicates[Key].asBool();
			}
			else if (
				(NoCaseCompare(Key, "is_lightning") == 0) ||
				(NoCaseCompare(Key, "IsLightning") == 0))
			{
				m_IsLightning = Predicates[Key].asBool();
			}
			else if (
				(NoCaseCompare(Key, "source_entity") == 0) ||
				(NoCaseCompare(Key, "SourceEntity") == 0))
			{
				m_SourceEntity = cEntityProperties(Predicates[Key]);
			}
		}
	}

	bool cDamageSourceProperties::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource) const
	{
		ACTIVECHECK
		return true;
		bool Res = true;
		if (m_BypassesArmor)
		{
			a_World.DoWithEntityByID(a_KilledID, [&] (cEntity & a_Entity)
			{
				Res &= a_Entity.ArmorCoversAgainst(a_DamageSource.DamageType);
				return true;
			});
		}
		if (m_BypassesInvulnerability)
		{
			switch (a_DamageSource.DamageType)
			{
				case dtAdmin:
				case dtAttack:
				case dtCactusContact:
				case dtDrowning:
				case dtEnderPearl:
				case dtEnvironment:
				case dtExplosion:
				case dtFalling:
				case dtFireContact:
				case dtInVoid:
				case dtOnFire:
				case dtLavaContact:
				case dtLightning:
				case dtPoisoning:
				case dtPotionOfHarming:
				case dtRangedAttack:
				case dtStarving:
				case dtSuffocating:
				case dtWithering:
				{
					Res &= false;
					break;
				}


				{
					Res &= true;
					break;
				}
			}
		}
		if (m_BypassesMagic)
		{
			switch (a_DamageSource.DamageType)
			{
				case dtAdmin:
				case dtAttack:
				case dtCactusContact:
				case dtDrowning:
				case dtEnderPearl:
				case dtEnvironment:
				case dtExplosion:
				case dtFalling:
				case dtFireContact:
				case dtInVoid:
				case dtOnFire:
				case dtLavaContact:
				case dtLightning:
				case dtPoisoning:
				case dtPotionOfHarming:
				case dtRangedAttack:
				case dtSuffocating:
				case dtWithering:
				{
					Res = true;
					break;
				}
				case dtStarving:
				{
					Res = true;
					break;
				}
			}
		}
		if (m_IsFire)
		{
			switch (a_DamageSource.DamageType)
			{
				case dtAdmin:
				case dtAttack:
				case dtCactusContact:
				case dtDrowning:
				case dtEnderPearl:
				case dtEnvironment:
				case dtExplosion:
				case dtFalling:
				case dtInVoid:
				case dtLavaContact:
				case dtLightning:
				case dtPoisoning:
				case dtPotionOfHarming:
				case dtRangedAttack:
				case dtStarving:
				case dtSuffocating:
				case dtWithering:
				{
					Res &= false;
					break;
				}
				case dtFireContact:
				case dtOnFire:
				{
					Res &= true;
					break;
				}
			}
		}
		if (m_IsMagic)
		{
			switch (a_DamageSource.DamageType)
			{
				case dtAdmin:
				case dtAttack:
				case dtCactusContact:
				case dtDrowning:
				case dtEnderPearl:
				case dtEnvironment:
				case dtExplosion:
				case dtFalling:
				case dtFireContact:
				case dtInVoid:
				case dtOnFire:
				case dtLavaContact:
				case dtLightning:
				case dtRangedAttack:
				case dtStarving:
				case dtSuffocating:
				case dtWithering:
				{
					Res &= false;
					break;
				}
				case dtPoisoning:
				case dtPotionOfHarming:
				{
					Res &= true;
					break;
				}
			}
		}
		if (m_IsProjectile)
		{
			switch (a_DamageSource.DamageType)
			{
				case dtAdmin:
				case dtAttack:
				case dtCactusContact:
				case dtDrowning:
				case dtEnderPearl:
				case dtEnvironment:
				case dtExplosion:
				case dtFalling:
				case dtFireContact:
				case dtInVoid:
				case dtOnFire:
				case dtLavaContact:
				case dtLightning:
				case dtPoisoning:
				case dtPotionOfHarming:
				case dtStarving:
				case dtSuffocating:
				case dtWithering:
				{
					Res &= false;
					break;
				}
				case dtRangedAttack: Res &= true; break;
			}
		}
		if (m_IsLightning)
		{
			switch (a_DamageSource.DamageType)
			{
				case dtAdmin:
				case dtAttack:
				case dtCactusContact:
				case dtDrowning:
				case dtEnderPearl:
				case dtEnvironment:
				case dtExplosion:
				case dtFalling:
				case dtFireContact:
				case dtInVoid:
				case dtOnFire:
				case dtLavaContact:
				case dtPoisoning:
				case dtPotionOfHarming:
				case dtRangedAttack:
				case dtStarving:
				case dtSuffocating:
				case dtWithering:
				{
					Res &= false;
					break;
				}
				case dtLightning:
				{
					Res &= true;
					break;
				}
			}
		}
		// TODO: 22.09.2020 - Add when implemented - 12xx12
		// Res &= m_DirectEntity(a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
		Res &= m_SourceEntity(a_World, a_Noise, a_Pos, a_DamageSource.Attacker->GetUniqueID(), a_DamageSource.Attacker->GetUniqueID());
	}


////////////////////////////////////////////////////////////////////////////////
// cEntityProperties

	cEntityProperties::sEffectDesc::sEffectDesc(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: \"EffectDesc\" encountered a Json problem, dropping function!");
			return;
		}
		for (const auto & Key : a_Value.getMemberNames())
		{
			if (NoCaseCompare(Key, "amplifier") == 0)
			{
				MinMaxRange<int>(a_Value[Key], m_AmplifierMin, m_AmplifierMax);
			}
			else if (NoCaseCompare(Key, "duration") == 0)
			{
				MinMaxRange<int>(a_Value[Key], m_DurationMin, m_DurationMax);
			}
		}
	}





	cEntityProperties::cEntityProperties(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"EntityProperties\" encountered a Json problem, dropping function!");
			return;
		}
		AString DestString;
		if (a_Value.isMember("entity"))
		{
			DestString = NamespaceConverter(a_Value["entity"].asString());
		}
		else if (a_Value.isMember("Entity"))
		{
			DestString = NamespaceConverter(a_Value["Entity"].asString());
		}

		if ((DestString.empty()) || (NoCaseCompare(DestString, "This") == 0))
		{
			m_Dest = eDest::This;
		}
		else if (NoCaseCompare(DestString, "Killer") == 0)
		{
			m_Dest = eDest::Killer;
		}
		else if (NoCaseCompare(DestString, "KillerPlayer") == 0)
		{
			m_Dest = eDest::KillerPlayer;
		}
		FromJson(a_Value);
	}





	cEntityProperties::cEntityProperties(
		const Json::Value & a_Value, enum eDest a_Dest) :
		m_Dest(a_Dest)
	{
		FromJson(a_Value);
	}





	cEntityProperties::~cEntityProperties()
	{
		if (m_Vehicle != nullptr)
		{
			delete[] m_Vehicle;
		}
		if (m_TargetEntity != nullptr)
		{
			delete [] m_TargetEntity;
		}
	}




	void cEntityProperties::FromJson(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"EntityProperties\" encountered a Json problem, dropping condition!");
			return;
		}
		Json::Value Predicate;
		if (a_Value.isMember("predicate"))
		{
			Predicate = a_Value["predicate"];
		}
		else if (a_Value.isMember("Predicate"))
		{
			Predicate = a_Value["Predicate"];
		}
		else
		{
			LOGWARNING("Loot table: Condition \"EntityProperties\" is missing it's predicates, dropping condition!");
			return;
		}
		m_Active = true;
		for (const auto & Key : Predicate.getMemberNames())
		{
			if (NoCaseCompare(Key, "distance") == 0)
			{
				Json::Value Distance = Predicate[Key];
				if (Distance.isArray())
				{
					LOGWARNING("Loot table: Condition \"EntityProperties - Distance\" encountered a Json problem, dropping condition!");
					continue;
				}
				for (const auto & DistanceKey : Distance.getMemberNames())
				{
					if (NoCaseCompare(DistanceKey, "absolute") == 0)
					{
						MinMaxRange<float>(Distance[DistanceKey], m_AbsoluteMin, m_AbsoluteMax);
					}
					else if (NoCaseCompare(DistanceKey, "horizontal") == 0)
					{
						MinMaxRange<float>(Distance[DistanceKey], m_HorizontalMin, m_HorizontalMax);
					}
					else if (NoCaseCompare(DistanceKey, "x") == 0)
					{
						MinMaxRange<float>(Distance[DistanceKey], m_XMin, m_XMax);
					}
					else if (NoCaseCompare(DistanceKey, "y") == 0)
					{
						MinMaxRange<float>(Distance[DistanceKey], m_YMin, m_YMax);
					}
					else if (NoCaseCompare(DistanceKey, "z") == 0)
					{
						MinMaxRange<float>(Distance[DistanceKey], m_ZMin, m_ZMax);
					}
				}
			}
			else if (NoCaseCompare(Key, "effects") == 0)
			{
				const Json::Value EffectObject = Predicate[Key];
				if (EffectObject.isArray())
				{
					LOGWARNING("Loot table: Condition \"EntityProperties - Effects\" encountered a Json problem, dropping condition!");
					continue;
				}
				for (const auto & EffectKey : EffectObject.getMemberNames())
				{
					m_Effects[cEntityEffect::GetTypeFromString(NamespaceConverter(EffectKey))] = sEffectDesc(EffectObject[EffectKey]);
				}
			}
			else if (NoCaseCompare(Key, "equipment") == 0)
			{
				Json::Value Equipment = Predicate[Key];
				if (!Equipment.isObject())
				{
					LOGWARNING("Loot table: Condition \"EntityProperties - equipment\" encountered a Json problem, dropping condition!");
					continue;
				}
				for (const auto & EquipmentKey : Equipment.getMemberNames())
				{
					if (NoCaseCompare(EquipmentKey, "mainhand") == 0)
					{
						m_MainHand = cMatchTool(Equipment[EquipmentKey]);
					}
					else if (NoCaseCompare(EquipmentKey, "offhand") == 0)
					{
						m_OffHand = cMatchTool(Equipment[EquipmentKey]);
					}
					else if (NoCaseCompare(EquipmentKey, "head") == 0)
					{
						m_Head = cMatchTool(Equipment[EquipmentKey]);
					}
					else if (NoCaseCompare(EquipmentKey, "chest") == 0)
					{
						m_Chest = cMatchTool(Equipment[EquipmentKey]);
					}
					else if (NoCaseCompare(EquipmentKey, "legs") == 0)
					{
						m_Legs = cMatchTool(Equipment[EquipmentKey]);
					}
					else if (NoCaseCompare(EquipmentKey, "feet") == 0)
					{
						m_Feet = cMatchTool(Equipment[EquipmentKey]);
					}
				}
			}
			else if (NoCaseCompare(Key, "flags") == 0)
			{
				Json::Value FlagsObject = Predicate[Key];
				if (FlagsObject.isArray())
				{
					LOGWARNING("Loot table: Condition \"EntityProperties - flags\" encountered a Json problem, dropping condition!");
					continue;
				}
				for (const auto & Flag : FlagsObject.getMemberNames())
				{
					if ((NoCaseCompare(Flag, "is_on_fire") == 0) ||
						(NoCaseCompare(Flag, "IsOnFire") == 0))
					{
						m_IsOnFire = FlagsObject[Flag].asBool();
					}
					else if (
						(NoCaseCompare(Flag, "is_sneaking") == 0) ||
						(NoCaseCompare(Flag, "IsSneaking") == 0))
					{
						m_IsSneaking = FlagsObject[Flag].asBool();
					}
					else if (
						(NoCaseCompare(Flag, "is_sprinting") == 0) ||
						(NoCaseCompare(Flag, "IsSprinting") == 0))
					{
						m_IsSprinting = FlagsObject[Flag].asBool();
					}
					else if (
						(NoCaseCompare(Flag, "is_swimming") == 0) ||
						(NoCaseCompare(Flag, "IsSwimming") == 0))
					{
						m_IsSwimming = FlagsObject[Flag].asBool();
					}
					else if (
						(NoCaseCompare(Flag, "is_baby") == 0) ||
						(NoCaseCompare(Flag, "IsBaby") == 0))
					{
						m_IsBaby = FlagsObject[Flag].asBool();
					}
				}
			}
			else if (NoCaseCompare(Key, "location") == 0)
			{
				m_Location = cLocationCheck(Predicate[Key]);
			}
			else if (NoCaseCompare(Key, "nbt") == 0)
			{
				m_NBT = Predicate[Key].asString();
			}
			else if (NoCaseCompare(Key, "player") == 0)
			{
				Json::Value PlayerObject = Predicate[Key];
				if (PlayerObject.isArray())
				{
					LOGWARNING("Loot table: Condition \"EntityProperties - Player\" encountered a Json problem, dropping condition!");
					continue;
				}
				m_Player = true;  // Flag if the player should be checked - fails if entity is not player
				for (const auto & PlayerKey : PlayerObject.getMemberNames())
				{
					if (NoCaseCompare(PlayerKey, "advancements") == 0)
					{
						// TODO: 14.09.2020 - Add when implemented - 12xx12
						LOGWARNING("Loot table: Advancements are not supported in the condition \"EntityProperties\"");
					}
					else if (NoCaseCompare(PlayerKey, "gamemode") == 0)
					{
						// Allowed values: survival, adventure, creative or spectator
						AString Gamemode = NamespaceConverter(PlayerObject[PlayerKey].asString());
						if (NoCaseCompare(Gamemode, "survival") == 0)
						{
							m_Gamemode = eGameMode_Survival;
						}
						else if (NoCaseCompare(Gamemode, "adventure") == 0)
						{
							m_Gamemode = eGameMode_Adventure;
						}
						else if (NoCaseCompare(Gamemode, "creative") == 0)
						{
							m_Gamemode = eGameMode_Creative;
						}
						else if (NoCaseCompare(Gamemode, "spectator") == 0)
						{
							m_Gamemode = eGameMode_Spectator;
						}
					}
					else if (NoCaseCompare(PlayerKey, "level") == 0)
					{
						MinMaxRange<int>(PlayerObject[PlayerKey], m_LevelMin, m_LevelMax);
					}
					else if (NoCaseCompare(PlayerKey, "recipes") == 0)
					{
						// TODO: 16.06.2020 - 12xx12
						LOGWARNING("Loot table: Recipes are not supported in the condition \"EntityProperties\"");
					}
					else if (NoCaseCompare(PlayerKey, "stats") == 0)
					{
						// TODO: 16.06.2020 - 12xx12
						LOGWARNING("Loot table: Statistics are not supported in the condition \"EntityProperties\"");
					}
				}
			}
			else if (NoCaseCompare(Key, "team") == 0)
			{
				m_Team = Predicate[Key].asString();
			}
			else if (NoCaseCompare(Key, "type") == 0)
			{
				auto Type = Predicate[Key].asString();

				if (Type.empty())
				{
					LOGWARNING("Loot table: Condition \"EntityProperties - Type\" encountered a Json problem, dropping condition!");
					continue;
				}
				// todo
			}
			else if (
				(NoCaseCompare(Key, "target_entity") == 0) ||
				(NoCaseCompare(Key, "TargetEntity") == 0))
			{
				m_TargetEntity = new cEntityProperties(Predicate[Key], eDest::This);
			}
			else if (NoCaseCompare(Key, "vehicle") == 0)
			{
				// Note: this might create looooooooooooooooooooooooooong (even infinite) loops... BUT the user must write any loop depth into the json. So this is limited by the users disk space and can also be abused in vanilla. I thought about limiting the depth but if the user want's  to break the server he shall.
				m_Vehicle = new cEntityProperties(Predicate[Key], eDest::This);
			}
		}  // Predicates
	}

	bool cEntityProperties::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		// If the Destination entity is KillerPlayer and the killer is not player return false
		if (m_Dest == eDest::KillerPlayer)
		{
			if (!a_World.DoWithEntityByID(a_KillerID, [&](cEntity & a_Entity)
			{
				return a_Entity.IsPlayer();
			}))
			{
				return false;
			}
		}

		bool Res = true;

		// Position check
		if (m_Dest != eDest::This)
		{
			Vector3d KilledPos;
			auto KilledCallback = [&](cEntity & a_Entity)
			{
				KilledPos = a_Entity.GetPosition();
				return true;
			};

			Vector3d KillerPos;
			auto KillerCallback = [&](cEntity & a_Entity)
			{
				KillerPos = a_Entity.GetPosition();
				return true;
			};

			if ((m_AbsoluteMin != 0) ||
				(m_AbsoluteMax != std::numeric_limits<float>::max()))
			{
				a_World.DoWithEntityByID(a_KilledID, KilledCallback);
				a_World.DoWithEntityByID(a_KillerID, KillerCallback);

				float Dist = std::abs(sqrt(
					(KillerPos.x - KilledPos.x) * (KillerPos.x - KilledPos.x) +
					(KillerPos.y - KilledPos.y) * (KillerPos.y - KilledPos.y) +
					(KillerPos.z - KilledPos.z) * (KillerPos.z - KilledPos.z)));
				Res &= (m_AbsoluteMin <= Dist) && (m_AbsoluteMax >= Dist);
			}
			if ((m_HorizontalMin != 0) ||
				(m_HorizontalMax != std::numeric_limits<float>::max()))
			{
				a_World.DoWithEntityByID(a_KilledID, KilledCallback);
				a_World.DoWithEntityByID(a_KillerID, KillerCallback);

				float Dist = std::abs(sqrt(
					(KillerPos.x - KilledPos.x) * (KillerPos.x - KilledPos.x) +
					(KillerPos.z - KilledPos.z) * (KillerPos.z - KilledPos.z)));
				Res &= (m_HorizontalMin <= Dist) && (m_HorizontalMax >= Dist);
			}
			if ((m_XMin != 0) || (m_XMax != std::numeric_limits<float>::max()))
			{
				a_World.DoWithEntityByID(a_KilledID, KilledCallback);
				a_World.DoWithEntityByID(a_KillerID, KillerCallback);

				float Dist = std::abs(sqrt((KillerPos.x - KilledPos.x) * (KillerPos.x - KilledPos.x)));
				Res &= (m_AbsoluteMin <= Dist) && (m_AbsoluteMax >= Dist);
			}
			if ((m_YMin != 0) || (m_YMax != std::numeric_limits<float>::max()))
			{
				a_World.DoWithEntityByID(a_KilledID, KilledCallback);
				a_World.DoWithEntityByID(a_KillerID, KillerCallback);

				float Dist = std::abs(sqrt((KillerPos.y - KilledPos.y) * (KillerPos.y - KilledPos.y)));
				Res &= (m_AbsoluteMin <= Dist) && (m_AbsoluteMax >= Dist);
			}
			if ((m_ZMin != 0) || (m_ZMax != std::numeric_limits<float>::max()))
			{
				a_World.DoWithEntityByID(a_KilledID, KilledCallback);
				a_World.DoWithEntityByID(a_KillerID, KillerCallback);

				float Dist = std::abs(sqrt((KillerPos.z - KilledPos.z) * (KillerPos.z - KilledPos.z)));
				Res &= (m_AbsoluteMin <= Dist) && (m_AbsoluteMax >= Dist);
			}
		}
		UInt32 DestID;
		if (m_Dest == eDest::This)
		{
			DestID = a_KilledID;
		}
		else
		{
			DestID = a_KillerID;
		}

		// Effects check
		if (!m_Effects.empty())
		{
			auto EffectCheckCallback = [&](cEntity & a_Entity)
			{
				bool CallbackRes = true;
				// This is a bad workaround to prevent checking for any entity type and then casting to it.
				const auto & Player = static_cast<const cPlayer &>(a_Entity);
				const auto & Effects = Player.GetEntityEffects();
				for (const auto Effect : m_Effects)
				{
					auto * EffectPrt = Effects.at(Effect.first);
					CallbackRes &=
						((Effect.second.m_AmplifierMin <= EffectPrt->GetIntensity()) &&
						(Effect.second.m_AmplifierMax >= EffectPrt->GetIntensity())
					);
					CallbackRes &=
						((Effect.second.m_DurationMin <= EffectPrt->GetDuration()) &&
						(Effect.second.m_DurationMax >= EffectPrt->GetDuration())
					);
				}
				return CallbackRes;
			};
			Res &= a_World.DoWithEntityByID(DestID, EffectCheckCallback);
		}

		// Equipment Check
		if (m_MainHand.IsActive())
		{
			auto Callback = [&](cEntity & a_Entity)
			{
				if (!a_Entity.IsPlayer())
				{
					return true;
				}
				const auto & Player = static_cast<cPlayer & >(a_Entity);
				return m_MainHand(Player.GetEquippedItem());
			};
			Res &= a_World.DoWithEntityByID(DestID, Callback);
		}

		if (m_OffHand.IsActive())
		{
			auto Callback = [&](cEntity & a_Entity)
			{
				return m_OffHand(a_Entity.GetOffHandEquipedItem());
			};
			Res &= a_World.DoWithEntityByID(DestID, Callback);
		}

		if (m_Head.IsActive())
		{
			auto Callback = [&](cEntity & a_Entity)
			{
				return m_Head(a_Entity.GetEquippedHelmet());
			};
			Res &= a_World.DoWithEntityByID(DestID, Callback);
		}

		if (m_Chest.IsActive())
		{
			auto Callback = [&](cEntity & a_Entity)
			{
				return m_Chest(a_Entity.GetEquippedChestplate());
			};
			Res &= a_World.DoWithEntityByID(DestID, Callback);
		}

		if (m_Legs.IsActive())
		{
			auto Callback = [&](cEntity & a_Entity)
			{
				return m_Legs(a_Entity.GetEquippedLeggings());
			};
			Res &= a_World.DoWithEntityByID(DestID, Callback);
		}

		if (m_Feet.IsActive())
		{
			auto Callback = [&](cEntity & a_Entity)
			{
				return m_Feet(a_Entity.GetEquippedBoots());
			};
			Res &= a_World.DoWithEntityByID(DestID, Callback);
		}

		auto FlagCallback = [&](cEntity & a_Entity)
		{
			if (m_IsOnFire)
			{
				Res &= a_Entity.IsOnFire();
			}
			if (m_IsSneaking)
			{
				Res &= a_Entity.IsCrouched();
			}
			if (m_IsSprinting)
			{
				Res &= a_Entity.IsSprinting();
			}
			if (m_IsSwimming)
			{
				Res &= a_Entity.IsInWater();
			}
			if (m_IsBaby)
			{
				if (a_Entity.IsMob())
				{
					const auto & Monster = static_cast<cMonster &>(a_Entity);
					Res &= Monster.IsBaby();
				}
			}
			return true;
		};

		a_World.DoWithEntityByID(DestID, FlagCallback);

		Res &= m_Location(a_World, a_Noise, a_Pos, DestID, DestID);

		if (!m_NBT.empty())
		{
			// Add NBT check here
		}

		if (m_Player)
		{
			if (m_Gamemode != eGameMode_NotSet)
			{
				Res &= a_World.DoWithEntityByID(DestID, [&] (cEntity & a_Entity)
				{
					if (!a_Entity.IsPlayer())
					{
						return false;
					}
					const auto & Player = static_cast<cPlayer &>(a_Entity);
					return (Player.GetGameMode() == m_Gamemode);
				});
			}
			if ((m_LevelMin != 0) ||
				(m_LevelMax != std::numeric_limits<int>::max()))
			{
				Res &= a_World.DoWithEntityByID(DestID, [&] (cEntity & a_Entity)
				{
					if (!a_Entity.IsPlayer())
					{
						return false;
					}
					const auto & Player = static_cast<cPlayer &>(a_Entity);
					return ((m_LevelMin <= Player.GetXpLevel()) && (Player.GetXpLevel() <= m_LevelMax));
				});
			}
			// Add other player checks here!
		}
		if (!m_Team.empty())
		{
			Res &= a_World.DoWithEntityByID(DestID, [&] (cEntity & a_Entity)
			{
				if (!a_Entity.IsPlayer())
				{
					return true;
				}
				const auto & Player = static_cast<cPlayer &>(a_Entity);
				return (NoCaseCompare(Player.GetTeam()->GetName(), m_Team) == 0);
			});
		}

		// Entity type

		if (m_TargetEntity->IsActive())
		{
			UInt32 Target;
			a_World.DoWithEntityByID(DestID, [&] (cEntity & a_Entity)
			{
				if (a_Entity.IsMob())
				{
					Target = static_cast<cMonster &>(a_Entity).GetTarget()->GetUniqueID();
				}
				return true;
			});

			Res &= m_TargetEntity->operator()(a_World, a_Noise, a_Pos, Target, Target);
		}

		if (m_Vehicle->IsActive())
		{
			UInt32 Vehicle;
			a_World.DoWithEntityByID(DestID, [&](cEntity & a_Entity)
			{
				if (a_Entity.IsPawn())
				{
					Vehicle = static_cast<cPawn &>(a_Entity).GetAttached()->GetUniqueID();
				}
				return true;
			});

			Res &= m_TargetEntity->operator()(a_World, a_Noise, a_Pos, Vehicle, Vehicle);
		}
		return Res;
	}

////////////////////////////////////////////////////////////////////////////////
// cEntityScores

	cEntityScores::cEntityScores(const Json::Value & a_Value) {}




	bool cEntityScores::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		return true;
	}

////////////////////////////////////////////////////////////////////////////////
// cInverted

	cInverted::cInverted(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"Inverted\" encountered a Json problem, dropping condition!");
			return;
		}
		Json::Value Term;
		if (a_Value.isMember("term"))
		{
			Term = a_Value["term"];
		}
		else if (a_Value.isMember("Term"))
		{
			Term = a_Value["Term"];
		}
		else
		{
			LOGWARNING("Loot table: Condition \"Inverted\" is missing it's sub condition, dropping condition!");
		}
		m_Conditions.emplace_back(ParseCondition(Term));
		m_Active = true;
	}





	bool cInverted::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource) const
	{
		ACTIVECHECK
		return !std::visit(VISITCONDITION, m_Conditions[0].m_Parameter);
	}


////////////////////////////////////////////////////////////////////////////////
// cKilledByPlayer

	cKilledByPlayer::cKilledByPlayer(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"KilledByPlayer\" encountered a Json problem, dropping function!");
			return;
		}
		m_Active = true;
		if (a_Value.isMember("inverse"))
		{
			m_Inverse = a_Value["inverse"].asBool();
		}
		else if (a_Value.isMember("Inverse"))
		{
			m_Inverse = a_Value["Inverse"].asBool();
		}
	}





	bool cKilledByPlayer::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		auto Callback = [&](cEntity & a_Entity)
		{
			return (a_Entity.GetEntityType() == cEntity::etPlayer);
		};
		return !(m_Inverse ^ a_World.DoWithEntityByID(a_KillerID, Callback));	 // True if both are the same
	}


////////////////////////////////////////////////////////////////////////////////
// cLocationCheck

	cLocationCheck::cLocationCheck(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"LocationCheck\" encountered a Json problem, dropping function!");
			return;
		}
		m_Active = true;

		for (const auto & Key : a_Value.getMemberNames())
		{
			if (NoCaseCompare(Key, "offsetX") == 0)
			{
				m_OffsetX = a_Value[Key].asInt();
			}
			else if (NoCaseCompare(Key, "offsetY") == 0)
			{
				m_OffsetY = a_Value[Key].asInt();
			}
			else if (NoCaseCompare(Key, "offsetZ") == 0)
			{
				m_OffsetZ = a_Value[Key].asInt();
			}
			else if (
				(NoCaseCompare(Key, "predicate") == 0) ||
				(NoCaseCompare(Key, "location") == 0))
			// Accepts "predicate" and "location" to prevent code duplication in cEntityProperties
			{
				Json::Value PredicateObject = a_Value[Key];

				if (!PredicateObject.isObject())
				{
					LOGWARNING("Loot table: Condition \"LocationCheck\" is missing predicates, dropping condition");
					m_Active = false;
					return;
				}

				for (const auto & PredicateKey : PredicateObject.getMemberNames())
				{
					if (NoCaseCompare(PredicateKey, "biome") == 0)
					{
						m_Biome = StringToBiome(NamespaceConverter(PredicateObject[PredicateKey].asString()));
						// Todo: this might fail due to changed biome names in 1.13
					}  // "biome"
					else if (NoCaseCompare(PredicateKey, "block") == 0)
					{
						Json::Value BlockObject;

						if (!BlockObject.isObject())
						{
							LOGWARNING("Loot table: Condition \"LocationCheck - Block\" is missing a Json object to describe it's parameters. Dropping part of the condition!");
							continue;
						}

						for (const auto & BlockKey : BlockObject.getMemberNames())
						{
							if (NoCaseCompare(BlockKey, "block") == 0)
							{
								m_BlockState = cBlockStateProperty(BlockObject);
							}
							else if (NoCaseCompare(BlockKey, "tag") == 0)
							{
								m_BlockTag = NamespaceConverter(BlockObject[BlockKey].asString());
							}
							else if (NoCaseCompare(BlockKey, "nbt") == 0)
							{
								m_BlockNBT = NamespaceConverter(BlockObject[BlockKey].asString());
							}
						}  // for BlockKey
					}	   // "block"
					else if (NoCaseCompare(PredicateKey, "dimension") == 0)
					{
						// Possible values: overworld, the_nether, the_end
						AString Dimension = PredicateObject[PredicateKey].asString();
						if (NoCaseCompare(Dimension, "overworld") == 0)
						{
							m_Dimension = eDimension::dimOverworld;
						}
						else if (
							(NoCaseCompare(Dimension, "the_nether") == 0) ||
							NoCaseCompare(Dimension, "TheNether") == 0)
						{
							m_Dimension = eDimension::dimNether;
						}
						else if (
							(NoCaseCompare(Dimension, "the_end") == 0) ||
							NoCaseCompare(Dimension, "TheEnd") == 0)
						{
							m_Dimension = eDimension::dimEnd;
						}
						else
						{
							LOGWARNING("Loot table: Got unknown dimension in condition \"LocationCheck\": %s", Dimension);
						}
					}  // "dimension"
					else if (NoCaseCompare(PredicateKey, "feature") == 0)
					{
						m_Feature = PredicateObject[PredicateKey].asString();
					}
					else if (NoCaseCompare(PredicateKey, "fluid") == 0)
					{
						Json::Value FluidObject;

						if (!FluidObject.isObject())
						{
							LOGWARNING("Loot table: Condition \"LocationCheck - Fluid\" is missing a Json object to describe it's parameters. Dropping part of the condition!");
							continue;
						}

						for (const auto & FluidKey : FluidObject.getMemberNames())
						{
							if (NoCaseCompare(FluidKey, "block") == 0)
							{
								m_FluidState = cBlockStateProperty(FluidObject);
							}
							else if (NoCaseCompare(FluidKey, "tag") == 0)
							{
								m_FluidTag = NamespaceConverter(FluidObject[FluidKey].asString());
							}
						}
					}  // "fluid"
					else if (NoCaseCompare(PredicateKey, "light") == 0)
					{
						MinMaxRange(PredicateObject[PredicateKey], m_LightMin, m_LightMax);
					}
					else if (NoCaseCompare(PredicateKey, "position") == 0)
					{
						Json::Value PositionObject;

						if (!PositionObject.isObject())
						{
							LOGWARNING("Loot table: Condition \"LocationCheck - Position\" is missing a Json object to describe it's parameters. Dropping part of the condition!");
							continue;
						}

						for (const auto & PosKey : PositionObject.getMemberNames())
						{
							if (NoCaseCompare(PosKey, "x") == 0)
							{
								MinMaxRange(PositionObject[PosKey], m_XMin, m_XMax);
							}
							else if (NoCaseCompare(PosKey, "y") == 0)
							{
								MinMaxRange(PositionObject[PosKey], m_YMin, m_YMax);
							}
							else if (NoCaseCompare(PosKey, "z") == 0)
							{
								MinMaxRange(PositionObject[PosKey], m_ZMin, m_ZMax);
							}
						}
					}  // "position"
					else if (NoCaseCompare(PredicateKey, "smokey") == 0)
					{
						LOGWARNING("Loot table: Cuberite doesn't support campfires yet. Dropping part of the condition \"LocationCheck - Smokey\"");
						m_Smokey = PredicateObject[PredicateKey].asBool();
					}
				}  // for PredicateKey : PredicateObject
			}	   // "predicate"
		}		   // for Key : a_Value
	}





	bool cLocationCheck::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		auto Pos = a_Pos;
		Pos += {m_OffsetX, m_OffsetY, m_OffsetZ};

		bool Res = true;

		if (m_Biome != EMCSBiome::biInvalidBiome)
		{
			Res &= (a_World.GetBiomeAt(Pos.x, Pos.z) == m_Biome);
		}
		Res &= m_BlockState(a_World, Pos);

		if (!m_BlockTag.empty())
		{
			Res &= ItemTag::GetItems(ItemTag::eItemTags(m_BlockTag)).Contains(cItem(a_World.GetBlock(a_Pos)));
		}

		// Block NBT
		// Note: the following code is just an example this is TODO
		/*
		Res &= (a_World.GetBlockNBTAt(Pos) == m_NBT)
		*/
		if (m_Dimension != eDimension::dimNotSet)
		{
			Res &= (a_World.GetDimension() == m_Dimension);
		}

		// TODO: 13.09.2020 - Add when feature is implemented - 12xx12
		if (!m_Feature.empty())
		{
		}

		Res &= m_FluidState(a_World, Pos);

		if (!m_FluidTag.empty())
		{
			auto Block = a_World.GetBlock(a_Pos);
			if (m_FluidTag == "Lava")
			{
				Res &= ((Block == E_BLOCK_LAVA) || (Block == E_BLOCK_STATIONARY_LAVA));
			}
			else if (m_FluidTag == "Water")
			{
				Res &= ((Block == E_BLOCK_WATER) || (Block == E_BLOCK_STATIONARY_WATER));
			}
		}

		if ((m_LightMin != 0) || (m_LightMax != 100))
		{
			int Light = a_World.GetBlockBlockLight(Pos);
			Res &= ((Light >= m_LightMin) && (Light <= m_LightMax));
		}

		if ((m_XMin != std::numeric_limits<double>::min()) ||
			(m_XMax != std::numeric_limits<double>::max()))
		{
			Res &= ((Pos.x >= m_XMin) && (Pos.x <= m_XMax));
		}

		if ((m_YMin != std::numeric_limits<double>::min()) ||
			(m_YMax != std::numeric_limits<double>::max()))
		{
			Res &= ((Pos.y >= m_YMin) && (Pos.y <= m_YMax));
		}

		if ((m_ZMin != std::numeric_limits<double>::min()) ||
			(m_ZMax != std::numeric_limits<double>::max()))
		{
			Res &= ((Pos.z >= m_ZMin) && (Pos.z <= m_ZMax));
		}

		// TODO: 13.09.2020 - Add when campfire is implemented - 12xx12
		/*
		if (m_Smokey)
		{

		}
		*/
		return Res;
	}


////////////////////////////////////////////////////////////////////////////////
// cMatchTool

	cMatchTool::cMatchTool(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"MatchTool\" encountered a Json problem, dropping condition!");
			return;
		}
		m_Active = true;
		// m_Item.FromJson(a_Value) is not used because this follows the minecraft naming convention
		Json::Value Predicates;
		if (a_Value.isMember("predicate"))
		{
			Predicates = a_Value["predicate"];
		}
		else if (a_Value.isMember("Predicate"))
		{
			Predicates = a_Value["Predicate"];
		}
		if (!Predicates.isObject())
		{
			LOGWARNING("Loot table: Condition \"MatchTool\" is missing predicates");
			return;
		}
		for (const auto & Key : Predicates.getMemberNames())
		{
			if (NoCaseCompare(Key, "count") == 0)
			{
				MinMaxRange<int>(Predicates[Key], m_CountMin, m_CountMax);
			}
			else if (NoCaseCompare(Key, "durability") == 0)
			{
				MinMaxRange<int>(Predicates[Key], m_DurabilityMin, m_DurabilityMax);
			}
			else if (NoCaseCompare(Key, "enchantments") == 0)
			{
				Json::Value Enchantments = Predicates[Key];
				if (!Enchantments.isArray())
				{
					LOGWARNING("Loot table: Unknown entry provided for enchantments in in condition \"MatchTool\"");
				}
				for (unsigned int i = 0; i < Enchantments.size(); i++)
				{
					Json::Value EnchantmentObject = Enchantments[i];
					cEnchantments Enchantment;
					int EnchantmentID;
					int Min = 0, Max = 100;
					if (!EnchantmentObject.isObject())
					{
						LOGWARNING("Loot table: Unknown entry provided for enchantment in in condition \"MatchTool\"");
						continue;
					}
					for (const auto & EnchantmentKey : EnchantmentObject.getMemberNames())
					{
						if (NoCaseCompare(EnchantmentKey, "enchantment") == 0)
						{
							EnchantmentID = cEnchantments::StringToEnchantmentID(NamespaceConverter(EnchantmentObject[EnchantmentKey].asString()));
							Enchantment = cEnchantments(NamespaceConverter(EnchantmentObject[EnchantmentKey].asString()));
						}
						else if (NoCaseCompare(EnchantmentKey, "levels") == 0)
						{
							MinMaxRange<int>(EnchantmentObject[EnchantmentKey], Min, Max);
						}
					}
					m_EnchantmentsMin.Add(Enchantment);
					m_EnchantmentsMin.SetLevel(EnchantmentID, Min);
					m_EnchantmentsMax.Add(Enchantment);
					m_EnchantmentsMax.SetLevel(EnchantmentID, Max);
				}
			}
			else if (
				(NoCaseCompare(Key, "sorted_enchantments") == 0) ||
				(NoCaseCompare(Key, "SortedEnchantments") == 0))
			{
				Json::Value Enchantments = Predicates[Key];
				if (!Enchantments.isArray())
				{
					LOGWARNING("Loot table: Unknown entry provided for enchantments in condition \"MatchTool\"");
				}
				for (unsigned int i = 0; i < Enchantments.size(); i++)
				{
					Json::Value EnchantmentObject = Enchantments[i];
					cEnchantments Enchantment;
					int EnchantmentID;
					int Min = 0, Max = 100;
					if (!EnchantmentObject.isObject())
					{
						LOGWARNING("Loot table: Unknown entry provided for enchantment in condition \"MatchTool\"");
						continue;
					}
					for (const auto & EnchantmentKey : EnchantmentObject.getMemberNames())
					{
						if (NoCaseCompare(EnchantmentKey, "enchantment") == 0)
						{
							EnchantmentID =cEnchantments::StringToEnchantmentID(NamespaceConverter(EnchantmentObject[EnchantmentKey].asString()));
							Enchantment = cEnchantments(NamespaceConverter(EnchantmentObject[EnchantmentKey].asString()));
						}
						else if (NoCaseCompare(EnchantmentKey, "levels") == 0)
						{
							MinMaxRange(EnchantmentObject[EnchantmentKey], Min, Max);
						}
					}
					m_StoredEnchantmentsMin.Add(Enchantment);
					m_StoredEnchantmentsMin.SetLevel(EnchantmentID, Min);
					m_StoredEnchantmentsMax.Add(Enchantment);
					m_StoredEnchantmentsMax.SetLevel(EnchantmentID, Max);
				}
			}
			else if (NoCaseCompare(Key, "item") == 0)
			{
				if (!StringToItem(NamespaceConverter(Predicates[Key].asString()), m_Item))
				{
					LOGWARNING("Loot table: Unknown item provided in condition \"MatchTool\"");
				}
			}
			else if (NoCaseCompare(Key, "nbt") == 0)
			{
				// TODO: 10.09.2020 - Add when implemented - 12xx12
				LOGWARNING("Loot table: NBT for items is not yet supported!");
				continue;
				m_NBT = Predicates[Key].asString();
			}
			else if (NoCaseCompare(Key, "potion") == 0)
			{
				// TODO: 10.09.2020 - Add when implemented - 12xx12
				LOGWARNING("Loot table: NBT for items is not yet supported!");
			}
			else if (NoCaseCompare(Key, "tag") == 0)
			{
				m_Tag = Predicates[Key].asString();
			}
		}
	}





	bool cMatchTool::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK

		cItem Item;
		auto Callback = [&](cEntity & a_Entity)
		{
			if (a_Entity.GetEntityType() != cEntity::etPlayer)
			{
				Item = a_Entity.GetOffHandEquipedItem();
			}
			else
			{
				const auto & Player = static_cast<const cPlayer &>(a_Entity);
				Item = Player.GetEquippedItem();
			}
			return true;
		};

		if (!a_World.DoWithEntityByID(a_KillerID, Callback))  // Callback failed
		{
			return true;
		}
		return this->operator()(Item);
	}




	bool cMatchTool::operator()(const cItem & a_Item) const
	{
		ACTIVECHECK
		bool Res = true;

		// Checks count
		if ((m_CountMin != 0) || (m_CountMax < 65))
		{
			Res &= ((a_Item.m_ItemCount >= m_CountMin) && (a_Item.m_ItemCount <= m_CountMax));
		}

		// Checks durability
		if ((m_DurabilityMin != 0) ||
			(m_DurabilityMax != std::numeric_limits<short>::max()))
		{
			int Durability = a_Item.GetMaxDamage() - a_Item.m_ItemDamage;
			Res &= ((Durability >= m_DurabilityMin) && (Durability <= m_DurabilityMax));
		}

		// Checks enchantments on book
		if ((!m_EnchantmentsMin.IsEmpty() || !m_EnchantmentsMax.IsEmpty()) &&
			(a_Item.m_ItemType != E_ITEM_ENCHANTED_BOOK))
		{
			for (const auto & Enchantment : m_EnchantmentsMin)
			{
				Res &= (a_Item.m_Enchantments.GetLevel(Enchantment.first) >= Enchantment.second);
			}

			for (const auto & Enchantment : m_EnchantmentsMax)
			{
				Res &= (a_Item.m_Enchantments.GetLevel(Enchantment.first) <= Enchantment.second);
			}
		}

		// Check enchantments on book
		if ((!m_StoredEnchantmentsMin.IsEmpty() ||
			!m_StoredEnchantmentsMax.IsEmpty()) &&
			(a_Item.m_ItemType == E_ITEM_ENCHANTED_BOOK))
		{
			for (const auto & Enchantment : m_StoredEnchantmentsMin)
			{
				Res &= (a_Item.m_Enchantments.GetLevel(Enchantment.first) >= Enchantment.second);
			}

			for (const auto & Enchantment : m_StoredEnchantmentsMax)
			{
				Res &= (a_Item.m_Enchantments.GetLevel(Enchantment.first) <= Enchantment.second);
			}
		}

		// Checks item type
		if (m_Item.m_ItemType != 0)
		{
			Res &= m_Item.IsSameType(a_Item);
		}

		// TODO: 11.09.2020 - Add when implemented - Checks NBT Tag. - 12xx12
		if (!m_NBT.empty())
		{
		}

		// Todo: Checks if item belongs to specified tag
		if (!m_Tag.empty())
		{
		}

		return Res;
	}

////////////////////////////////////////////////////////////////////////////////
// cRandomChance

	cRandomChance::cRandomChance(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"RandomChance\" encountered a Json problem, dropping condition!");
			return;
		}
		if (a_Value.isMember("chance"))
		{
			m_Chance = a_Value["chance"].asFloat();
		}
		else if (a_Value.isMember("Chance"))
		{
			m_Chance = a_Value["Chance"].asFloat();
		}
		else
		{
			LOGWARNING("Loot table: Condition \"RandomChance\" is missing chance, dropping condition!");
			return;
		}
		m_Active = true;
	}




	bool cRandomChance::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		return fmod(a_Noise.IntNoise1D(a_World.GetWorldAge()), 1.0f) < m_Chance;
	}

////////////////////////////////////////////////////////////////////////////////
// cRandomChanceWithLooting

	cRandomChanceWithLooting::cRandomChanceWithLooting(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"RandomChanceWithLooting\" encountered a Json problem, dropping condition!");
			return;
		}
		m_Active = true;
		if (a_Value.isMember("looting_multiplier"))
		{
			m_LootingMultiplier = a_Value["looting_multiplier"].asFloat();
		}
		else if (a_Value.isMember("LootingMultiplier"))
		{
			m_LootingMultiplier = a_Value["LootingMultiplier"].asFloat();
		}

		if (a_Value.isMember("chance"))
		{
			m_Chance = a_Value["chance"].asFloat();
		}
		else if (a_Value.isMember("Chance"))
		{
			m_Chance = a_Value["Chance"].asFloat();
		}
	}




	bool cRandomChanceWithLooting::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		float Rnd = fmod(a_Noise.IntNoise1D(a_World.GetWorldAge()), 1.0f);

		auto Callback = [&](cEntity & a_Entity)
		{
			if (a_Entity.GetEntityType() != cEntity::etPlayer)
			{
				return true;
			}
			const auto & Player = static_cast<const cPlayer &>(a_Entity);
			int Looting = Player.GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchLooting);
			return m_Chance + Looting * m_LootingMultiplier > Rnd;
		};

		return a_World.DoWithEntityByID(a_KillerID, Callback);
	}


////////////////////////////////////////////////////////////////////////////////
// cReference
	cReference::cReference(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"Reference\" encountered a Json problem, dropping condition!");
			return;
		}
		// Todo
		m_Active = true;
	}

	bool cReference::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		bool Res = true;
		// Todo
		return Res;
	}


////////////////////////////////////////////////////////////////////////////////
// cSurvivesExplosion

	bool cSurvivesExplosion::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		// Todo
		return true;
	}

////////////////////////////////////////////////////////////////////////////////
// cTableBonus
	cTableBonus::cTableBonus(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"TableBonus\" encountered a Json problem, dropping function!");
			return;
		}
		m_Active = true;
		if (a_Value.isMember("enchantment"))
		{
			m_Enchantment = cEnchantments::StringToEnchantmentID(NamespaceConverter(a_Value["enchantment"].asString()));
		}
		else if (a_Value.isMember("Enchantment"))
		{
			m_Enchantment = cEnchantments::StringToEnchantmentID(NamespaceConverter(a_Value["Enchantment"].asString()));
		}

		Json::Value Chances;
		if (a_Value.isMember("chances"))
		{
			Chances = a_Value["chances"];
		}
		else if (a_Value.isMember("Chances"))
		{
			Chances = a_Value["Chances"];
		}
		if (!Chances.isArray())
		{
			LOGWARNING("Loot table: Condition \"TableBonus - Chances\" encountered a Json problem, dropping function!");
			m_Active = false;
			return;
		}
		else
		{
			for (unsigned int i = 0; i < Chances.size(); i++)
			{
				m_Chances[i] = Chances[i].asFloat();
			}
		}
	}





	bool cTableBonus::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		auto Callback = [&](cEntity & a_Entity)
		{
			if (a_Entity.GetEntityType() != cEntity::etPlayer)
			{
				return true;
			}
			const auto & Player = static_cast<const cPlayer &>(a_Entity);
			unsigned long Level = Player.GetEquippedItem().m_Enchantments.GetLevel(m_Enchantment);

			Level = std::min(Level, (unsigned long) m_Chances.size());
			// this is some funky business going on with MVSC. This sees the return value of .size() as unsigned long long. Clang does see unsigned long...

			return GetRandomProvider().RandReal(0.0f, 1.0f) < m_Chances.at(Level);
		};
		return a_World.DoWithEntityByID(a_KillerID, Callback);
	}


////////////////////////////////////////////////////////////////////////////////
// cTimeCheck
	cTimeCheck::cTimeCheck(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"TimeCheck\" encountered a Json problem, dropping condition!");
			return;
		}
		m_Active = true;
		if (a_Value.isMember("period"))
		{
			m_Period = a_Value["period"].asInt();
		}
		if (a_Value.isMember("Period"))
		{
			m_Period = a_Value["Period"].asInt();
		}
		Json::Value Value;
		if (a_Value.isMember("value"))
		{
			Value = a_Value["value"];
		}
		else if (a_Value.isMember("Value"))
		{
			Value = a_Value["Value"];
		}
		MinMaxRange(Value, m_Min, m_Max);
	}





	bool cTimeCheck::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		return ((a_World.GetTimeOfDay() % m_Period) >= m_Min) &&
			((a_World.GetTimeOfDay() % m_Period) <= m_Max);
	}

////////////////////////////////////////////////////////////////////////////////
// cWeatherCheck

	cWeatherCheck::cWeatherCheck(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Condition \"WeatherCheck\" encountered a Json problem, dropping condition!");
			return;
		}
		m_Active = true;
		if (a_Value.isMember("raining"))
		{
			m_Raining = a_Value["raining"].asBool();
		}
		else if (a_Value.isMember("Raining"))
		{
			m_Raining = a_Value["raining"].asBool();
		}
		else if (a_Value.isMember("thundering"))
		{
			m_Thundering = a_Value["thundering"].asBool();
		}
		else if (a_Value.isMember("Thundering"))
		{
			m_Thundering = a_Value["Thundering"].asBool();
		}
	}




	bool cWeatherCheck::operator()(
		cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
		UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		auto Weather = a_World.GetWeather();
		bool Res = true;
		if (m_Raining)
		{
			Res &= (Weather == eWeather_Rain);
		}
		if (m_Thundering)
		{
			Res &= (Weather == eWeather_ThunderStorm);
		}
		return Res;
	}

#undef ACTIVECHECK
	}  // Namespace Condition

	namespace Function
	{
////////////////////////////////////////////////////////////////////////////////
// cApplyBonus
#define ACTIVECHECK if (!m_Active) { return; }


	cApplyBonus::cApplyBonus(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"ApplyBonus\" encountered a Json problem, dropping function!");
			return;
		}
		Json::Value EnchantmentObject;

		if (a_Value.isMember("enchantment"))
		{
			EnchantmentObject = a_Value["enchantment"];
		}
		else if (a_Value.isMember("Enchantment"))
		{
			EnchantmentObject = a_Value["Enchantment"];
		}

		m_Enchantment = cEnchantments::StringToEnchantmentID(NamespaceConverter(EnchantmentObject.asString()));

		AString Formula;
		if (a_Value.isMember("formula"))
		{
			Formula = NamespaceConverter(a_Value["formula"].asString());
		}
		else if (a_Value.isMember("Formula"))
		{
			Formula = NamespaceConverter(a_Value["Formula"].asString());
		}

		if (Formula == "BinomialWithBonusCount")
		{
			m_Formula = eFormula::BinomialWithBonusCount;
		}
		else if (Formula == "UniformBonusCount")
		{
			m_Formula = eFormula::UniformBonusCount;
		}
		else if (Formula == "OreDrops")
		{
			m_Formula = eFormula::OreDrops;
		}
		else
		{
			LOGWARNING("Loot table: Condition \"ApplyBonus\" got a unknown Formula: %s", Formula);
			return;
		}

		Json::Value Parameters;

		if (a_Value.isMember("parameters"))
		{
			Parameters = a_Value["parameters"];
		}
		else if (a_Value.isMember("Parameters"))
		{
			Parameters = a_Value["Parameters"];
		}

		if (Parameters.isMember("extra"))
		{
			m_Extra = Parameters["extra"].asInt();
		}
		else if (Parameters.isMember("Extra"))
		{
			m_Extra = Parameters["Extra"].asInt();
		}

		if (Parameters.isMember("probability"))
		{
			m_Probability = Parameters["probability"].asFloat();
		}
		else if (Parameters.isMember("Probability"))
		{
			m_Probability = Parameters["Probability"].asFloat();
		}

		if (Parameters.isMember("bonusMultiplier"))
		{
			m_BonusMultiplier = Parameters["bonusMultiplier"].asInt();
		}
		else if (Parameters.isMember("BonusMultiplier"))
		{
			m_BonusMultiplier = Parameters["BonusMultiplier"].asInt();
		}
	}





	void cApplyBonus::operator() (
		cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos,
			UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		int Level;
		auto Callback = [&] (cEntity & a_Entity)
		{
			if (a_Entity.GetEntityType() != cEntity::etPlayer)
			{
				return false;
			}

			auto & Player = static_cast<cPlayer &>(a_Entity);

			Level = Player.GetEquippedItem().m_Enchantments.GetLevel(m_Enchantment);
			return true;
		};

		a_World.DoWithEntityByID(a_KilledID, Callback);

		switch (m_Formula)
		{
			case eFormula::BinomialWithBonusCount:
			{
				// Binomial Probability Distribution with n=level + extra, p=probability
				std::default_random_engine Generator(a_Noise.GetSeed());
				std::binomial_distribution<int> Dist(Level + m_Extra, m_Probability);
				std::vector<int> Values;
				for (int i = 0; i < Level + m_Extra; i++)
				{
					Values[i] = Dist(Generator);
				}
				a_Item.m_ItemCount += Values[a_Noise.IntNoise3DInt(a_Pos * 15) % Values.size()];
				break;
			}
			case eFormula::UniformBonusCount:
			{
				// prob = (0, level * BonusMultiplier)
				a_Item.m_ItemCount += (a_Noise.IntNoise3DInt(a_Pos) / 3) % (int) round(Level * m_BonusMultiplier);
				break;
			}
			case eFormula::OreDrops:
			{
				// Count *= (max(0; random(0..Level + 2) - 1) + 1)
				a_Item.m_ItemCount *= std::max(0, a_Noise.IntNoise1DInt(a_Noise.GetSeed() * a_Item.m_ItemType) % (Level + 2) - 1) + 1;
				break;
			}
			case eFormula::None:
			{
				break;
			}
		}
	}


////////////////////////////////////////////////////////////////////////////////
// cCopyName

	cCopyName::cCopyName(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"CopyName\" encountered a Json problem, dropping function!");
			return;
		}

		if (a_Value.isMember("source"))
		{
			if ((a_Value["source"].asString() == "block entity") || (a_Value["source"].asString() == "BlockEntity"))
			{
				m_Active = true;
			}
		}
		else if (a_Value.isMember("Source"))
		{
			if ((a_Value["Source"].asString() == "block entity") || (a_Value["Source"].asString() == "BlockEntity"))
			{
				m_Active = true;
			}
		}
		else
		{
			LOGWARNING("Loot table: Function wasn't provided a valid source, dropping function");
		}
	}




	void cCopyName::operator()(cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		if (!m_Active)
		{
			return;
		}
		// Todo
		// a_Item.m_CustomName = a_BlockEntity.GetCustomName();
	}


////////////////////////////////////////////////////////////////////////////////
// cCopNBT

	cCopyNbt::cCopyNbt(const Json::Value & a_Value)
	{
		// TODO: 06.09.2020 - Add when implemented - 12xx12
		LOGWARNING("Loot table: NBT for items is not yet supported. Dropping function!");
		return;
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"CopyNbt\" encountered a Json problem, dropping function!");
			return;
		}

		AString Source;
		if (a_Value.isMember("source"))
		{
			Source = NamespaceConverter(a_Value["source"].asString());
		}
		else if (a_Value.isMember("Source"))
		{
			Source = NamespaceConverter(a_Value["Source"].asString());
		}
		else
		{
			LOGWARNING("Loot table: Function \"CopyNBT\" wasn't provided a source, dropping function!");
			return;
		}

		if (Source == "BlockEntity")
		{
			m_Source = eSource::BlockEntity;
		}
		else if (Source == "This")
		{
			m_Source = eSource::This;
		}
		else if (Source == "Killer")
		{
			m_Source = eSource::Killer;
		}
		else if (Source == "KillerPlayer")
		{
			m_Source = eSource::KillerPlayer;
		}
		else
		{
			LOGWARNING("Loot table: Function \"CopyNBT\" was provided a source unknown source: %s, dropping function!", Source);
			return;
		}

		Json::Value Operations;
		if (a_Value.isMember("ops"))
		{
			Operations = a_Value["ops"];
		}
		else if (a_Value.isMember("Ops"))
		{
			Operations = a_Value["Ops"];
		}

		if (!Operations.isArray())
		{
			LOGWARNING("Loot table: Function \"CopyNBT\" was provided an invalid object to get the operations from, dropping functions");
			return;
		}
		m_Active = true;

		for (unsigned int i = 0; i < Operations.size(); i++)
		{
			Json::Value OperationObject = Operations[i];
			if (!OperationObject.isObject())
			{
				continue;
			}
			for (const auto & OperationKey : OperationObject.getMemberNames())
			{
				AString SourcePath, TargetPath;
				eOperation Operation;
				if (NoCaseCompare(OperationKey, "source") == 0)
				{
					SourcePath = NamespaceConverter(OperationObject[OperationKey].asString());
				}
				else if (NoCaseCompare(OperationKey, "target") == 0)
				{
					TargetPath = NamespaceConverter(OperationObject[OperationKey].asString());
				}
				else if (NoCaseCompare(OperationKey, "op") == 0)
				{
					AString Op = NamespaceConverter(OperationObject[OperationKey].asString());
					if (Op == "Replace")
					{
						Operation = eOperation::Replace;
					}
					else if (Op == "Append")
					{
						Operation = eOperation::Append;
					}
					else if (Op == "Merge")
					{
						Operation = eOperation::Merge;
					}
				}
				m_Operations.emplace_back(sOperation{SourcePath, TargetPath, Operation});
			}
		}
	}





	void cCopyNbt::operator()(cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		for (const auto & Operation : m_Operations)
		{
			// Do operation. This depends on the actual implementation
			UNUSED(Operation);
		}
	}


////////////////////////////////////////////////////////////////////////////////
// cCopyState

	cCopyState::cCopyState(const Json::Value & a_Value)
	{
		// TODO: 02.09.2020 - Add when implemented - 12xx12
		LOGWARNING("Loot table: States for blocks is not yet supported, dropping function!");
		return;

		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"CopyState\" encountered a Json problem, dropping function!");
			return;
		}
		for (const auto & ParameterName : a_Value.getMemberNames())
		{

			if (NoCaseCompare(ParameterName, "block") == 0)
			{
				cItem Item;

				if (StringToItem(NamespaceConverter(a_Value[ParameterName].asString()), Item))
				{
					m_Block = Item.m_ItemType;
				}
				else
				{
					LOGWARNING("Loot table: Provided unknown Block for condition \"CopyState\"");
				}
			}
			else if (NoCaseCompare(ParameterName, "properties") == 0)
			{
				Json::Value Properties = a_Value[ParameterName];
				if (!Properties.isArray())
				{
					LOGWARNING("Loot table: Function copy state wasn't provided with a valid object to get it's properties from, dropping function");
					return;
				}

				for (unsigned int i = 0; i < Properties.size(); i++)
				{
					m_Properties.push_back(NamespaceConverter(Properties[i].asString()));
				}
			}
			m_Active = true;
		}
	}

	void cCopyState::operator()(cItem & a_Item, cWorld & a_World, const Vector3i & a_Pos) const
	{
		ACTIVECHECK
		// TODO: 02.09.2020 - Add when implemented - 12xx12
	}

////////////////////////////////////////////////////////////////////////////////
// cCopyState



	cEnchantRandomly::cEnchantRandomly(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"EnchantRandomly\" encountered a Json problem, dropping function!");
			return;
		}
		Json::Value Enchantments;
		if (a_Value.isMember("enchantments"))
		{
			Enchantments = a_Value["enchantments"];
		}
		else if (a_Value.isMember("Enchantments"))
		{
			Enchantments = a_Value["Enchantments"];
		}
		if (Enchantments.isArray())
		{
			for (unsigned int i = 0; i < Enchantments.size(); i++)
			{
				m_EnchantmentLimiter.push_back({1, cEnchantments(NamespaceConverter(Enchantments[i].asString()))});
			}
		}
		else if (!Enchantments.empty())
		{
			LOGWARNING("Loot table: Function \"EnchantRandomly\" got invalid object to get it's allowed functions from");
		}
		m_Active = true;
	}




	void cEnchantRandomly::operator()(cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const
	{
		ACTIVECHECK
		if (!cItem::IsEnchantable(a_Item.m_ItemType))
		{
			LOGWARNING("Loot table: Item %s can not be enchanted in loot table", ItemToString(a_Item));
			return;
		}
		if (!m_EnchantmentLimiter.empty())
		{
			if (a_Item.IsSameType(E_ITEM_BOOK))
			{
				a_Item.m_ItemType = E_ITEM_ENCHANTED_BOOK;
			}
			a_Item.m_Enchantments.Add(cEnchantments::GetRandomEnchantmentFromVector(m_EnchantmentLimiter));
		}
		else  // All are possible
		{
			cWeightedEnchantments EnchantmentLimiter;
			cEnchantments::AddItemEnchantmentWeights(EnchantmentLimiter, a_Item.m_ItemType, 24 + a_Noise.IntNoise3DInt(a_Pos * 20) % 7);
			a_Item.m_Enchantments.Add(cEnchantments::GetRandomEnchantmentFromVector(EnchantmentLimiter));
		}
	}

////////////////////////////////////////////////////////////////////////////////
// cEnchantWithLevels


	cEnchantWithLevels::cEnchantWithLevels(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"EnchantWithLevels\" encountered a Json problem, dropping function!");
			return;
		}

		// TODO: 02.09.2020 - Add treasure when implemented - 12xx12
		if (a_Value.isMember("treasure"))
		{
			m_Treasure = a_Value["treasure"].asBool();
			LOGWARNING("Loot table: Treasure enchantments are not yet supported");
		}
		else if (a_Value.isMember("Treasure"))
		{
			m_Treasure = a_Value["Treasure"].asBool();
			LOGWARNING("Loot table: Treasure enchantments are not yet supported");
		}
		Json::Value LevelsObject;
		if (a_Value.isMember("levels"))
		{
			LevelsObject = a_Value["levels"];
		}
		else if (a_Value.isMember("Levels"))
		{
			LevelsObject = a_Value["Levels"];
		}
		else
		{
			LOGWARNING("Loot table: No levels provided for enchantments in function \"EnchantWithLevels\"");
			return;
		}
		m_Active = true;
		MinMaxRange<int>(LevelsObject, m_LevelsMin, m_LevelsMax);
	}




	void cEnchantWithLevels::operator()(cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const
	{
		ACTIVECHECK
		if (!cItem::IsEnchantable(a_Item.m_ItemType))
		{
			LOGWARNING("Loot table: Item %s can not be enchanted in loot table", ItemToString(a_Item));
			return;
		}
		int Levels = (a_Noise.IntNoise3DInt(a_Pos) / 13) % (m_LevelsMax - m_LevelsMin) + m_LevelsMin;
		a_Item.EnchantByXPLevels(Levels);
		if (a_Item.m_ItemType == E_ITEM_BOOK)
		{
			a_Item.m_ItemType = E_ITEM_ENCHANTED_BOOK;
		}
	}

////////////////////////////////////////////////////////////////////////////////
// cExplorationMap


	cExplorationMap::cExplorationMap(const Json::Value & a_Value)
	{
		// TODO: 02.09.2020 - Add when implemented - 12xx12
		LOGWARNING("Loot table: Exploration maps are not implemented, dropping function!");
		return;
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"ExplorationMap\" encountered a Json problem, dropping function!");
			return;
		}

		for (const auto & Key : a_Value.getMemberNames())
		{
			if (NoCaseCompare(Key, "Destination") == 0)
			{
				m_Destination = NamespaceConverter(a_Value[Key].asString());
			}
			else if (NoCaseCompare(Key, "Decoration") == 0)
			{
				m_Decoration = NamespaceConverter(a_Value[Key].asString());
			}
			else if (NoCaseCompare(Key, "Zoom") == 0)
			{
				m_Zoom = a_Value[Key].asInt();
			}
			else if ((NoCaseCompare(Key, "search_radius") == 0) || (NoCaseCompare(Key, "SearchRadius") == 0))
			{
				m_SearchRadius = a_Value[Key].asInt();
			}
			else if ((NoCaseCompare(Key, "skip_existing_chunks") == 0) || (NoCaseCompare(Key, "SkipExistingChunks") == 0))
			{
				m_SkipExistingChunks = a_Value[Key].asBool();
			}
		}
	}





	void cExplorationMap::operator()(cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		// TODO: 02.09.2020 - Add when implemented - 12xx12
	}

////////////////////////////////////////////////////////////////////////////////
// cExplosionDecay


	void cExplosionDecay::operator() (cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		// Todo
	}

////////////////////////////////////////////////////////////////////////////////
// cFurnaceSmelt

	void cFurnaceSmelt::operator()(cItem & a_Item) const
	{
		auto Recipes = cRoot::Get()->GetFurnaceRecipe();
		auto NewItem = Recipes->GetRecipeFrom(a_Item)->Out;
		a_Item.m_ItemType = NewItem->m_ItemType;
		a_Item.m_ItemDamage = NewItem->m_ItemDamage;
	}

////////////////////////////////////////////////////////////////////////////////
// cFillPlayerHead

	cFillPlayerHead::cFillPlayerHead(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"FillPlayerHead\" encountered a Json problem, dropping function!");
			return;
		}
		AString Entity;
		if (a_Value.isMember("entity"))
		{
			Entity = a_Value["entity"].asString();
		}
		else if (a_Value.isMember("Entity"))
		{
			Entity = a_Value["entity"].asString();
		}
		else
		{
			LOGWARNING("Loot table: Function \"FillPlayerHead\" is missing it's destination, dropping function!");
			return;
		}

		if (NoCaseCompare(Entity, "this") == 0)
		{
			m_Dest = eDest::This;
		}
		else if (NoCaseCompare(Entity, "killer") == 0)
		{
			m_Dest = eDest::Killer;
		}
		else if ((NoCaseCompare(Entity, "killer_player")) || (NoCaseCompare(Entity, "KillerPlayer")))
		{
			m_Dest = eDest::KillerPlayer;
		}
		else
		{
			LOGWARNING("Loot table: Function \"FillPlayerHead\" got unknown destination %s, dropping function!", Entity);
			return;
		}
		m_Active = true;
	}





	void cFillPlayerHead::operator()(cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
		switch (m_Dest)
		{
			case eDest::This:
			{
				a_World.DoWithEntityByID(a_KilledID, [&] (cEntity & a_Entity)
				{
					if (!a_Entity.IsMob())
					{
						return true;
					}
					const auto & Monster = static_cast<cMonster &>(a_Entity);
					MobTypeToHead(Monster.GetMobType(), a_Item);
					return true;
				});
				break;
			}
			case eDest::Killer: break;
			case eDest::KillerPlayer:
			{
				a_World.DoWithEntityByID(a_KillerID, [&] (cEntity & a_Entity)
				{
					if (!a_Entity.IsPlayer())
					{
						return true;
					}
					a_Item.m_ItemType = E_ITEM_HEAD;
					a_Item.m_ItemDamage = E_META_HEAD_PLAYER;
					return true;
				});
				break;
			}
		}
	}




	void cFillPlayerHead::MobTypeToHead(eMonsterType a_Type, cItem & a_Item)
	{
		switch (a_Type)
		{
			case mtCreeper:
			{
				a_Item.m_ItemType = E_BLOCK_HEAD;
				a_Item.m_ItemDamage = E_META_HEAD_CREEPER;
				break;
			}
			case mtEnderDragon:
			{
				a_Item.m_ItemType = E_BLOCK_HEAD;
				a_Item.m_ItemDamage = E_META_HEAD_DRAGON;
				break;
			}
			case mtSkeleton:
			{
				a_Item.m_ItemType = E_BLOCK_HEAD;
				a_Item.m_ItemDamage = E_META_HEAD_SKELETON;
				break;
			}
			case mtWitherSkeleton:
			{
				a_Item.m_ItemType = E_BLOCK_HEAD;
				a_Item.m_ItemDamage = E_META_HEAD_WITHER;
				break;
			}
			case mtZombie:
			{
				a_Item.m_ItemType = E_BLOCK_HEAD;
				a_Item.m_ItemDamage = E_META_HEAD_ZOMBIE;
				break;
			}
			default: break;
		}
	}


////////////////////////////////////////////////////////////////////////////////
// cLimitCount

	cLimitCount::cLimitCount(const Json::Value & a_Value)
	{
		Json::Value LimitObject;
		if (a_Value.isMember("limit"))
		{
			LimitObject = a_Value["limit"];
		}
		else if (a_Value.isMember("Limit"))
		{
			LimitObject = a_Value["Limit"];
		}
		else
		{
			LOGWARNING("Loot table: Missing limit in function \"LimitCount\", dropping function!");
			return;
		}

		MinMaxRange<int>(LimitObject, m_LimitMin, m_LimitMax);
		m_Active = true;
	}





	void cLimitCount::operator()(cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const
	{
		ACTIVECHECK
		int Limit = (a_Noise.IntNoise3DInt(a_Pos) / 13) % (m_LimitMin - m_LimitMax) + m_LimitMin;
		if (a_Item.m_ItemCount > Limit)
		{
			a_Item.m_ItemCount = Limit;
		}
	}

////////////////////////////////////////////////////////////////////////////////
// cLootingEnchant

	cLootingEnchant::cLootingEnchant(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"LootingEnchant\" encountered a Json problem, dropping function!");
			return;
		}
		Json::Value CountObject;
		if (a_Value.isMember("count"))
		{
			CountObject = a_Value["count"];
		}
		else if (a_Value.isMember("Count"))
		{
			CountObject = a_Value["Count"];
		}
		else
		{
			LOGWARNING("Loot table: Function \"LootingEnchant\" is missing it's count object, dropping function!");
			return;
		}
		MinMaxRange<int>(CountObject, m_CountMin, m_CountMax);

		if (a_Value.isMember("limit"))
		{
			m_Limit = a_Value["limit"].asInt();
		}
		else if (a_Value.isMember("Limit"))
		{
			m_Limit = a_Value["Limit"].asInt();
		}
		m_Active = true;
	}




	void cLootingEnchant::operator()(cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KillerID) const
	{
		int Looting = 0;
		auto Callback = [&] (cEntity & a_Entity)
		{
			if (a_Entity.GetEntityType() != cEntity::etPlayer)
			{
				return false;
			}
			auto & Player = static_cast<cPlayer &>(a_Entity);
			Looting = Player.GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchLooting);
			return true;
		};

		if (!a_World.DoWithEntityByID(a_KillerID, Callback))
		{
			return;
		}

		int Count = (a_Noise.IntNoise3DInt(a_Pos) / 11) % (m_CountMin - m_CountMax) + m_CountMin;

		a_Item.m_ItemCount += Looting * Count;

		if ((m_Limit > 0) && (a_Item.m_ItemCount > m_Limit))
		{
			a_Item.m_ItemCount = m_Limit;
		}
	}

////////////////////////////////////////////////////////////////////////////////
// cSetAttributes


	cSetAttributes::cSetAttributes(const Json::Value & a_Value)
	{
		// TODO: 02.09.2020 - Add when implemented - 12xx12
		LOGWARNING("Loot table: Attributes for items are not implemented, dropping function!");
	}




	void cSetAttributes::operator()(cItem & a_Item) const
	{
		ACTIVECHECK
	}

////////////////////////////////////////////////////////////////////////////////
// cSetContents


	cSetContents::cSetContents(const Json::Value & a_Value)
	{
		// TODO: 02.09.2020 - Add when implemented - 12xx12
		LOGWARNING("Loot table: NBT for items is not yet supported, Dropping function \"SetContents\"!");
	}




	void cSetContents::operator()(cItem & a_Item) const
	{
		ACTIVECHECK
	}

////////////////////////////////////////////////////////////////////////////////
// cSetCount


	cSetCount::cSetCount(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"SetCount\" encountered a Json problem, dropping function!");
			return;
		}
		Json::Value CountObject;
		if (a_Value.isMember("count"))
		{
			CountObject = a_Value["count"];
		}
		else if (a_Value.isMember("Count"))
		{
			CountObject = a_Value["Count"];
		}

		if (CountObject.isInt())
		{
			m_Count = CountObject.asInt();
		}
		else if (CountObject.isObject())
		{
			AString Type;
			if (CountObject.isMember("type"))
			{
				Type = NamespaceConverter(CountObject["type"].asString());
			}
			else if (CountObject.isMember("Type"))
			{
				Type = NamespaceConverter(CountObject["Type"].asString());
			}

			if (Type == "Uniform")
			{
				m_Type = eType::Uniform;
				MinMaxRange<int>(CountObject, m_UniformMin, m_UniformMax);
			}
			else if (Type == "Binomial")
			{
				if (CountObject.isMember("n"))
				{
					m_N = CountObject["n"].asInt();
				}
				else if (CountObject.isMember("N"))
				{
					m_N = CountObject["N"].asInt();
				}

				if (CountObject.isMember("p"))
				{
					m_P = CountObject["p"].asFloat();
				}
				else if (CountObject.isMember("P"))
				{
					m_P = CountObject["P"].asFloat();
				}
			}
			else
			{
				LOGWARNING("Loot table: Function \"SetCount\" got provided a unknown type: %s, dropping function!", Type);
				m_Active = false;
			}
		}
	}





	void cSetCount::operator()(cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const
	{
		ACTIVECHECK
		switch (m_Type)
		{
			case eType::Uniform:
			{
				a_Item.m_ItemCount = (a_Noise.IntNoise3DInt(a_Pos) / 9) % (m_UniformMin - m_UniformMax) + m_UniformMin;
				break;
			}
			case eType::Binomial:
			{
				std::default_random_engine Generator(a_Noise.GetSeed());
				std::binomial_distribution<int> Dist(m_N, m_P);
				std::vector<int> Values;
				for (int i = 0; i < m_N; i++)
				{
					Values[i] = Dist(Generator);
				}
				a_Item.m_ItemCount += Values[a_Noise.IntNoise3DInt(a_Pos * a_Item.m_ItemType) % Values.size()];
				break;
			}
		}
	}

////////////////////////////////////////////////////////////////////////////////
// cSetDamage

	cSetDamage::cSetDamage(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"SetDamage\" encountered a Json problem, dropping function!");
			return;
		}
		Json::Value DamageObject;

		if (a_Value.isMember("damage"))
		{
			DamageObject = a_Value["damage"];
		}
		else if (a_Value.isMember("Damage"))
		{
			DamageObject = a_Value["Damage"];
		}
		else
		{
			LOGWARNING("Loot table: Function \"SetDamage\" is missing it's damage object, dropping function!");
			return;
		}
		MinMaxRange<float>(DamageObject, m_Min, m_Max);
		m_Active = true;
	}




	void cSetDamage::operator()(cItem & a_Item, const cNoise & a_Noise, const Vector3i & a_Pos) const
	{
		ACTIVECHECK
		float Damage = std::fmod((a_Noise.IntNoise3D(a_Pos) / 7), m_Max - m_Min) + m_Min;
		a_Item.m_ItemDamage = floor(a_Item.m_ItemDamage * Damage);
	}

////////////////////////////////////////////////////////////////////////////////
// cSetLootTable

	cSetLootTable::cSetLootTable(const Json::Value & a_Value)
	{
		// TODO: 02.09.2020 - Add when implemented - 12xx12
		LOGWARNING("Loot table: NBT for items is not yet supported, dropping \"SetLootTable\" function!");
		return;
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"SetLootTable\" encountered a Json problem, dropping function!");
			return;
		}
		m_Active = true;
		for (const auto & Key : a_Value.getMemberNames())
		{
			if (NoCaseCompare(Key, "name") == 0)
			{
				m_LootTable = NamespaceConverter(a_Value[Key].asString());
			}
			else if (NoCaseCompare(Key, "seed") == 0)
			{
				m_Seed = a_Value[Key].asInt();
			}
		}
	}




	void cSetLootTable::operator()(cItem & a_Item) const
	{
		ACTIVECHECK
		// TODO: 02.09.2020 - Add when implemented - 12xx12
	}


////////////////////////////////////////////////////////////////////////////////
// cSetLore

	cSetLore::cSetLore(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"SetLore\" encountered a Json problem, dropping function!");
			return;
		}

		for (const auto & Key : a_Value.getMemberNames())
		{
			if (NoCaseCompare(Key, "lore") == 0)
			{
				Json::Value Lore = a_Value[Key];
				if (!Lore.isArray())
				{
					LOGWARNING("Loot table: function \"SetLore\" got a unknown object for it's lore, dropping function!");
				}
				for (unsigned int i = 0; i < Lore.size(); i++)
				{
					if (!Lore[i].isString())
					{
						// TODO: 16.09.2020 - Add when Json objects are used for Lore - 12xx12
						LOGWARNING("Loot table: Items only support plain text in item lore. Please make sure you don't supply a Json object in function \"SetLore\".");
					}
					else
					{
						m_Lore.push_back(Lore[i].asString());
					}
				}
			}
			else if (NoCaseCompare(Key, "entity") == 0)
			{
				AString Entity = NamespaceConverter(a_Value[Key].asString());
				if (Entity == "This")
				{
					m_Type = eType::This;
				}
				else if (Entity == "Killer")
				{
					m_Type = eType::Killer;
				}
				else if (Entity == "KillerPlayer")
				{
					m_Type = eType::KillerPlayer;
				}
				else
				{
					LOGWARNING("Loot table: Function \"SetLore\" got unknown destination: %s, dropping function!", Entity);
				}
			}
			else if (NoCaseCompare(Key, "replace") == 0)
			{
				m_Replace = a_Value[Key].asBool();
			}
		}
	}




	void cSetLore::operator()(cItem & a_Item, UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		// TODO: 19.09.2020 - Add Json text component - 12xx12
		if (m_Replace)
		{
			a_Item.m_LoreTable = std::move(m_Lore);
		}
		else
		{
			a_Item.m_LoreTable.insert(a_Item.m_LoreTable.end(), m_Lore.begin(), m_Lore.end());
		}
	}


////////////////////////////////////////////////////////////////////////////////
// cSetName


	cSetName::cSetName(const Json::Value & a_Value)
	{
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"SetName\" encountered a Json problem, dropping function!");
			return;
		}
		for (const auto & Key : a_Value.getMemberNames())
		{
			if (NoCaseCompare(Key, "name") == 0)
			{
				if (!a_Value[Key].isString())
				{
					// TODO: 16.09.2020 - Add when Json objects are used for Names - 12xx12
					LOGWARNING("Loot table: Items only support plain text in item name. Please make sure you don't supply a Json object in function \"SetName\".");
				}
				else
				{
					m_Name = a_Value[Key].asString();
				}
			}
			else if (NoCaseCompare(Key, "entity") == 0)
			{
				AString Entity = NamespaceConverter(a_Value[Key].asString());
				if (Entity == "This")
				{
					m_Type = eType::This;
				}
				else if (Entity == "Killer")
				{
					m_Type = eType::Killer;
				}
				else if (Entity == "KillerPlayer")
				{
					m_Type = eType::KillerPlayer;
				}
				else
				{
					LOGWARNING("Loot table: Function \"SetName\" got unknown destination: %s, dropping function!", Entity);
				}
			}
		}

	}




	void cSetName::operator()(cItem & a_Item, UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		// Todo: 19.09.20 - Add Json text component - 12xx12
		a_Item.m_CustomName = m_Name;
	}

////////////////////////////////////////////////////////////////////////////////
// cSetName


	cSetNbt::cSetNbt(const Json::Value & a_Value)
	{
		// TODO: 02.09.2020 - Add when implemented - 12xx12
		LOGWARNING("Loot table: NBT for items is not yet supported, dropping function \"SetNBT\"!");
		return;
		if ((a_Value.empty()) || (a_Value.isArray()))
		{
			LOGWARNING("Loot table: Function \"SetNbt\" encountered a Json problem, dropping function!");
			return;
		}

		if (a_Value.isMember("tag"))
		{
			m_Tag = a_Value["tag"].asString();
		}
		else if (a_Value.isMember("Tag"))
		{
			m_Tag = a_Value["Tag"].asString();
		}
		else
		{
			LOGWARNING("Loot table: Function \"SetNbt\" didn't get any tag, dropping function!");
			return;
		}
		m_Active = true;
	}




	void cSetNbt::operator()(cItem & a_Item) const
	{
		ACTIVECHECK
		// a_Item.SetNBT(m_Tag);
	}

////////////////////////////////////////////////////////////////////////////////
// cSetStewEffect

	cSetStewEffect::cSetStewEffect(const Json::Value & a_Value)
	{
		// TODO: 02.09.2020 - Add when implemented - 12xx12
		LOGWARNING("Loot table: Stews are not yet supported, dropping function \"SetStewEffect\"!");
	}





	void cSetStewEffect::operator()(cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID) const
	{
		ACTIVECHECK
	}


}  // Namespace Function


	cLootTablePool ParsePool(const Json::Value & a_Value)
	{
		cLootTablePoolRolls PoolRolls;
		cLootTablePoolRolls PoolBonusRolls;
		cLootTablePoolEntries PoolEntries;
		cLootTableFunctions Functions;
		cLootTableConditions Conditions;
		for (auto & PoolElement : a_Value.getMemberNames())
		{
			if (NoCaseCompare(PoolElement, "rolls") == 0)
			{
				auto Rolls = a_Value[PoolElement];
				PoolRolls = ParseRolls(Rolls);
			}
			else if (
				(NoCaseCompare(PoolElement, "bonus_rolls") == 0) ||
				(NoCaseCompare(PoolElement, "BonusRolls") == 0))
			{
				auto Rolls = a_Value[PoolElement];
				PoolBonusRolls = ParseRolls(Rolls);
			}
			else if (NoCaseCompare(PoolElement, "entries") == 0)
			{
				auto Entries = a_Value[PoolElement];
				for (unsigned int EntryIndex = 0; EntryIndex < Entries.size(); EntryIndex++)
				{
					PoolEntries.push_back(ParsePoolEntry(Entries[EntryIndex]));
				}
			}
			else if (NoCaseCompare(PoolElement, "functions") == 0)
			{
				auto FunctionsObject = a_Value[PoolElement];
				for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
				{
					Functions.push_back(ParseFunction(FunctionsObject[FunctionIndex]));
				}
			}
			else if (NoCaseCompare(PoolElement, "conditions") == 0)
			{
				auto ConditionsObject = a_Value[PoolElement];
				for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
				{
					Conditions.emplace_back(ParseCondition(ConditionsObject[ConditionId]));
				}
			}
		}
		return cLootTablePool(PoolRolls, PoolEntries, Conditions);
	}





	cLootTableFunction ParseFunction(const Json::Value & a_Value)
	{
		cLootTableConditions Conditions;

		Json::Value ConditionsObject;

		if (a_Value.isMember("condition"))
		{
			ConditionsObject = a_Value["condition"];
		}
		else if (a_Value.isMember("Condition"))
		{
			ConditionsObject = a_Value["Conditions"];
		}

		if (ConditionsObject.isObject())
		{
			for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
			{
				Conditions.emplace_back(ParseCondition(ConditionsObject[ConditionId]));
			}
		}

		AString Function;

		if (a_Value.isMember("function"))
		{
			Function = NamespaceConverter(a_Value["function"].asString());
		}
		else if (a_Value.isMember("Function"))
		{
			Function = NamespaceConverter(a_Value["Function"].asString());
		}

		if (NoCaseCompare(Function, "ApplyBonus") == 0)
		{
			return cLootTableFunction(LootTable::Function::cApplyBonus(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "CopyName") == 0)
		{
			return cLootTableFunction(LootTable::Function::cCopyName(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "CopyNbt") == 0)
		{
			return cLootTableFunction(LootTable::Function::cCopyNbt(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "CopyState") == 0)
		{
			return cLootTableFunction(LootTable::Function::cCopyState(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "EnchantRandomly") == 0)
		{
			return cLootTableFunction(LootTable::Function::cEnchantRandomly(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "EnchantWithLevels") == 0)
		{
			return cLootTableFunction(LootTable::Function::cEnchantWithLevels(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "ExplorationMap") == 0)
		{
			return cLootTableFunction(LootTable::Function::cExplorationMap(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "ExplosionDecay") == 0)
		{
			return cLootTableFunction(LootTable::Function::cExplosionDecay(), Conditions);
		}
		else if (NoCaseCompare(Function, "FurnaceSmelt") == 0)
		{
			return cLootTableFunction(LootTable::Function::cFurnaceSmelt(), Conditions);
		}
		else if (NoCaseCompare(Function, "FillPlayerHead") == 0)
		{
			return cLootTableFunction(LootTable::Function::cFillPlayerHead(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "LimitCount") == 0)
		{
			return cLootTableFunction(LootTable::Function::cLimitCount(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "LootingEnchant") == 0)
		{
			return cLootTableFunction(LootTable::Function::cLootingEnchant(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "SetAttributes") == 0)
		{
			return cLootTableFunction(LootTable::Function::cSetAttributes(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "SetContents") == 0)
		{
			return cLootTableFunction(LootTable::Function::cSetContents(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "SetCount") == 0)
		{
			return cLootTableFunction(LootTable::Function::cSetCount(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "SetDamage") == 0)
		{
			return cLootTableFunction(LootTable::Function::cSetDamage(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "SetLootTable") == 0)
		{
			return cLootTableFunction(LootTable::Function::cSetLootTable(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "SetLore") == 0)
		{
			return cLootTableFunction(LootTable::Function::cSetLore(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "SetName") == 0)
		{
			return cLootTableFunction(LootTable::Function::cSetName(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "SetNbt") == 0)
		{
			return cLootTableFunction(LootTable::Function::cSetNbt(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "SetStewEffect") == 0)
		{
			return cLootTableFunction(LootTable::Function::cSetStewEffect(a_Value), Conditions);
		}
		else if (NoCaseCompare(Function, "None") == 0)
		{
			return cLootTableFunction(LootTable::Function::cNone(), Conditions);
		}


		return cLootTableFunction(LootTable::Function::cNone(), Conditions);
	}





	cLootTableCondition ParseCondition(const Json::Value & a_Value)
	{
		AString Type;

		// Type has to be known beforehand
		if (a_Value.isMember("condition"))
		{
			Type = NamespaceConverter(a_Value["condition"].asString());
		}
		else if (a_Value.isMember("Condition"))
		{
			Type = NamespaceConverter(a_Value["Condition"].asString());
		}
		else
		{
			LOGWARNING("Loot table: Condition is missing type. Dropping condition!");
			return cLootTableCondition(Condition::cNone());
		}

		if (NoCaseCompare(Type, "Alternative") == 0)
		{
			return cLootTableCondition(Condition::cAlternative(a_Value));
		}
		else if (NoCaseCompare(Type, "BlockStateProperty") == 0)
		{
			return cLootTableCondition(Condition::cBlockStateProperty(a_Value));
		}
		else if (NoCaseCompare(Type, "DamageSourceProperties") == 0)
		{
			return cLootTableCondition(Condition::cDamageSourceProperties(a_Value));
		}
		else if (NoCaseCompare(Type, "EntityProperties") == 0)
		{
			return cLootTableCondition(Condition::cEntityProperties(a_Value));
		}
		else if (NoCaseCompare(Type, "EntityScores") == 0)
		{
			return cLootTableCondition(Condition::cEntityScores(a_Value));
		}
		else if (NoCaseCompare(Type, "Inverted") == 0)
		{
			return cLootTableCondition(Condition::cInverted(a_Value));
		}
		else if (NoCaseCompare(Type, "KilledByPlayer") == 0)
		{
			return cLootTableCondition(Condition::cKilledByPlayer(a_Value));
		}
		else if (NoCaseCompare(Type, "LocationCheck") == 0)
		{
			return cLootTableCondition(Condition::cLocationCheck(a_Value));
		}
		else if (NoCaseCompare(Type, "MatchTool") == 0)
		{
			return cLootTableCondition(Condition::cMatchTool(a_Value));
		}
		else if (NoCaseCompare(Type, "RandomChance") == 0)
		{
			return cLootTableCondition(Condition::cRandomChance(a_Value));
		}
		else if (NoCaseCompare(Type, "RandomChanceWithLooting") == 0)
		{
			return cLootTableCondition(Condition::cRandomChanceWithLooting(a_Value));
		}
		else if (NoCaseCompare(Type, "Reference") == 0)
		{
			return cLootTableCondition(Condition::cReference(a_Value));
		}
		else if (NoCaseCompare(Type, "SurvivesExplosion") == 0)
		{
			return cLootTableCondition(Condition::cSurvivesExplosion());
		}
		else if (NoCaseCompare(Type, "TableBonus") == 0)
		{
			return cLootTableCondition(Condition::cTableBonus(a_Value));
		}
		else if (NoCaseCompare(Type, "TimeCheck") == 0)
		{
			return cLootTableCondition(Condition::cTimeCheck(a_Value));
		}
		else if (NoCaseCompare(Type, "WeatherCheck") == 0)
		{
			return cLootTableCondition(Condition::cWeatherCheck(a_Value));
		}
		else
		{
			LOGWARNING("Loot table: Unknown loot table condition provided: %s. Using no condition", Type);
			return cLootTableCondition(Condition::cNone());
		}
	}





	cLootTablePoolEntry ParsePoolEntry(const Json::Value & a_Value)
	{
		cLootTableConditions Conditions;
		cLootTableFunctions Functions;
		enum ePoolEntryType Type;

		cItem Item;
		AString Name;
		cLootTablePoolEntries Children;

		bool Expand = true;
		int Weight = 1;
		int Quality = 0;

		// The type has to be known beforehand
		if (a_Value.isMember("type"))
		{
			Type = ePoolEntryType(NamespaceConverter(a_Value["type"].asString()));
		}
		else if (a_Value.isMember("Type"))
		{
			Type = ePoolEntryType(NamespaceConverter(a_Value["Type"].asString()));
		}
		else
		{
			LOGWARNING("Loot table: No entry type provided - dropping entry");
			return cLootTablePoolEntry();
		}

		for (auto & EntryParameter : a_Value.getMemberNames())
		{
			if (NoCaseCompare(EntryParameter, "weight") == 0)
			{
				Weight = a_Value[EntryParameter].asInt();
			}
			else if (NoCaseCompare(EntryParameter, "name") == 0)
			{
				switch (Type)
				{
					case ePoolEntryType::Item:
					{
						StringToItem(NamespaceConverter(a_Value[EntryParameter].asString()), Item);
						break;
					}
					case ePoolEntryType::Tag:
					case ePoolEntryType::LootTable:
					case ePoolEntryType::Dynamic:
					{
						Name = NamespaceConverter(a_Value[EntryParameter].asString());
						break;
					}
					default: break;
				}
			}
			else if (NoCaseCompare(EntryParameter, "functions") == 0)
			{
				auto FunctionsObject = a_Value[EntryParameter];
				for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
				{
					Functions.push_back(ParseFunction(FunctionsObject[FunctionIndex]));
				}
			}
			else if (NoCaseCompare(EntryParameter, "conditions") == 0)
			{
				auto ConditionsObject = a_Value[EntryParameter];
				for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
				{
					Conditions.push_back(ParseCondition(ConditionsObject[ConditionId]));
				}
			}
			else if (NoCaseCompare(EntryParameter, "children") == 0)
			{
				switch (Type)
				{
					case ePoolEntryType::Group:
					case ePoolEntryType::Alternatives:
					case ePoolEntryType::Sequence:
					{
						auto ChildrenObject = a_Value[EntryParameter];
						for (unsigned int ChildrenObjectId = 0; ChildrenObjectId < ChildrenObject.size(); ++ChildrenObjectId)
						{
							Children.push_back(ParsePoolEntry(ChildrenObject[ChildrenObjectId]));
						}
						break;
					}
					default: break;
				}
			}
			else if (NoCaseCompare(EntryParameter, "expand") == 0)
			{
				Expand = a_Value[EntryParameter].asBool();
			}
			else if (NoCaseCompare(EntryParameter, "quality") == 0)
			{
				Quality = a_Value[EntryParameter].asInt();
			}
		}
		switch (Type)
		{
			case ePoolEntryType::Item:
			{
				return cLootTablePoolEntry(Conditions, Functions, Type, Item, Weight, Quality);
			}

			case ePoolEntryType::Tag:
			case ePoolEntryType::LootTable:
			case ePoolEntryType::Dynamic:
			{
				return cLootTablePoolEntry(Conditions, Functions, Type, Name, Expand, Weight, Quality);
			}

			case ePoolEntryType::Group:
			case ePoolEntryType::Alternatives:
			case ePoolEntryType::Sequence:
			{
				return cLootTablePoolEntry(Conditions, Functions, Type, Children, Weight, Quality);
			}

			case ePoolEntryType::Empty: return cLootTablePoolEntry();
			default:                               return cLootTablePoolEntry();
		}
	}





	cLootTablePoolRolls ParseRolls(const Json::Value & a_Value)
	{
		if (a_Value.isInt())
		{
			return cLootTablePoolRolls(a_Value.asInt(), a_Value.asInt());
		}
		else
		{
			int Min = 0, Max = -1;
			for (auto & RollEntry: a_Value.getMemberNames())
			{
				if (NoCaseCompare(RollEntry, "min") == 0)
				{
					Min = a_Value[RollEntry].asInt();
				}
				else if (NoCaseCompare(RollEntry, "max") == 0)
				{
					Max = a_Value[RollEntry].asInt();
				}
			}
			if (Max == -1)
			{
				LOGWARNING("Loot table: Missing maximum value in pool rolls - assuming steady roll");
				Max = Min;
			}
			return cLootTablePoolRolls(Min, Max);
		}
	}
}




