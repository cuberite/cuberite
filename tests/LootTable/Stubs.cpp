
#include "Globals.h"

#include "Items/ItemHandler.h"
#include "WorldStorage/FastNBT.h"
#include "Root.h"
#include "FurnaceRecipe.h"
#include "Entities/EntityEffect.h"
#include "Mobs/Monster.h"
#include "ChunkMap.h"
#include "World.h"
#include "Entities/Player.h"

class cItem;

const cFurnaceRecipe::cRecipe * cFurnaceRecipe::GetRecipeFrom(const cItem &) const { return nullptr; }





int cPlayer::GetXpLevel() const { return 0; }





const cItem Item;





const cItem & cInventory::GetEquippedItem() const { return Item; }





EMCSBiome cWorld::GetBiomeAt(int a_BlockX, int a_BlockZ) { return biInvalidBiome; }





unsigned char cWorld::GetBlockBlockLight(Vector3i a_BlockPos) { return 0; }





bool cWorld::DoWithEntityByID(UInt32 a_UniqueID, cFunctionRef<bool (cEntity &)> a_Callback) { return true; }





cItemHandler * cItemHandler::GetItemHandler(int a_ItemType) { return nullptr; };





bool StringToItem(const AString & a_String, cItem & a_Item) { return true; }





AString ItemToString(const cItem & a_Item) { return ""; }





void cFastNBTWriter::AddByte(const AString & a_Name, unsigned char a_Value) {}





void cFastNBTWriter::AddIntArray (const AString & a_Name, const Int32 * a_Value, size_t a_NumElements) {}





void cFastNBTWriter::BeginList(const AString & a_Name, eTagType a_ChildrenType) {}





void cFastNBTWriter::EndList(void) {}





void cFastNBTWriter::BeginCompound(const AString & a_Name) {}





void cFastNBTWriter::EndCompound(void) {}





decltype(cRoot::s_Root)      cRoot::s_Root;





class cPawn * cMonster::GetTarget(void) { return nullptr; }





unsigned char cChunkMap::GetBlock(class Vector3<int>) { return 0; }





cEntity * cEntity::GetAttached() { return nullptr; }





std::map<cEntityEffect::eType, cEntityEffect *> cPawn::GetEntityEffects() const { return std::map<cEntityEffect::eType, cEntityEffect *>(); }





cEntityEffect::eType cEntityEffect::GetTypeFromString(const AString & m_Type)
{
	if (NoCaseCompare(m_Type, "NoEffect") == 0)
	{
		return effNoEffect;
	}
	else if (NoCaseCompare(m_Type, "Speed") == 0)
	{
		return effSpeed;
	}
	else if (NoCaseCompare(m_Type, "Slowness") == 0)
	{
		return effSlowness;
	}
	else if (NoCaseCompare(m_Type, "Haste") == 0)
	{
		return effHaste;
	}
	else if (NoCaseCompare(m_Type, "MiningFatigue") == 0)
	{
		return effMiningFatigue;
	}
	else if (NoCaseCompare(m_Type, "Strength") == 0)
	{
		return effStrength;
	}
	else if (NoCaseCompare(m_Type, "InstantHealth") == 0)
	{
		return effInstantHealth;
	}
	else if (NoCaseCompare(m_Type, "InstantDamage") == 0)
	{
		return effInstantDamage;
	}
	else if (NoCaseCompare(m_Type, "JumpBoost") == 0)
	{
		return effJumpBoost;
	}
	else if (NoCaseCompare(m_Type, "Nausea") == 0)
	{
		return effNausea;
	}
	else if (NoCaseCompare(m_Type, "Regeneration") == 0)
	{
		return effRegeneration;
	}
	else if (NoCaseCompare(m_Type, "Resistance") == 0)
	{
		return effResistance;
	}
	else if (NoCaseCompare(m_Type, "FireResistance") == 0)
	{
		return effFireResistance;
	}
	else if (NoCaseCompare(m_Type, "WaterBreathing") == 0)
	{
		return effWaterBreathing;
	}
	else if (NoCaseCompare(m_Type, "Invisibility") == 0)
	{
		return effInvisibility;
	}
	else if (NoCaseCompare(m_Type, "Hunger") == 0)
	{
		return effHunger;
	}
	else if (NoCaseCompare(m_Type, "Weakness") == 0)
	{
		return effWeakness;
	}
	else if (NoCaseCompare(m_Type, "Poison") == 0)
	{
		return effPoison;
	}
	else if (NoCaseCompare(m_Type, "Wither") == 0)
	{
		return effWither;
	}
	else if (NoCaseCompare(m_Type, "HealthBoost") == 0)
	{
		return effHealthBoost;
	}
	else if (NoCaseCompare(m_Type, "Absorption") == 0)
	{
		return effAbsorption;
	}
	else if (NoCaseCompare(m_Type, "Saturation") == 0)
	{
		return effSaturation;
	}
	else
	{
		return effNoEffect;
	}
}





int cChunkGeneratorThread::GetSeed(void) const { return 0; }





cLootTable const * cLootTableProvider::GetLootTable(const AString & a_String) { return nullptr; }
