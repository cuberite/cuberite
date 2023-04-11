
#include "WSSAnvilHandler_1_8.h"

#include "WorldStorage/EnchantmentSerializer.h"
#include "WorldStorage/FireworksSerializer.h"


bool cWSSAnvilHandler_1_8::LoadItem(cItem & a_Item, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int Type = a_NBT.FindChildByName(a_TagIdx, "id");
	if (Type <= 0)
	{
		return false;
	}

	if (a_NBT.GetType(Type) == TAG_String)
	{
		if (!StringToItem(a_NBT.GetString(Type), a_Item))
		{
			// Can't resolve item type
			return false;
		}
	}
	else if (a_NBT.GetType(Type) == TAG_Short)
	{
		a_Item.m_ItemType = a_NBT.GetShort(Type);
	}
	else
	{
		return false;
	}

	if (a_Item.m_ItemType < 0)
	{
		a_Item.Empty();
		return true;
	}

	int Damage = a_NBT.FindChildByName(a_TagIdx, "Damage");
	if ((Damage > 0) && (a_NBT.GetType(Damage) == TAG_Short))
	{
		a_Item.m_ItemDamage = a_NBT.GetShort(Damage);
	}

	int Count = a_NBT.FindChildByName(a_TagIdx, "Count");
	if ((Count > 0) && (a_NBT.GetType(Count) == TAG_Byte))
	{
		a_Item.m_ItemCount = static_cast<char>(a_NBT.GetByte(Count));
	}

	// Find the "tag" tag, used for enchantments and other extra data
	int TagTag = a_NBT.FindChildByName(a_TagIdx, "tag");
	if (TagTag <= 0)
	{
		// No extra data
		return true;
	}

	// Load repair cost:
	int RepairCost = a_NBT.FindChildByName(TagTag, "RepairCost");
	if ((RepairCost > 0) && (a_NBT.GetType(RepairCost) == TAG_Int))
	{
		a_Item.m_RepairCost = a_NBT.GetInt(RepairCost);
	}

	// Load display name:
	int DisplayTag = a_NBT.FindChildByName(TagTag, "display");
	if (DisplayTag > 0)
	{
		int DisplayName = a_NBT.FindChildByName(DisplayTag, "Name");
		if ((DisplayName > 0) && (a_NBT.GetType(DisplayName) == TAG_String))
		{
			a_Item.m_CustomName = a_NBT.GetString(DisplayName);
		}
		int Lore = a_NBT.FindChildByName(DisplayTag, "Lore");
		if ((Lore > 0) && (a_NBT.GetType(Lore) == TAG_String))
		{
			// Legacy string lore
			a_Item.m_LoreTable = StringSplit(a_NBT.GetString(Lore), "`");
		}
		else if ((Lore > 0) && (a_NBT.GetType(Lore) == TAG_List))
		{
			// Lore table
			a_Item.m_LoreTable.clear();
			for (int loretag = a_NBT.GetFirstChild(Lore); loretag >= 0; loretag = a_NBT.GetNextSibling(loretag))  // Loop through array of strings
			{
				a_Item.m_LoreTable.push_back(a_NBT.GetString(loretag));
			}
		}
	}

	// Load enchantments:
	const char * EnchName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
	int EnchTag = a_NBT.FindChildByName(TagTag, EnchName);
	if (EnchTag > 0)
	{
		EnchantmentSerializer::ParseFromNBT(a_Item.m_Enchantments, a_NBT, EnchTag);
	}

	// Load firework data:
	int FireworksTag = a_NBT.FindChildByName(TagTag, ((a_Item.m_ItemType == E_ITEM_FIREWORK_STAR) ? "Explosion" : "Fireworks"));
	if (FireworksTag > 0)
	{
		cFireworkItem::ParseFromNBT(a_Item.m_FireworkItem, a_NBT, FireworksTag, static_cast<ENUM_ITEM_TYPE>(a_Item.m_ItemType));
	}

	return true;
}





void cWSSAnvilHandler_1_8::LoadItemGrid(cItemGrid & a_ItemGrid, const cParsedNBT & a_NBT, int a_ItemsTagIdx, int a_SlotOffset) const
{
	int NumSlots = a_ItemGrid.GetNumSlots();
	for (int Child = a_NBT.GetFirstChild(a_ItemsTagIdx); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		int SlotTag = a_NBT.FindChildByName(Child, "Slot");
		if ((SlotTag < 0) || (a_NBT.GetType(SlotTag) != TAG_Byte))
		{
			continue;
		}
		int SlotNum = static_cast<int>(a_NBT.GetByte(SlotTag)) - a_SlotOffset;
		if ((SlotNum < 0) || (SlotNum >= NumSlots))
		{
			// SlotNum outside of the range
			continue;
		}
		cItem Item;
		if (LoadItem(Item, a_NBT, Child))
		{
			a_ItemGrid.SetSlot(SlotNum, Item);
		}
	}  // for itr - ItemDefs[]
}





void cWSSAnvilHandler_1_8::LoadEntities(cEntityList & a_Entities, const cParsedNBT & a_NBT, const int a_TagIdx) const
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_List))
	{
		return;
	}

	for (int Child = a_NBT.GetFirstChild(a_TagIdx); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		if (a_NBT.GetType(Child) != TAG_Compound)
		{
			continue;
		}
		int sID = a_NBT.FindChildByName(Child, "id");
		if (sID < 0)
		{
			continue;
		}

		try
		{
			LoadEntity(a_Entities, a_NBT, Child, a_NBT.GetStringView(sID));
		}
		catch (...)
		{
			continue;
		}
	}  // for Child - a_NBT[]
}





