
#pragma once

/**
* Any information about changes is taken from: https://pokechu22.github.io/BurgerWebViewer
*/

#include "ChunkDef.h"
#include "StringCompression.h"

#include "BlockEntities/BlockEntity.h"

#include "Registries/DataVersion.h"

#include "WorldStorage/FastNBT.h"
#include "WorldStorage/NamespaceSerializer.h"
#include "WorldStorage/WorldStorage.h"

// fwd:
class cItem;
class cItemGrid;
class cMonster;
class cProjectileEntity;
class cHangingEntity;
class cUUID;
class ChunkBlockData;


class cWSSAnvilHandler
{
public:
	cWSSAnvilHandler() = default;
	virtual ~cWSSAnvilHandler() = default;

	using HandlerMap = std::map<eDataVersion, std::unique_ptr<cWSSAnvilHandler>>;

	static const eDataVersion m_DataVersion = eDataVersion::vUnknown;

	// Basic data types:
	/** Loads an array of doubles of the specified length from the specified NBT list tag a_TagIdx; returns true if successful */
	template <size_t NumDoubles> static inline bool LoadDoublesList(std::array<double, NumDoubles> & a_Doubles, const cParsedNBT & a_NBT, int a_TagIdx)
	{
		if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_List) || (a_NBT.GetChildrenType(a_TagIdx) != TAG_Double))
		{
			return false;
		}
		size_t idx = 0;
		for (int Tag = a_NBT.GetFirstChild(a_TagIdx); (Tag > 0) && (idx < NumDoubles); Tag = a_NBT.GetNextSibling(Tag), ++idx)
		{
			a_Doubles[idx] = a_NBT.GetDouble(Tag);
		}  // for Tag - PosTag[]
		return (idx == NumDoubles);  // Did we read enough doubles?
	}

	/** Loads an array of floats of the specified length from the specified NBT list tag a_TagIdx; returns true if successful */
	template <size_t NumFloats>  static inline bool LoadFloatsList(std::array<float, NumFloats> & a_Floats, const cParsedNBT & a_NBT, int a_TagIdx)
	{
		if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_List) || (a_NBT.GetChildrenType(a_TagIdx) != TAG_Double))
		{
			return false;
		}
		size_t idx = 0;
		for (int Tag = a_NBT.GetFirstChild(a_TagIdx); (Tag > 0) && (idx < NumFloats); Tag = a_NBT.GetNextSibling(Tag), ++idx)
		{
			a_Floats[idx] = a_NBT.GetFloat(Tag);
		}  // for Tag - PosTag[]
		return (idx == NumFloats);  // Did we read enough floats?
	}

	/** Loads a cItem contents from the specified NBT tag; returns true if successful. Doesn't load the Slot tag */
	virtual bool LoadItem(cItem & a_Item, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;

	/** Loads contents of an Items[] list tag into a cItemGrid
	ItemGrid begins at the specified slot offset
	Slots outside the ItemGrid range are ignored */
	virtual void LoadItemGrid(cItemGrid & a_ItemGrid, const cParsedNBT & a_NBT, int a_ItemsTagIdx, int s_SlotOffset = 0) const = 0;

	virtual void LoadEntities(cEntityList & a_Entities, const cParsedNBT & a_NBT, const int a_TagIdx) const = 0;
	virtual void LoadEntity(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_EntityTagIdx, const std::string_view a_EntityName) const = 0;
	/** Loads entity common data from the NBT compound; returns true if successful */
	virtual bool LoadEntityBase(cEntity & a_Entity, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;

	// Miscellaneous:
	virtual void LoadAreaEffectCloud (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadArmorStand      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadBoat            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadEndCrystal      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadFallingBlock    (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadExpOrb          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadFloater         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadHanging         (cHangingEntity & a_Hanging, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadItemFrame       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadLeashKnot       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPainting        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPickup          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadTNT             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;

	// Minecart:
	/** This is a pre 1.5 function. Back then the minecarts shared the same ID */
	virtual void LoadOldMinecart          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadRideableMinecart     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadChestMinecart        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadCommandBlockMinecart (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadFurnaceMinecart      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadTNTMinecart          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadHopperMinecart       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;

	// Projectile:
	/** Loads projectile common data from the NBT compound; returns true if successful */
	virtual bool LoadProjectileBase  (cProjectileEntity & a_Entity, const cParsedNBT & a_NBT, int a_TagIx) const = 0;
	virtual void LoadArrow           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadDragonFireball  (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadEgg             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadEvocationFangs  (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadEyeOfEnder      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadFireball        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadFireCharge      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadFireworkRocket  (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadLlamaSpit       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadShulkerBullet   (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSnowball        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSpectralArrow   (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSplashPotion    (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadThrownEnderPearl(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadThrownExpBottle (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadWitherSkull     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;

	/** Loads monster common data from the NBT compound; returns true if successful */
	virtual bool LoadMonsterBase     (cMonster & a_Monster, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;

	/** Loads the position to where is leashed the monster */
	virtual void LoadLeashToPosition (cMonster & a_Monster, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;

	/** Loads the owner name and UUID from the entity at the specified NBT tag.
	Returns a pair of {name, uuid}. If the entity is not owned, name is an empty string and uuid is nil. */
	virtual std::pair<AString, cUUID> LoadEntityOwner(const cParsedNBT & a_NBT, int a_TagIdx) const = 0;

	// Mobs
	virtual void LoadBat              (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadBlaze            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadCat              (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadCaveSpider       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadChicken          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadCod              (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadCow              (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadCreeper          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadDolphin          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadDonkey           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadDrowned          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadElderGuardian    (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadEnderDragon      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadEnderman         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadEndermite        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadEvocationIllager (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadEvoker           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadFox              (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadGhast            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadGiant            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadGuardian         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadHorse            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadHoglin           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadHusk             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadIllusioner       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadIronGolem        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadLlama            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadMagmaCube        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadMooshroom        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadMule             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadOcelot           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPanda            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadParrot           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPhantom          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPig              (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPiglin           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPiglinBrute      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPillager         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPolarBear        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadPufferfish       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadRabbit           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadRavager          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSalmon           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSheep            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadShulker          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSilverfish       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSkeleton         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSkeletonHorse    (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSlime            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSnowGolem        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSpider           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadSquid            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadStray            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadStrider          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadTraderLlama      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadTropicalFish     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadTurtle           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadVex              (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadVillager         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadVindicator       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadWanderingTrader  (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadWitch            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadWither           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadWitherSkeleton   (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadWolf             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadZoglin           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadZombie           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadZombieHorse      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadZombifiedPiglin  (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;
	virtual void LoadZombieVillager   (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const = 0;

	/** Helper function for extracting the X, Y, and Z int subtags of a NBT compound; returns true if successful */
	virtual bool GetBlockEntityNBTPos(const cParsedNBT & a_NBT, int a_TagIdx, Vector3i & a_AbsPos) const = 0;

	/** Returns true iff the "id" child tag inside the specified tag equals (case-sensitive) any of the specified expected types.
	Logs a warning to the console on mismatch.
	The coordinates are used only for the log message. */
	virtual bool CheckBlockEntityType(const cParsedNBT & a_NBT, int a_TagIdx, const AStringVector & a_ExpectedTypes, Vector3i a_Pos) const = 0;

	/** Loads the chunk's BlockEntities from NBT data (a_Tag is the Level\\TileEntities list tag; may be -1) */
	virtual void LoadBlockEntities(cBlockEntities & a_BlockEntities, const cParsedNBT & a_NBT, int a_Tag, const ChunkBlockData & a_BlockData, cWorld * a_World) const = 0;

	/** Loads the data for a block entity from the specified NBT tag.
	Returns the loaded block entity, or nullptr upon failure. */
	virtual OwnedBlockEntity LoadBlockEntity(const cParsedNBT & a_NBT, int a_Tag, Vector3i a_Pos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cWorld * a_World) const = 0;

	/** Decodes the text contained within a sign.
	Older versions used direct string representation, newer versions use JSON-formatted string.
	This function extracts the text from either version. */
	virtual AString DecodeSignLine(const AString & a_Line) const = 0;

	virtual OwnedBlockEntity LoadBanner                (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadBeacon                (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadBed                   (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadBrewingStand          (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadChest                 (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;

	virtual OwnedBlockEntity LoadCommandBlock          (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadRepeatingCommandBlock (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadChainCommandBlock     (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;

	virtual OwnedBlockEntity LoadComparator            (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadDispenser             (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadDropper               (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadEnchantingTable       (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadEnderChest            (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadEndGateway            (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadEndPortal             (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadFlowerPot             (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadFurnace               (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadHopper                (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadJukebox               (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadMobHead               (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadMobSpawner            (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadNoteBlock             (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadSign                  (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;
	virtual OwnedBlockEntity LoadStructureBlock        (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const = 0;

	virtual bool LoadHeightMap(cChunkDef::HeightMap & a_HeightMap, const cParsedNBT & a_NBT, const int a_TagIdx) const = 0;

	virtual bool LoadBiomeMap(cChunkDef::BiomeMap & a_BiomeMap, const cParsedNBT & a_NBT, const int a_TagIdx) const = 0;

	static const cWSSAnvilHandler & From(eDataVersion a_DataVersion);

protected:

	inline void Unimplemented(std::string_view a_FnName) const
	{
		LOGWARNING("cWSSAnvilHandler::%s is not jet implemented for data version %d", a_FnName, NamespaceSerializer::From(m_DataVersion));
	}

	inline void Unsupported(std::string_view a_FnName) const
	{
		LOGWARNING("cWSSAnvilHandler::%s is not supported by minecraft for data version", a_FnName, NamespaceSerializer::From(m_DataVersion));
	}
};

