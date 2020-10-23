
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
#include "BlockEntities/BlockEntity.h"

class cItem;

const cFurnaceRecipe::cRecipe * cFurnaceRecipe::GetRecipeFrom(const cItem &) const { return nullptr; }





int cPlayer::GetXpLevel() const { return 0; }





const cItem Item;





const cItem & cInventory::GetEquippedItem() const { return Item; }





EMCSBiome cWorld::GetBiomeAt(int a_BlockX, int a_BlockZ) { return biInvalidBiome; }





bool cWorld::DoWithBlockEntityAt(int a_X, int a_Y, int a_Z, cFunctionRef<bool (cBlockEntity&)> a_Function)
{
	return true;
}





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





int cChunkGeneratorThread::GetSeed(void) const { return 0; }





cLootTable const * cLootTableProvider::GetLootTable(const AString & a_String) { return nullptr; }





bool cBlockEntity::IsBlockEntityBlockType(BLOCKTYPE a_Block) { return true; }