void cWSSAnvilHandler_1_8::LoadEntity(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_EntityTagIdx, const std::string_view a_EntityName) const
{
	const auto NamespaceSplit = NamespaceSerializer::SplitNamespacedID(a_EntityName);
	if (NamespaceSplit.first == NamespaceSerializer::Namespace::Unknown)
	{
		return;
	}

	switch (NamespaceSerializer::ToEntityType(NamespaceSplit.second))
	{
		case cEntity::etBoat:            return LoadBoat(a_Entities, a_NBT, a_EntityTagIdx);
		case cEntity::etEnderCrystal:    return LoadEndCrystal(a_Entities, a_NBT, a_EntityTagIdx);
		case cEntity::etFallingBlock:    return LoadFallingBlock(a_Entities, a_NBT, a_EntityTagIdx);

		case cEntity::etPickup:          return LoadPickup(a_Entities, a_NBT, a_EntityTagIdx);
		case cEntity::etPainting:        return LoadPainting(a_Entities, a_NBT, a_EntityTagIdx);
		case cEntity::etTNT:             return LoadTNT(a_Entities, a_NBT, a_EntityTagIdx);
		case cEntity::etExpOrb:          return LoadExpOrb(a_Entities, a_NBT, a_EntityTagIdx);
		case cEntity::etItemFrame:       return LoadItemFrame(a_Entities, a_NBT, a_EntityTagIdx);
		case cEntity::etLeashKnot:       return LoadLeashKnot(a_Entities, a_NBT, a_EntityTagIdx);
		default: break;
	}

	switch (NamespaceSerializer::ToProjectileType(NamespaceSplit.second))
	{
		case cProjectileEntity::pkArrow:           return LoadArrow(a_Entities, a_NBT, a_EntityTagIdx);
		case cProjectileEntity::pkSplashPotion:    return LoadSplashPotion(a_Entities, a_NBT, a_EntityTagIdx);
		case cProjectileEntity::pkSnowball:        return LoadSnowball(a_Entities, a_NBT, a_EntityTagIdx);
		case cProjectileEntity::pkEgg:             return LoadEgg(a_Entities, a_NBT, a_EntityTagIdx);
		case cProjectileEntity::pkGhastFireball:   return LoadFireball(a_Entities, a_NBT, a_EntityTagIdx);
		case cProjectileEntity::pkFireCharge:      return LoadFireCharge(a_Entities, a_NBT, a_EntityTagIdx);
		case cProjectileEntity::pkEnderPearl:      return LoadThrownEnderPearl(a_Entities, a_NBT, a_EntityTagIdx);
		case cProjectileEntity::pkExpBottle:       break;
		case cProjectileEntity::pkFirework:        break;
		case cProjectileEntity::pkWitherSkull:     break;
	}

	switch (NamespaceSerializer::ToMinecartType(NamespaceSplit.second))
	{
		case cMinecart::mpChest:   return LoadChestMinecart(a_Entities, a_NBT, a_EntityTagIdx);
		case cMinecart::mpFurnace: return LoadFurnaceMinecart(a_Entities, a_NBT, a_EntityTagIdx);
		case cMinecart::mpTNT:     return LoadTNTMinecart(a_Entities, a_NBT, a_EntityTagIdx);
		case cMinecart::mpHopper:  return LoadHopperMinecart(a_Entities, a_NBT, a_EntityTagIdx);
		case cMinecart::mpNone:    return LoadRideableMinecart(a_Entities, a_NBT, a_EntityTagIdx);
	}

	switch (NamespaceSerializer::ToMonsterType(NamespaceSplit.second))
	{
		case mtBat:             return LoadBat(a_Entities, a_NBT, a_EntityTagIdx);
		case mtBlaze:           return LoadBlaze(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCat:             return LoadCat(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCaveSpider:      return LoadCaveSpider(a_Entities, a_NBT, a_EntityTagIdx);
		case mtChicken:         return LoadChicken(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCod:             return LoadCod(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCow:             return LoadCow(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCreeper:         return LoadCreeper(a_Entities, a_NBT, a_EntityTagIdx);
		case mtDolphin:         return LoadDolphin(a_Entities, a_NBT, a_EntityTagIdx);
		case mtDonkey:          return LoadDonkey(a_Entities, a_NBT, a_EntityTagIdx);
		case mtDrowned:         return LoadDrowned(a_Entities, a_NBT, a_EntityTagIdx);
		case mtElderGuardian:   return LoadElderGuardian(a_Entities, a_NBT, a_EntityTagIdx);
		case mtEnderDragon:     return LoadEnderDragon(a_Entities, a_NBT, a_EntityTagIdx);
		case mtEnderman:        return LoadEnderman(a_Entities, a_NBT, a_EntityTagIdx);
		case mtEndermite:       return LoadEndermite(a_Entities, a_NBT, a_EntityTagIdx);
		case mtEvoker:          return LoadEvoker(a_Entities, a_NBT, a_EntityTagIdx);
		case mtFox:             return LoadFox(a_Entities, a_NBT, a_EntityTagIdx);
		case mtGhast:           return LoadGhast(a_Entities, a_NBT, a_EntityTagIdx);
		case mtGiant:           return LoadGiant(a_Entities, a_NBT, a_EntityTagIdx);
		case mtGuardian:        return LoadGuardian(a_Entities, a_NBT, a_EntityTagIdx);
		case mtHorse:           return LoadHorse(a_Entities, a_NBT, a_EntityTagIdx);
		case mtHoglin:          return LoadHoglin(a_Entities, a_NBT, a_EntityTagIdx);
		case mtHusk:            return LoadHusk(a_Entities, a_NBT, a_EntityTagIdx);
		case mtIllusioner:      return LoadIllusioner(a_Entities, a_NBT, a_EntityTagIdx);
		case mtIronGolem:       return LoadIronGolem(a_Entities, a_NBT, a_EntityTagIdx);
		case mtLlama:           return LoadLlama(a_Entities, a_NBT, a_EntityTagIdx);
		case mtMagmaCube:       return LoadMagmaCube(a_Entities, a_NBT, a_EntityTagIdx);
		case mtMooshroom:       return LoadMooshroom(a_Entities, a_NBT, a_EntityTagIdx);
		case mtMule:            return LoadMule(a_Entities, a_NBT, a_EntityTagIdx);
		case mtOcelot:          return LoadOcelot(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPanda:           return LoadPanda(a_Entities, a_NBT, a_EntityTagIdx);
		case mtParrot:          return LoadParrot(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPhantom:         return LoadPhantom(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPig:             return LoadPig(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPiglin:          return LoadPiglin(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPiglinBrute:     return LoadPiglinBrute(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPillager:        return LoadPillager(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPolarBear:       return LoadPolarBear(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPufferfish:      return LoadPufferfish(a_Entities, a_NBT, a_EntityTagIdx);
		case mtRabbit:          return LoadRabbit(a_Entities, a_NBT, a_EntityTagIdx);
		case mtRavager:         return LoadRavager(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSalmon:          return LoadSalmon(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSheep:           return LoadSheep(a_Entities, a_NBT, a_EntityTagIdx);
		case mtShulker:         return LoadShulker(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSilverfish:      return LoadSilverfish(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSkeleton:        return LoadSkeleton(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSkeletonHorse:   return LoadSkeletonHorse(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSlime:           return LoadSlime(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSnowGolem:       return LoadSnowGolem(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSpider:          return LoadSpider(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSquid:           return LoadSquid(a_Entities, a_NBT, a_EntityTagIdx);
		case mtStray:           return LoadStray(a_Entities, a_NBT, a_EntityTagIdx);
		case mtStrider:         return LoadStrider(a_Entities, a_NBT, a_EntityTagIdx);
		case mtTraderLlama:     return LoadTraderLlama(a_Entities, a_NBT, a_EntityTagIdx);
		case mtTropicalFish:    return LoadTropicalFish(a_Entities, a_NBT, a_EntityTagIdx);
		case mtTurtle:          return LoadTurtle(a_Entities, a_NBT, a_EntityTagIdx);
		case mtVex:             return LoadVex(a_Entities, a_NBT, a_EntityTagIdx);
		case mtVillager:        return LoadVillager(a_Entities, a_NBT, a_EntityTagIdx);
		case mtVindicator:      return LoadVindicator(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWanderingTrader: return LoadWanderingTrader(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWitch:           return LoadWitch(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWither:          return LoadWither(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWitherSkeleton:  return LoadWitherSkeleton(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWolf:            return LoadWolf(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZoglin:          return LoadZoglin(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZombie:          return LoadZombie(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZombieHorse:     return LoadZombieHorse(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZombifiedPiglin: return LoadZombifiedPiglin(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZombieVillager:  return LoadZombieVillager(a_Entities, a_NBT, a_EntityTagIdx);
		case mtInvalidType:     break;
	}
}





bool cWSSAnvilHandler_1_8::LoadEntityBase(cEntity & a_Entity, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::array<double, 3> Pos;
	if (!LoadDoublesList<3>(Pos, a_NBT, a_NBT.FindChildByName(a_TagIdx, "Pos")))
	{
		return false;
	}
	a_Entity.SetPosition(Pos[0], Pos[1], Pos[2]);

	std::array<double, 3> Speed;
	if (!LoadDoublesList<3>(Speed, a_NBT, a_NBT.FindChildByName(a_TagIdx, "Motion")))
	{
		// Provide default speed:
		Speed[0] = 0;
		Speed[1] = 0;
		Speed[2] = 0;
	}
	a_Entity.SetSpeed(Speed[0], Speed[1], Speed[2]);

	std::array<double, 2> Rotation;
	if (!LoadDoublesList<2>(Rotation, a_NBT, a_NBT.FindChildByName(a_TagIdx, "Rotation")))
	{
		// Provide default rotation:
		Rotation[0] = 0;
		Rotation[1] = 0;
	}
	a_Entity.SetYaw(Rotation[0]);
	a_Entity.SetRoll(Rotation[1]);

	// Depending on the Minecraft version, the entity's health is
	// stored either as a float Health tag (HealF prior to 1.9) or
	// as a short Health tag. The float tags should be preferred.
	int Health = a_NBT.FindChildByName(a_TagIdx, "Health");
	int HealF  = a_NBT.FindChildByName(a_TagIdx, "HealF");

	if (Health > 0 && a_NBT.GetType(Health) == TAG_Float)
	{
		a_Entity.SetHealth(a_NBT.GetFloat(Health));
	}
	else if (HealF > 0)
	{
		a_Entity.SetHealth(a_NBT.GetFloat(HealF));
	}
	else if (Health > 0)
	{
		a_Entity.SetHealth(static_cast<float>(a_NBT.GetShort(Health)));
	}
	else
	{
		a_Entity.SetHealth(a_Entity.GetMaxHealth());
	}

	return true;
}





void cWSSAnvilHandler_1_8::LoadBoat(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Boat = std::make_unique<cBoat>(Vector3d(), cBoat::bmOak);
	if (!LoadEntityBase(*Boat.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.emplace_back(std::move(Boat));
}





void cWSSAnvilHandler_1_8::LoadEndCrystal(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	bool ShowBottom = false;
	auto CurrentLine = a_NBT.FindChildByName(a_TagIdx, "ShowBottom");
	if (CurrentLine > 0)
	{
		ShowBottom = a_NBT.GetByte(CurrentLine) == 1;
	}

	// Beam wasn't stored in 1.8
	auto EnderCrystal = std::make_unique<cEnderCrystal>(Vector3d(), Vector3i(), false, ShowBottom);
	if (!LoadEntityBase(*EnderCrystal.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(EnderCrystal));
}





void cWSSAnvilHandler_1_8::LoadExpOrb(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto ExpOrb = std::make_unique<cExpOrb>(Vector3d(), 0);
	if (!LoadEntityBase(*ExpOrb.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Load Age:
	int Age = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (Age > 0)
	{
		ExpOrb->SetAge(a_NBT.GetShort(Age));
	}

	// Load Reward (Value):
	int Reward = a_NBT.FindChildByName(a_TagIdx, "Value");
	if (Reward > 0)
	{
		ExpOrb->SetReward(a_NBT.GetShort(Reward));
	}

	a_Entities.emplace_back(std::move(ExpOrb));
}





void cWSSAnvilHandler_1_8::LoadFallingBlock(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "TileID");
	int MetaIdx = a_NBT.FindChildByName(a_TagIdx, "Data");

	if ((TypeIdx < 0) || (MetaIdx < 0))
	{
		return;
	}

	auto Type = static_cast<BLOCKTYPE>(a_NBT.GetInt(TypeIdx));
	auto Meta = static_cast<NIBBLETYPE>(a_NBT.GetByte(MetaIdx));

	auto FallingBlock = std::make_unique<cFallingBlock>(Vector3i(0, 0, 0), Type, Meta);
	if (!LoadEntityBase(*FallingBlock.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.emplace_back(std::move(FallingBlock));
}





void cWSSAnvilHandler_1_8::LoadHanging(cHangingEntity & a_Hanging, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	// "Facing" tag is the prime source of the Facing; if not available, translate from older "Direction" or "Dir"
	int Facing = a_NBT.FindChildByName(a_TagIdx, "Facing");
	if (Facing < 0)
	{
		return;
	}

	// Todo: Move this to the storage class
	a_Hanging.SetProtocolFacing(a_NBT.GetByte(Facing));

	int TileX = a_NBT.FindChildByName(a_TagIdx, "TileX");
	int TileY = a_NBT.FindChildByName(a_TagIdx, "TileY");
	int TileZ = a_NBT.FindChildByName(a_TagIdx, "TileZ");
	if ((TileX > 0) && (TileY > 0) && (TileZ > 0))
	{
		a_Hanging.SetPosition(
			static_cast<double>(a_NBT.GetInt(TileX)),
			static_cast<double>(a_NBT.GetInt(TileY)),
			static_cast<double>(a_NBT.GetInt(TileZ))
		);
	}
}





void cWSSAnvilHandler_1_8::LoadItemFrame(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	// Load item:
	int ItemTag = a_NBT.FindChildByName(a_TagIdx, "Item");
	if ((ItemTag < 0) || (a_NBT.GetType(ItemTag) != TAG_Compound))
	{
		return;
	}
	cItem Item;
	if (!LoadItem(Item, a_NBT, ItemTag))
	{
		return;
	}

	auto ItemFrame = std::make_unique<cItemFrame>(BLOCK_FACE_NONE, Vector3d());
	if (!LoadEntityBase(*ItemFrame.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	ItemFrame->SetItem(Item);

	LoadHanging(*ItemFrame.get(), a_NBT, a_TagIdx);

	// Load Rotation:
	int Rotation = a_NBT.FindChildByName(a_TagIdx, "ItemRotation");
	if (Rotation > 0)
	{
		ItemFrame->SetItemRotation(static_cast<Byte>(a_NBT.GetByte(Rotation)));
	}

	a_Entities.emplace_back(std::move(ItemFrame));
}





void cWSSAnvilHandler_1_8::LoadLeashKnot(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto LeashKnot = std::make_unique<cLeashKnot>(BLOCK_FACE_NONE, Vector3d());

	if (!LoadEntityBase(*LeashKnot.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	LoadHanging(*LeashKnot.get(), a_NBT, a_TagIdx);

	a_Entities.emplace_back(std::move(LeashKnot));
}





void cWSSAnvilHandler_1_8::LoadPainting(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	// Load painting name:
	int MotiveTag = a_NBT.FindChildByName(a_TagIdx, "Motive");
	if ((MotiveTag < 0) || (a_NBT.GetType(MotiveTag) != TAG_String))
	{
		return;
	}

	auto Painting = std::make_unique<cPainting>(a_NBT.GetString(MotiveTag), BLOCK_FACE_NONE, Vector3d());
	if (!LoadEntityBase(*Painting.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	LoadHanging(*Painting.get(), a_NBT, a_TagIdx);
	a_Entities.emplace_back(std::move(Painting));
}





void cWSSAnvilHandler_1_8::LoadPickup(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	// Load item:
	int ItemTag = a_NBT.FindChildByName(a_TagIdx, "Item");
	if ((ItemTag < 0) || (a_NBT.GetType(ItemTag) != TAG_Compound))
	{
		return;
	}
	cItem Item;
	if (!LoadItem(Item, a_NBT, ItemTag))
	{
		return;
	}

	auto Pickup = std::make_unique<cPickup>(Vector3d(), Item, false);  // Pickup delay doesn't matter, just say false
	if (!LoadEntityBase(*Pickup.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Load age:
	int Age = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (Age > 0)
	{
		Pickup->SetAge(a_NBT.GetShort(Age));
	}

	a_Entities.emplace_back(std::move(Pickup));
}





void cWSSAnvilHandler_1_8::LoadTNT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto TNT = std::make_unique<cTNTEntity>(Vector3d(), 0);
	if (!LoadEntityBase(*TNT.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Load Fuse Ticks:
	int FuseTicks = a_NBT.FindChildByName(a_TagIdx, "Fuse");
	if (FuseTicks > 0)
	{
		TNT->SetFuseTicks(a_NBT.GetByte(FuseTicks));
	}

	a_Entities.emplace_back(std::move(TNT));
}





void cWSSAnvilHandler_1_8::LoadOldMinecart(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	// It is a minecart, old style, find out the type:
	int TypeTag = a_NBT.FindChildByName(a_TagIdx, "Type");
	if ((TypeTag < 0) || (a_NBT.GetType(TypeTag) != TAG_Int))
	{
		return;
	}
	int MinecartType = a_NBT.GetInt(TypeTag);
	switch (MinecartType)
	{
		case 0: LoadRideableMinecart(a_Entities, a_NBT, a_TagIdx); break;  // Rideable minecart
		case 1: LoadChestMinecart(a_Entities, a_NBT, a_TagIdx); break;  // Minecart with chest
		case 2: LoadFurnaceMinecart(a_Entities, a_NBT, a_TagIdx); break;  // Minecart with furnace
		case 3: LoadTNTMinecart(a_Entities, a_NBT, a_TagIdx); break;  // Minecart with TNT
		case 4: LoadHopperMinecart(a_Entities, a_NBT, a_TagIdx); break;  // Minecart with Hopper
		default: LOGWARNING("cWSSAnvilHandler_1_8::LoadOldMinecart: Unhandled minecart type (%d)", MinecartType); break;
	}
}





void cWSSAnvilHandler_1_8::LoadRideableMinecart(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Minecart = std::make_unique<cRideableMinecart>(Vector3d(), cItem(), 1);  // TODO: Load the block and the height
	if (!LoadEntityBase(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.emplace_back(std::move(Minecart));
}





void cWSSAnvilHandler_1_8::LoadChestMinecart(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return;  // Make it an empty chest - the chunk loader will provide an empty cChestEntity for this
	}
	auto Minecart = std::make_unique<cMinecartWithChest>(Vector3d());
	if (!LoadEntityBase(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	for (int Child = a_NBT.GetFirstChild(Items); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		int Slot = a_NBT.FindChildByName(Child, "Slot");
		if ((Slot < 0) || (a_NBT.GetType(Slot) != TAG_Byte))
		{
			continue;
		}
		cItem Item;
		if (LoadItem(Item, a_NBT, Child))
		{
			Minecart->SetSlot(a_NBT.GetByte(Slot), Item);
		}
	}  // for itr - ItemDefs[]
	a_Entities.emplace_back(std::move(Minecart));
}





void cWSSAnvilHandler_1_8::LoadFurnaceMinecart(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Minecart = std::make_unique<cMinecartWithFurnace>(Vector3d());
	if (!LoadEntityBase(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Load the Push and Fuel tags

	a_Entities.emplace_back(std::move(Minecart));
}





void cWSSAnvilHandler_1_8::LoadTNTMinecart(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Minecart = std::make_unique<cMinecartWithTNT>(Vector3d());
	if (!LoadEntityBase(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Everything to do with TNT carts

	a_Entities.emplace_back(std::move(Minecart));
}





void cWSSAnvilHandler_1_8::LoadHopperMinecart(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Minecart = std::make_unique<cMinecartWithHopper>(Vector3d());
	if (!LoadEntityBase(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Everything to do with hopper carts

	a_Entities.emplace_back(std::move(Minecart));
}





bool cWSSAnvilHandler_1_8::LoadProjectileBase(cProjectileEntity & a_Entity, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	if (!LoadEntityBase(a_Entity, a_NBT, a_TagIdx))
	{
		return false;
	}

	bool IsInGround = false;
	int InGroundIdx = a_NBT.FindChildByName(a_TagIdx, "inGround");
	if (InGroundIdx > 0)
	{
		IsInGround = (a_NBT.GetByte(InGroundIdx) != 0);
	}
	a_Entity.SetIsInGround(IsInGround);

	return true;
}





void cWSSAnvilHandler_1_8::LoadArrow(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Arrow = std::make_unique<cArrowEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBase(*Arrow.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Load pickup state:
	int PickupIdx = a_NBT.FindChildByName(a_TagIdx, "pickup");
	if ((PickupIdx > 0) && (a_NBT.GetType(PickupIdx) == TAG_Byte))
	{
		Arrow->SetPickupState(static_cast<cArrowEntity::ePickupState>(a_NBT.GetByte(PickupIdx)));
	}
	else
	{
		// Try the older "player" tag:
		int PlayerIdx = a_NBT.FindChildByName(a_TagIdx, "player");
		if ((PlayerIdx > 0) && (a_NBT.GetType(PlayerIdx) == TAG_Byte))
		{
			Arrow->SetPickupState((a_NBT.GetByte(PlayerIdx) == 0) ? cArrowEntity::psNoPickup : cArrowEntity::psInSurvivalOrCreative);
		}
	}

	// Load damage:
	int DamageIdx = a_NBT.FindChildByName(a_TagIdx, "damage");
	if ((DamageIdx > 0) && (a_NBT.GetType(DamageIdx) == TAG_Double))
	{
		Arrow->SetDamageCoeff(a_NBT.GetDouble(DamageIdx));
	}

	// Load block hit:
	int InBlockXIdx = a_NBT.FindChildByName(a_TagIdx, "xTile");
	int InBlockYIdx = a_NBT.FindChildByName(a_TagIdx, "yTile");
	int InBlockZIdx = a_NBT.FindChildByName(a_TagIdx, "zTile");
	if ((InBlockXIdx > 0) && (InBlockYIdx > 0) && (InBlockZIdx > 0))
	{
		eTagType typeX = a_NBT.GetType(InBlockXIdx);
		if ((typeX == a_NBT.GetType(InBlockYIdx)) &&  (typeX == a_NBT.GetType(InBlockZIdx)))
		{
			switch (typeX)
			{
				case TAG_Int:
				{
					// Old MCS code used this, keep reading it for compatibility reasons:
					Arrow->SetBlockHit(Vector3i(a_NBT.GetInt(InBlockXIdx), a_NBT.GetInt(InBlockYIdx), a_NBT.GetInt(InBlockZIdx)));
					break;
				}
				case TAG_Short:
				{
					// Vanilla uses this
					Arrow->SetBlockHit(Vector3i(a_NBT.GetShort(InBlockXIdx), a_NBT.GetShort(InBlockYIdx), a_NBT.GetShort(InBlockZIdx)));
					break;
				}
				default:
				{
					// No hit block, the arrow is still flying?
					break;
				}
			}
		}
	}

	// Store the new arrow in the entities list:
	a_Entities.emplace_back(std::move(Arrow));
}





void cWSSAnvilHandler_1_8::LoadEgg(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Egg = std::make_unique<cThrownEggEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBase(*Egg.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new egg in the entities list:
	a_Entities.emplace_back(std::move(Egg));
}





void cWSSAnvilHandler_1_8::LoadFireball(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Fireball = std::make_unique<cGhastFireballEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBase(*Fireball.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new fireball in the entities list:
	a_Entities.emplace_back(std::move(Fireball));
}





void cWSSAnvilHandler_1_8::LoadFireCharge(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto FireCharge = std::make_unique<cFireChargeEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBase(*FireCharge.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new FireCharge in the entities list:
	a_Entities.emplace_back(std::move(FireCharge));
}





void cWSSAnvilHandler_1_8::LoadSnowball(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Snowball = std::make_unique<cThrownSnowballEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBase(*Snowball.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new snowball in the entities list:
	a_Entities.emplace_back(std::move(Snowball));
}





void cWSSAnvilHandler_1_8::LoadSplashPotion(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto SplashPotion = std::make_unique<cSplashPotionEntity>(nullptr, Vector3d(), Vector3d(), cItem());
	if (!LoadProjectileBase(*SplashPotion.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int EffectDuration         = a_NBT.FindChildByName(a_TagIdx, "EffectDuration");
	int EffectIntensity        = a_NBT.FindChildByName(a_TagIdx, "EffectIntensity");
	int EffectDistanceModifier = a_NBT.FindChildByName(a_TagIdx, "EffectDistanceModifier");

	SplashPotion->SetEntityEffectType(static_cast<cEntityEffect::eType>(a_NBT.FindChildByName(a_TagIdx, "EffectType")));
	SplashPotion->SetEntityEffect(cEntityEffect(EffectDuration, static_cast<Int16>(EffectIntensity), EffectDistanceModifier));
	SplashPotion->SetPotionColor(a_NBT.FindChildByName(a_TagIdx, "PotionName"));

	// Store the new splash potion in the entities list:
	a_Entities.emplace_back(std::move(SplashPotion));
}





void cWSSAnvilHandler_1_8::LoadThrownEnderPearl(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto EnderPearl = std::make_unique<cThrownEnderPearlEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBase(*EnderPearl.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new enderpearl in the entities list:
	a_Entities.emplace_back(std::move(EnderPearl));
}





bool cWSSAnvilHandler_1_8::LoadMonsterBase(cMonster & a_Monster, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::array<float, 5> DropChance;
	if (LoadFloatsList<5>(DropChance, a_NBT, a_NBT.FindChildByName(a_TagIdx, "DropChances")))
	{
		a_Monster.SetDropChanceWeapon(DropChance[0]);
		a_Monster.SetDropChanceHelmet(DropChance[1]);
		a_Monster.SetDropChanceChestplate(DropChance[2]);
		a_Monster.SetDropChanceLeggings(DropChance[3]);
		a_Monster.SetDropChanceBoots(DropChance[4]);
	}

	std::array<float, 2> HandDropChance;
	if (LoadFloatsList<2>(HandDropChance, a_NBT, a_NBT.FindChildByName(a_TagIdx, "HandDropChances")))
	{
		a_Monster.SetDropChanceWeapon(HandDropChance[0]);
		// Todo: What is the second number for?
	}

	std::array<float, 4> ArmorDropChance;
	if (LoadFloatsList<4>(ArmorDropChance, a_NBT, a_NBT.FindChildByName(a_TagIdx, "ArmorDropChances")))
	{
		a_Monster.SetDropChanceHelmet(ArmorDropChance[0]);
		a_Monster.SetDropChanceChestplate(ArmorDropChance[1]);
		a_Monster.SetDropChanceLeggings(ArmorDropChance[2]);
		a_Monster.SetDropChanceBoots(ArmorDropChance[3]);
	}

	int LootTag = a_NBT.FindChildByName(a_TagIdx, "CanPickUpLoot");
	if (LootTag > 0)
	{
		bool CanPickUpLoot = (a_NBT.GetByte(LootTag) == 1);
		a_Monster.SetCanPickUpLoot(CanPickUpLoot);
	}

	int CustomNameTag = a_NBT.FindChildByName(a_TagIdx, "CustomName");
	if ((CustomNameTag > 0) && (a_NBT.GetType(CustomNameTag) == TAG_String))
	{
		a_Monster.SetCustomName(a_NBT.GetString(CustomNameTag));
	}

	int CustomNameVisibleTag = a_NBT.FindChildByName(a_TagIdx, "CustomNameVisible");
	if ((CustomNameVisibleTag > 0) && (a_NBT.GetType(CustomNameVisibleTag) == TAG_Byte))
	{
		bool CustomNameVisible = (a_NBT.GetByte(CustomNameVisibleTag) == 1);
		a_Monster.SetCustomNameAlwaysVisible(CustomNameVisible);
	}

	// Leashed to a knot
	int LeashedIdx = a_NBT.FindChildByName(a_TagIdx, "Leashed");
	if ((LeashedIdx >= 0) && a_NBT.GetByte(LeashedIdx))
	{
		LoadLeashToPosition(a_Monster, a_NBT, a_TagIdx);
	}

	return true;
}





void cWSSAnvilHandler_1_8::LoadLeashToPosition(cMonster & a_Monster, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int LeashIdx = a_NBT.FindChildByName(a_TagIdx, "Leash");
	if (LeashIdx < 0)
	{
		return;
	}

	double PosX = 0.0, PosY = 0.0, PosZ = 0.0;
	bool KnotPosPresent = true;
	int LeashDataLine = a_NBT.FindChildByName(LeashIdx, "X");
	if (LeashDataLine >= 0)
	{
		PosX = a_NBT.GetDouble(LeashDataLine);
	}
	else
	{
		KnotPosPresent = false;
	}
	LeashDataLine = a_NBT.FindChildByName(LeashIdx, "Y");
	if (LeashDataLine >= 0)
	{
		PosY = a_NBT.GetDouble(LeashDataLine);
	}
	else
	{
		KnotPosPresent = false;
	}
	LeashDataLine = a_NBT.FindChildByName(LeashIdx, "Z");
	if (LeashDataLine >= 0)
	{
		PosZ = a_NBT.GetDouble(LeashDataLine);
	}
	else
	{
		KnotPosPresent = false;
	}
	if (KnotPosPresent)
	{
		// Set leash pos for the mob
		a_Monster.SetLeashToPos(new Vector3d(PosX, PosY, PosZ));
	}
}





std::pair<AString, cUUID> cWSSAnvilHandler_1_8::LoadEntityOwner(const cParsedNBT & a_NBT, int a_TagIdx) const
{
	// Load the owner information. OwnerUUID or Owner may be specified, possibly both:
	AString OwnerName;
	cUUID OwnerUUID;
	int OwnerUUIDIdx = a_NBT.FindChildByName(a_TagIdx, "OwnerUUID");
	if (OwnerUUIDIdx > 0)
	{
		OwnerUUID.FromString(a_NBT.GetString(OwnerUUIDIdx));
	}
	int OwnerIdx = a_NBT.FindChildByName(a_TagIdx, "Owner");
	if (OwnerIdx > 0)
	{
		OwnerName = a_NBT.GetString(OwnerIdx);
	}
	if (OwnerName.empty() && OwnerUUID.IsNil())
	{
		// There is no owner, bail out:
		return {};
	}

	// Convert name to UUID, if needed:
	if (OwnerUUID.IsNil())
	{
		// This entity has only playername stored (pre-1.7.6), look up the UUID
		// The lookup is blocking, but we're running in a separate thread, so it's ok
		OwnerUUID = cRoot::Get()->GetMojangAPI().GetUUIDFromPlayerName(OwnerName);
		if (OwnerUUID.IsNil())
		{
			// Not a known player, un-tame the entity by bailing out
			return {};
		}
	}

	// Convert UUID to name, if needed:
	if (OwnerName.empty())
	{
		// The lookup is blocking, but we're running in a separate thread, so it's ok
		OwnerName = cRoot::Get()->GetMojangAPI().GetPlayerNameFromUUID(OwnerUUID);
		if (OwnerName.empty())
		{
			// Not a known player, un-tame the entity by bailing out
			return {};
		}
	}

	return { OwnerName, OwnerUUID };
}





void cWSSAnvilHandler_1_8::LoadBat(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Monster = std::make_unique<cBat>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadBlaze(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cBlaze> Monster = std::make_unique<cBlaze>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadCaveSpider(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cCaveSpider> Monster = std::make_unique<cCaveSpider>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadChicken(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cChicken> Monster = std::make_unique<cChicken>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadCow(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cCow> Monster = std::make_unique<cCow>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadCreeper(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cCreeper> Monster = std::make_unique<cCreeper>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadEnderDragon(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cEnderDragon> Monster = std::make_unique<cEnderDragon>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadEnderman(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cEnderman> Monster = std::make_unique<cEnderman>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadGhast(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cGhast> Monster = std::make_unique<cGhast>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadGiant(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cGiant> Monster = std::make_unique<cGiant>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadGuardian(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cGuardian> Monster = std::make_unique<cGuardian>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadHorse(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int TypeIdx  = a_NBT.FindChildByName(a_TagIdx, "Type");
	int ColorIdx = a_NBT.FindChildByName(a_TagIdx, "Color");
	int StyleIdx = a_NBT.FindChildByName(a_TagIdx, "Style");
	if ((TypeIdx < 0) || (ColorIdx < 0) || (StyleIdx < 0))
	{
		return;
	}

	int Type = a_NBT.GetInt(TypeIdx);
	int Color = a_NBT.GetInt(ColorIdx);
	int Style = a_NBT.GetInt(StyleIdx);

	std::unique_ptr<cHorse> Monster = std::make_unique<cHorse>(Type, Color, Style, 1);

	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadIronGolem(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cIronGolem> Monster = std::make_unique<cIronGolem>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadMagmaCube(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int SizeIdx = a_NBT.FindChildByName(a_TagIdx, "Size");

	if (SizeIdx < 0)
	{
		return;
	}

	int Size = a_NBT.GetInt(SizeIdx);

	std::unique_ptr<cMagmaCube> Monster = std::make_unique<cMagmaCube>(Size);
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadMooshroom(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cMooshroom> Monster = std::make_unique<cMooshroom>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadOcelot(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cOcelot> Monster = std::make_unique<cOcelot>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	auto OwnerInfo = LoadEntityOwner(a_NBT, a_TagIdx);
	if (!OwnerInfo.first.empty() && !OwnerInfo.second.IsNil())
	{
		Monster->SetOwner(OwnerInfo.first, OwnerInfo.second);
		Monster->SetIsTame(true);
	}

	int TypeIdx  = a_NBT.FindChildByName(a_TagIdx, "CatType");
	if (TypeIdx > 0)
	{
		int Type = a_NBT.GetInt(TypeIdx);
		Monster->SetCatType(static_cast<cOcelot::eCatType>(Type));
	}

	int SittingIdx = a_NBT.FindChildByName(a_TagIdx, "Sitting");
	if ((SittingIdx > 0) && (a_NBT.GetType(SittingIdx) == TAG_Byte))
	{
		bool Sitting = (a_NBT.GetByte(SittingIdx) == 1);
		Monster->SetIsSitting(Sitting);
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadPig(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cPig> Monster = std::make_unique<cPig>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadRabbit(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int TypeIdx  = a_NBT.FindChildByName(a_TagIdx, "RabbitType");
	int MoreCarrotTicksIdx = a_NBT.FindChildByName(a_TagIdx, "MoreCarrotTicks");

	if ((TypeIdx < 0) || (MoreCarrotTicksIdx < 0))
	{
		return;
	}

	int Type = a_NBT.GetInt(TypeIdx);
	int MoreCarrotTicks = a_NBT.GetInt(MoreCarrotTicksIdx);

	std::unique_ptr<cRabbit> Monster = std::make_unique<cRabbit>(static_cast<eRabbitType>(Type), MoreCarrotTicks);
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadSheep(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int ColorIdx = a_NBT.FindChildByName(a_TagIdx, "Color");
	int Color = -1;
	if (ColorIdx > 0)
	{
		Color = static_cast<int>(a_NBT.GetByte(ColorIdx));
	}

	std::unique_ptr<cSheep> Monster = std::make_unique<cSheep>(Color);
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int ShearedIdx = a_NBT.FindChildByName(a_TagIdx, "Sheared");
	if (ShearedIdx > 0)
	{
		Monster->SetSheared(a_NBT.GetByte(ShearedIdx) != 0);
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadSilverfish(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cSilverfish> Monster = std::make_unique<cSilverfish>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadSkeleton(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	// Wither skeleton is a separate mob in Minecraft 1.11+, but we need this to
	// load them from older worlds where wither skeletons were only a skeleton with a flag
	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "SkeletonType");

	std::unique_ptr<cMonster> Monster;
	if ((TypeIdx > 0) && (a_NBT.GetByte(TypeIdx) == 1))
	{
		Monster = std::make_unique<cWitherSkeleton>();
	}
	else
	{
		Monster = std::make_unique<cSkeleton>();
	}

	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadSlime(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int SizeIdx = a_NBT.FindChildByName(a_TagIdx, "Size");

	if (SizeIdx < 0)
	{
		return;
	}

	int Size = a_NBT.GetInt(SizeIdx);

	std::unique_ptr<cSlime> Monster = std::make_unique<cSlime>(Size);
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadSnowGolem(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cSnowGolem> Monster = std::make_unique<cSnowGolem>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadSpider(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cSpider> Monster = std::make_unique<cSpider>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadSquid(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cSquid> Monster = std::make_unique<cSquid>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadVillager(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "Profession");
	if (TypeIdx < 0)
	{
		return;
	}

	int Type = a_NBT.GetInt(TypeIdx);

	std::unique_ptr<cVillager> Monster = std::make_unique<cVillager>(cVillager::eVillagerType(Type));
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	int InventoryIdx = a_NBT.FindChildByName(a_TagIdx, "Inventory");
	if (InventoryIdx > 0)
	{
		LoadItemGrid(Monster->GetInventory(), a_NBT, InventoryIdx);
	}


	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadWitch(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cWitch> Monster = std::make_unique<cWitch>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadWither(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cWither> Monster = std::make_unique<cWither>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int CurrLine = a_NBT.FindChildByName(a_TagIdx, "Invul");
	if (CurrLine > 0)
	{
		Monster->SetWitherInvulnerableTicks(static_cast<unsigned int>(a_NBT.GetInt(CurrLine)));
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadWitherSkeleton(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Monster = std::make_unique<cWitherSkeleton>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadWolf(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cWolf> Monster = std::make_unique<cWolf>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	auto OwnerInfo = LoadEntityOwner(a_NBT, a_TagIdx);
	if (!OwnerInfo.first.empty() && !OwnerInfo.second.IsNil())
	{
		Monster->SetOwner(OwnerInfo.first, OwnerInfo.second);
		Monster->SetIsTame(true);
	}

	int SittingIdx = a_NBT.FindChildByName(a_TagIdx, "Sitting");
	if ((SittingIdx > 0) && (a_NBT.GetType(SittingIdx) == TAG_Byte))
	{
		bool Sitting = (a_NBT.GetByte(SittingIdx) == 1);
		Monster->SetIsSitting(Sitting);
	}
	int AngryIdx = a_NBT.FindChildByName(a_TagIdx, "Angry");
	if ((AngryIdx > 0) && (a_NBT.GetType(AngryIdx) == TAG_Byte))
	{
		bool Angry = (a_NBT.GetByte(AngryIdx) == 1);
		Monster->SetIsAngry(Angry);
	}
	int CollarColorIdx = a_NBT.FindChildByName(a_TagIdx, "CollarColor");
	if (CollarColorIdx > 0)
	{
		switch (a_NBT.GetType(CollarColorIdx))
		{
			case TAG_Byte:
			{
				// Vanilla uses this
				Monster->SetCollarColor(a_NBT.GetByte(CollarColorIdx));
				break;
			}
			case TAG_Int:
			{
				// Old MCS code used this, keep reading it for compatibility reasons:
				Monster->SetCollarColor(a_NBT.GetInt(CollarColorIdx));
				break;
			}
			default:
			{
				// No other values are interpreted
				break;
			}
		}
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadZombie(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cZombie> Monster = std::make_unique<cZombie>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt(AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadZombifiedPiglin(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	std::unique_ptr<cZombiePigman> Monster = std::make_unique<cZombiePigman>();
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvilHandler_1_8::LoadZombieVillager(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	int ProfessionIdx = a_NBT.FindChildByName(a_TagIdx, "Profession");
	if (ProfessionIdx < 0)
	{
		return;
	}

	cVillager::eVillagerType Profession = static_cast<cVillager::eVillagerType>(a_NBT.GetInt(ProfessionIdx));

	std::unique_ptr<cZombieVillager> Monster = std::make_unique<cZombieVillager>(Profession);
	if (!LoadEntityBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBase(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Conversion time

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default:       Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





bool cWSSAnvilHandler_1_8::GetBlockEntityNBTPos(const cParsedNBT & a_NBT, int a_TagIdx, Vector3i & a_AbsPos) const
{
	int x = a_NBT.FindChildByName(a_TagIdx, "x");
	if ((x < 0) || (a_NBT.GetType(x) != TAG_Int))
	{
		return false;
	}
	int y = a_NBT.FindChildByName(a_TagIdx, "y");
	if ((y < 0) || (a_NBT.GetType(y) != TAG_Int))
	{
		return false;
	}
	int z = a_NBT.FindChildByName(a_TagIdx, "z");
	if ((z < 0) || (a_NBT.GetType(z) != TAG_Int))
	{
		return false;
	}
	a_AbsPos.Set(
		Clamp(a_NBT.GetInt(x), -40000000, 40000000),  // World is limited to 30M blocks in XZ, we clamp to 40M
		Clamp(a_NBT.GetInt(y), -10000,    10000),     // Y is limited to 0 .. 255, we clamp to 10K
		Clamp(a_NBT.GetInt(z), -40000000, 40000000)
	);
	return true;
}





bool cWSSAnvilHandler_1_8::CheckBlockEntityType(const cParsedNBT & a_NBT, int a_TagIdx, const AStringVector & a_ExpectedTypes, Vector3i a_Pos) const
{
	// Check if the given tag is a compound:
	if (a_NBT.GetType(a_TagIdx) != TAG_Compound)
	{
		return false;
	}

	// Get the "id" tag:
	int TagID = a_NBT.FindChildByName(a_TagIdx, "id");
	if (TagID < 0)
	{
		return false;
	}

	// Check if the "id" tag is a string:
	if (a_NBT.GetType(TagID) != eTagType::TAG_String)
	{
		return false;
	}

	// Compare the value:
	for (const auto & et: a_ExpectedTypes)
	{
		if (a_NBT.GetStringView(TagID) == et)
		{
			return true;
		}
	}

	// Expectation not met, output an error into the log:
	AString expectedTypes;
	for (const auto & et : a_ExpectedTypes)
	{
		expectedTypes.append(", \"");
		expectedTypes.append(et);
		expectedTypes.push_back('\"');
	}
	FLOGWARNING("Block entity type mismatch: exp {0}, got \"{1}\". The block entity at {2} will lose all its properties.",
		expectedTypes.c_str() + 2,  // Skip the first ", " that is extra in the string
		a_NBT.GetStringView(TagID), a_Pos
	);
	return false;
}





void cWSSAnvilHandler_1_8::LoadBlockEntities(cBlockEntities & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx, const ChunkBlockData & a_BlockData, cWorld * a_World) const
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_List))
	{
		return;
	}

	for (int Child = a_NBT.GetFirstChild(a_TagIdx); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		if (a_NBT.GetType(Child) != TAG_Compound)
		{
			continue;
		}

		// Get the BlockEntity's position
		Vector3i absPos;
		if (!GetBlockEntityNBTPos(a_NBT, Child, absPos) || (absPos.y < 0) || (absPos.y >= cChunkDef::Height))
		{
			LOGWARNING("Bad block entity, missing the coords. Will be ignored.");
			continue;
		}
		const auto relPos = cChunkDef::AbsoluteToRelative(absPos);

		// Load the proper BlockEntity type based on the block type:
		const auto BlockType = a_BlockData.GetBlock(relPos);
		const auto BlockMeta = a_BlockData.GetMeta(relPos);
		OwnedBlockEntity Entity;

		try
		{
			Entity = LoadBlockEntity(a_NBT, Child, absPos, BlockType, BlockMeta, a_World);
		}
		catch (...)
		{
			continue;
		}

		// TODO: exception-ify the failure case
		if (Entity == nullptr)
		{
			continue;
		}

		// Index computed before Entity moved.
		const auto Index = cChunkDef::MakeIndex(Entity->GetRelPos());

		// Add the BlockEntity to the loaded data:
		a_BlockEntities.emplace(Index, std::move(Entity));
	}  // for Child - tag children
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadBlockEntity(const cParsedNBT & a_NBT, int a_Tag, Vector3i a_Pos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cWorld * a_World) const
{
	ASSERT((a_Pos.y >= 0) && (a_Pos.y < cChunkDef::Height));

	// Load the specific BlockEntity type:
	switch (a_BlockType)
	{
		// Banners:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_WALL_BANNER:        return LoadBanner          (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);

		// Others:
		case E_BLOCK_BEACON:             return LoadBeacon          (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_BED:                return LoadBed             (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_BREWING_STAND:      return LoadBrewingstand    (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_CHEST:              return LoadChest           (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_COMMAND_BLOCK:      return LoadCommandBlock    (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_DISPENSER:          return LoadDispenser       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_DROPPER:            return LoadDropper         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_ENCHANTMENT_TABLE:  return LoadEnchantingTable (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_ENDER_CHEST:        return LoadEnderChest      (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_END_PORTAL:         return LoadEndPortal       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_FLOWER_POT:         return LoadFlowerPot       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_FURNACE:            return LoadFurnace         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_HEAD:               return LoadMobHead         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_HOPPER:             return LoadHopper          (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_JUKEBOX:            return LoadJukebox         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_LIT_FURNACE:        return LoadFurnace         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_MOB_SPAWNER:        return LoadMobSpawner      (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_NOTE_BLOCK:         return LoadNoteBlock       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_SIGN_POST:          return LoadSign            (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_TRAPPED_CHEST:      return LoadChest           (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_WALLSIGN:           return LoadSign            (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos, a_World);
		default:
		{
			// All the other blocktypes should have no entities assigned to them. Report an error:
			// Get the "id" tag:
			int TagID = a_NBT.FindChildByName(a_Tag, "id");
			FLOGINFO("WorldLoader({0}): Block entity mismatch: block type {1} ({2}), type \"{3}\", at {4}; the entity will be lost.",
				a_World->GetName(),
				ItemTypeToString(a_BlockType), a_BlockType, (TagID >= 0) ? a_NBT.GetStringView(TagID) : "unknown",
				a_Pos
			);
			return nullptr;
		}
	}
}





AString cWSSAnvilHandler_1_8::DecodeSignLine(const AString & a_Line) const
{
	if (a_Line.empty())
	{
		return AString();
	}
	if (a_Line[0] != '{')
	{
		return a_Line;
	}

	// Try to parse the JSON:
	Json::Value root;
	if (!JsonUtils::ParseString(a_Line, root) || !root.isObject())
	{
		return a_Line;
	}
	const auto & txt = root["text"];
	if (txt.isString())
	{
		return txt.asString();
	}
	return a_Line;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadBanner(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	static const AStringVector expectedTypes({"Banner", "minecraft:standingbanner","minecraft:wallbanner"});
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	// Reads base color from NBT
	int CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Base");
	if (CurrentLine >= 0)
	{
		const auto Color = static_cast<unsigned char>(a_NBT.GetInt(CurrentLine));
		return std::make_unique<cBannerEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World, Color);
	}

	return nullptr;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadBeacon(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({"Beacon", "minecraft:beacon"});
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto Beacon = std::make_unique<cBeaconEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);

	int CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Levels");
	if (CurrentLine >= 0)
	{
		Beacon->SetBeaconLevel(static_cast<char>(a_NBT.GetInt(CurrentLine)));
	}

	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Primary");
	if (CurrentLine >= 0)
	{
		Beacon->SetPrimaryEffect(static_cast<cEntityEffect::eType>(a_NBT.GetInt(CurrentLine)));
	}

	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Secondary");
	if (CurrentLine >= 0)
	{
		Beacon->SetSecondaryEffect(static_cast<cEntityEffect::eType>(a_NBT.GetInt(CurrentLine)));
	}

	// We are better than mojang, we load / save the beacon inventory!
	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items >= 0) && (a_NBT.GetType(Items) == TAG_List))
	{
		LoadItemGrid(Beacon->GetContents(), a_NBT, Items);
	}

	return Beacon;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadBed(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Bed", "minecraft:bed" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	// Use color red as default
	short Color = E_META_WOOL_RED;

	int ColorIDx = a_NBT.FindChildByName(a_TagIdx, "color");
	if (ColorIDx >= 0)
	{
		Color = static_cast<short>(a_NBT.GetInt(ColorIDx));
	}

	return std::make_unique<cBedEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World, Color);
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadBrewingstand(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Brewingstand", "minecraft:brewing_stand" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty brewingstand - the chunk loader will provide an empty cBrewingstandEntity for this
	}

	auto Brewingstand = std::make_unique<cBrewingstandEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);

	// Fuel has to be loaded at first, because of slot events:
	int Fuel = a_NBT.FindChildByName(a_TagIdx, "Fuel");
	if (Fuel >= 0)
	{
		Int16 tb = a_NBT.GetShort(Fuel);
		Brewingstand->SetRemainingFuel(tb);
	}

	// Load slots:
	for (int Child = a_NBT.GetFirstChild(Items); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		int Slot = a_NBT.FindChildByName(Child, "Slot");
		if ((Slot < 0) || (a_NBT.GetType(Slot) != TAG_Byte))
		{
			continue;
		}
		cItem Item;
		if (LoadItem(Item, a_NBT, Child))
		{
			Brewingstand->SetSlot(a_NBT.GetByte(Slot), Item);
		}
	}  // for itr - ItemDefs[]

	// Load brewing time:
	int BrewTime = a_NBT.FindChildByName(a_TagIdx, "BrewTime");
	if (BrewTime >= 0)
	{
		Int16 tb = a_NBT.GetShort(BrewTime);
		Brewingstand->SetTimeBrewed(tb);
	}

	// Restart brewing:
	Brewingstand->LoadRecipes();
	Brewingstand->ContinueBrewing();
	return Brewingstand;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadChest(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	// Note that older Cuberite code used "TrappedChest" for trapped chests; new code mimics vanilla and uses "Chest" throughout, but we allow migration here:
	static const AStringVector expectedTypes({ "Chest", "TrappedChest", "minecraft:chest" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty chest - the chunk loader will provide an empty cChestEntity for this
	}
	auto Chest = std::make_unique<cChestEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
	LoadItemGrid(Chest->GetContents(), a_NBT, Items);
	return Chest;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadCommandBlock(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Control", "minecraft:command_block" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto CmdBlock = std::make_unique<cCommandBlockEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "Command");
	if (currentLine >= 0)
	{
		CmdBlock->SetCommand(a_NBT.GetString(currentLine));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "SuccessCount");
	if (currentLine >= 0)
	{
		CmdBlock->SetResult(static_cast<NIBBLETYPE>(a_NBT.GetInt(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "LastOutput");
	if (currentLine >= 0)
	{
		CmdBlock->SetLastOutput(a_NBT.GetString(currentLine));
	}

	// TODO 2014-01-18 xdot: Figure out what TrackOutput is and parse it.

	return CmdBlock;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadDispenser(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Trap", "minecraft:dispenser" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty dispenser - the chunk loader will provide an empty cDispenserEntity for this
	}
	auto Dispenser = std::make_unique<cDispenserEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
	LoadItemGrid(Dispenser->GetContents(), a_NBT, Items);
	return Dispenser;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadDropper(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Dropper", "minecraft:dropper" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty dropper - the chunk loader will provide an empty cDropperEntity for this
	}
	auto Dropper = std::make_unique<cDropperEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
	LoadItemGrid(Dropper->GetContents(), a_NBT, Items);
	return Dropper;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadEnchantingTable(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "EnchantingTable", "minecraft:enchanting_table" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	AString CustomName;
	int currentLine = a_NBT.FindChildByName(a_TagIdx, "CustomName");
	if (currentLine >= 0)
	{
		if (a_NBT.GetType(currentLine) == TAG_String)
		{
			CustomName = a_NBT.GetString(currentLine);
		}
	}
	return std::make_unique<cEnchantingTableEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World, CustomName);
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadEnderChest(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "EnderChest", "minecraft:ender_chest" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}
	return std::make_unique<cEnderChestEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadEndPortal(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "EndPortal", "minecraft:end_portal" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}
	return std::make_unique<cEndPortalEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadFlowerPot(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "FlowerPot", "minecraft:flower_pot" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto FlowerPot = std::make_unique<cFlowerPotEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
	cItem Item;

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "Item");
	if (currentLine >= 0)
	{
		if (a_NBT.GetType(currentLine) == TAG_String)
		{
			StringToItem(a_NBT.GetString(currentLine), Item);
		}
		else if (a_NBT.GetType(currentLine) == TAG_Int)
		{
			Item.m_ItemType = static_cast<short>(a_NBT.GetInt(currentLine));
		}
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Data");
	if ((currentLine >= 0) && (a_NBT.GetType(currentLine) == TAG_Int))
	{
		Item.m_ItemDamage = static_cast<short>(a_NBT.GetInt(currentLine));
	}

	FlowerPot->SetItem(Item);
	return FlowerPot;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadFurnace(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Furnace", "minecraft:furnace" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty furnace - the chunk loader will provide an empty cFurnaceEntity for this
	}

	auto Furnace = std::make_unique<cFurnaceEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
	Furnace->SetLoading(true);

	// Load slots:
	for (int Child = a_NBT.GetFirstChild(Items); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		int Slot = a_NBT.FindChildByName(Child, "Slot");
		if ((Slot < 0) || (a_NBT.GetType(Slot) != TAG_Byte))
		{
			continue;
		}
		cItem Item;
		if (LoadItem(Item, a_NBT, Child))
		{
			Furnace->SetSlot(a_NBT.GetByte(Slot), Item);
		}
	}  // for itr - ItemDefs[]

	// Load burn time:
	int BurnTime = a_NBT.FindChildByName(a_TagIdx, "BurnTime");
	if (BurnTime >= 0)
	{
		Int16 bt = a_NBT.GetShort(BurnTime);
		// Anvil doesn't store the time that the fuel can burn. We simply "reset" the current value to be the 100%
		Furnace->SetBurnTimes(bt, 0);
	}

	// Load cook time:
	int CookTime = a_NBT.FindChildByName(a_TagIdx, "CookTime");
	if (CookTime >= 0)
	{
		Int16 ct = a_NBT.GetShort(CookTime);
		// Anvil doesn't store the time that an item takes to cook. We simply use the default - 10 seconds (200 ticks)
		Furnace->SetCookTimes(200, ct);
	}
	// Restart cooking:
	Furnace->ContinueCooking();
	Furnace->SetLoading(false);
	return Furnace;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadHopper(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Hopper", "minecraft:hopper" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty hopper - the chunk loader will provide an empty cHopperEntity for this
	}
	auto Hopper = std::make_unique<cHopperEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
	LoadItemGrid(Hopper->GetContents(), a_NBT, Items);
	return Hopper;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadJukebox(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "RecordPlayer", "minecraft:jukebox" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto Jukebox = std::make_unique<cJukeboxEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
	int Record = a_NBT.FindChildByName(a_TagIdx, "Record");
	if (Record >= 0)
	{
		Jukebox->SetRecord(a_NBT.GetInt(Record));
	}
	return Jukebox;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadMobSpawner(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "MobSpawner", "minecraft:mob_spawner" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto MobSpawner = std::make_unique<cMobSpawnerEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);

	// Load entity type
	int Type = a_NBT.FindChildByName(a_TagIdx, "EntityId");
	if ((Type >= 0) && (a_NBT.GetType(Type) == TAG_String))
	{
		const auto StatInfo = NamespaceSerializer::SplitNamespacedID(a_NBT.GetStringView(Type));
		if (StatInfo.first == NamespaceSerializer::Namespace::Unknown)
		{
			return nullptr;
		}

		eMonsterType MonsterType = NamespaceSerializer::ToMonsterType(StatInfo.second);
		if (MonsterType != eMonsterType::mtInvalidType)
		{
			MobSpawner->SetEntity(MonsterType);
		}
	}

	// Load spawn count:
	int CurrentLine = a_NBT.FindChildByName(a_TagIdx, "SpawnCount");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetSpawnCount(a_NBT.GetShort(CurrentLine));
	}

	// Load spawn range:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "SpawnRange");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetSpawnRange(a_NBT.GetShort(CurrentLine));
	}

	// Load delay:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Delay");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetSpawnDelay(a_NBT.GetShort(CurrentLine));
	}

	// Load delay range:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "MinSpawnDelay");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetMinSpawnDelay(a_NBT.GetShort(CurrentLine));
	}

	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "MaxSpawnDelay");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetMaxSpawnDelay(a_NBT.GetShort(CurrentLine));
	}

	// Load MaxNearbyEntities:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "MaxNearbyEntities");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetMaxNearbyEntities(a_NBT.GetShort(CurrentLine));
	}

	// Load RequiredPlayerRange:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "RequiredPlayerRange");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetRequiredPlayerRange(a_NBT.GetShort(CurrentLine));
	}


	return MobSpawner;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadMobHead(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Skull", "minecraft:skull" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto MobHead = std::make_unique<cMobHeadEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "SkullType");
	if (currentLine >= 0)
	{
		MobHead->SetType(static_cast<eMobHeadType>(a_NBT.GetByte(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Rot");
	if (currentLine >= 0)
	{
		MobHead->SetRotation(static_cast<eMobHeadRotation>(a_NBT.GetByte(currentLine)));
	}

	int ownerLine = a_NBT.FindChildByName(a_TagIdx, "Owner");
	if (ownerLine >= 0)
	{
		AString OwnerName, OwnerTexture, OwnerTextureSignature;
		cUUID OwnerUUID;

		currentLine = a_NBT.FindChildByName(ownerLine, "Id");
		if (currentLine >= 0)
		{
			OwnerUUID.FromString(a_NBT.GetString(currentLine));
		}

		currentLine = a_NBT.FindChildByName(ownerLine, "Name");
		if (currentLine >= 0)
		{
			OwnerName = a_NBT.GetString(currentLine);
		}

		int textureLine = a_NBT.GetFirstChild(  // The first texture of
			a_NBT.FindChildByName(              // The texture list of
				a_NBT.FindChildByName(          // The Properties compound of
					ownerLine,                  // The Owner compound
					"Properties"
					),
				"textures"
				)
			);
		if (textureLine >= 0)
		{
			currentLine = a_NBT.FindChildByName(textureLine, "Signature");
			if (currentLine >= 0)
			{
				OwnerTextureSignature = a_NBT.GetString(currentLine);
			}

			currentLine = a_NBT.FindChildByName(textureLine, "Value");
			if (currentLine >= 0)
			{
				OwnerTexture = a_NBT.GetString(currentLine);
			}
		}
		MobHead->SetOwner(OwnerUUID, OwnerName, OwnerTexture, OwnerTextureSignature);
	}

	return MobHead;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadNoteBlock(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Music", "minecraft:noteblock" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto NoteBlock = std::make_unique<cNoteEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
	int note = a_NBT.FindChildByName(a_TagIdx, "note");
	if (note >= 0)
	{
		NoteBlock->SetNote(a_NBT.GetByte(note));
	}
	return NoteBlock;
}





OwnedBlockEntity cWSSAnvilHandler_1_8::LoadSign(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({"Sign", "minecraft:sign"});
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto Sign =
		std::make_unique<cSignEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "Text1");
	if (currentLine >= 0)
	{
		Sign->SetLine(0, DecodeSignLine(a_NBT.GetString(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Text2");
	if (currentLine >= 0)
	{
		Sign->SetLine(1, DecodeSignLine(a_NBT.GetString(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Text3");
	if (currentLine >= 0)
	{
		Sign->SetLine(2, DecodeSignLine(a_NBT.GetString(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Text4");
	if (currentLine >= 0)
	{
		Sign->SetLine(3, DecodeSignLine(a_NBT.GetString(currentLine)));
	}

	return Sign;
}





bool cWSSAnvilHandler_1_8::LoadHeightMap(cChunkDef::HeightMap & a_HeightMap, const cParsedNBT & a_NBT, const int a_TagIdx) const
{
	if (
		(a_TagIdx < 0) ||
		(a_NBT.GetType(a_TagIdx) != TAG_IntArray) ||
		(a_NBT.GetDataLength(a_TagIdx) != (4 * std::size(a_HeightMap)))
	)
	{
		return false;
	}

	const auto * const HeightData = a_NBT.GetData(a_TagIdx);
	for (int RelZ = 0; RelZ < cChunkDef::Width; RelZ++)
	{
		for (int RelX = 0; RelX < cChunkDef::Width; RelX++)
		{
			const int Index = 4 * (RelX + RelZ * cChunkDef::Width);
			const int Height = GetBEInt(HeightData + Index);

			if (Height > std::numeric_limits<HEIGHTTYPE>::max())
			{
				// Invalid data:
				return false;
			}

			cChunkDef::SetHeight(a_HeightMap, RelX, RelZ, static_cast<HEIGHTTYPE>(Height));
		}
	}

	return true;
}





bool cWSSAnvilHandler_1_8::LoadBiomeMap(cChunkDef::BiomeMap & a_BiomeMap, const cParsedNBT & a_NBT, const int a_TagIdx) const
{
	if (
		(a_TagIdx < 0) ||
		(a_NBT.GetType(a_TagIdx) != TAG_ByteArray) ||
		(a_NBT.GetDataLength(a_TagIdx) != std::size(a_BiomeMap))
	)
	{
		return false;
	}

	const auto * const BiomeData = a_NBT.GetData(a_TagIdx);
	for (size_t i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		if (BiomeData[i] > std::byte(EMCSBiome::biMaxVariantBiome))
		{
			// Unassigned biomes:
			return false;
		}

		a_BiomeMap[i] = static_cast<EMCSBiome>(BiomeData[i]);
	}

	return true;
}
