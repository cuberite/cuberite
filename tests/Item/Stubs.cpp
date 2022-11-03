
#include "Globals.h"

#include "Bindings/PluginManager.h"
#include "BlockEntities/BannerEntity.h"
#include "BlockEntities/BedEntity.h"
#include "BlockEntities/MobHeadEntity.h"
#include "Blocks/BlockHandler.h"
#include "Blocks/ChunkInterface.h"
#include "ClientHandle.h"
#include "Entities/ArrowEntity.h"
#include "Entities/Floater.h"
#include "Entities/ItemFrame.h"
#include "Entities/Painting.h"
#include "Entities/Player.h"
#include "Inventory.h"
#include "LineBlockTracer.h"
#include "MapManager.h"
#include "Noise/Noise.h"
#include "Root.h"
#include "Simulator/FluidSimulator.h"


const cBlockHandler & cBlockHandler::For(unsigned char)
{
	// Dummy handler.
	static cBlockHandler Handler(E_BLOCK_AIR);
	return Handler;
}

AString ItemTypeToString(short) { return ""; }
decltype(cRoot::s_Root)      cRoot::s_Root;


void cArrowEntity::CollectedBy(cPlayer&)
{
}





bool cArrowEntity::DoesPreventBlockPlacement() const
{
	return false;
}





void cArrowEntity::OnHitEntity(cEntity&, Vector3<double>)
{
}





void cArrowEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
}





void cArrowEntity::Tick(std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000l> >, cChunk&)
{
}





cArrowEntity::cArrowEntity(cPlayer&, double) : cProjectileEntity(cProjectileEntity::pkArrow, nullptr, Vector3d(), 0.0, 0.0)
{
}





void cBannerEntity::SetBaseColor(unsigned char)
{
}





void cBedEntity::SetColor(short)
{
}





bool cBlockHandler::CanBeAt(cChunk const&, Vector3<int>, unsigned char) const
{
	return false;
}





cItems cBlockHandler::ConvertToPickups(unsigned char, cItem const*) const
{
	return cItems();
}





bool cBlockHandler::DoesIgnoreBuildCollision(cWorld const&, cItem const&, Vector3<int>, unsigned char, eBlockFace, bool) const
{
	return false;
}





ColourID cBlockHandler::GetMapBaseColourID(unsigned char) const
{
	return 0;
}





cBoundingBox cBlockHandler::GetPlacementCollisionBox(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char) const
{
	return cBoundingBox(0, 0, 0, 0, 0, 0);
}





bool cBlockHandler::IsInsideBlock(Vector3<double>, unsigned char) const
{
	return false;
}





bool cBlockHandler::IsUseable() const
{
	return false;
}





void cBlockHandler::OnNeighborChanged(cChunkInterface&, Vector3<int>, eBlockFace) const
{
}





void cBlockHandler::OnUpdate(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cChunk & a_Chunk, const Vector3i a_RelPos) const
{
}





cBoat::eMaterial cBoat::ItemToMaterial(cItem const&)
{
	return cBoat::bmAcacia;
}





cBoundingBox::cBoundingBox(Vector3<double>, Vector3<double>)
{
}





cBoundingBox::cBoundingBox(double, double, double, double, double, double)
{
}





bool cChunkInterface::ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback)
{
}





BLOCKTYPE cChunkInterface::GetBlock(Vector3<int>)
{
	return 0;
}





NIBBLETYPE cChunkInterface::GetBlockMeta(Vector3<int>)
{
	return 0;
}





bool cChunkInterface::GetBlockTypeMeta(Vector3<int>, unsigned char&, unsigned char&)
{
	return false;
}





void cChunkInterface::SetBlock(Vector3<int>, unsigned char, unsigned char)
{
}





bool cChunkInterface::WriteBlockArea(cBlockArea&, int, int, int, int)
{
	return false;
}





void cChunkMap::FastSetBlock(Vector3<int>, unsigned char, unsigned char)
{
}





BLOCKTYPE cChunkMap::GetBlock(Vector3<int>) const
{
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockMeta(Vector3<int>) const
{
	return 0;
}





void cClientHandle::SendEditSign(Vector3<int>)
{
}





void cEntity::AddSpeed(double, double, double)
{
}





void cEntity::ApplyArmorDamage(int)
{
}





bool cEntity::ArmorCoversAgainst(eDamageType)
{
	return false;
}





void cEntity::BroadcastMovementUpdate(cClientHandle const*)
{
}





void cEntity::Destroy()
{
}





void cEntity::DetectCacti()
{
}





void cEntity::DetectMagma()
{
}





bool cEntity::DetectPortal()
{
	return false;
}





bool cEntity::DoTakeDamage(TakeDamageInfo&)
{
	return false;
}





float cEntity::GetArmorCoverAgainst(cEntity const*, eDamageType, int)
{
	return 0.0f;
}





const char * cEntity::GetClass() const
{
	return "";
}





float cEntity::GetEnchantmentBlastKnockbackReduction()
{
	return 0.0f;
}





float cEntity::GetEnchantmentCoverAgainst(cEntity const*, eDamageType, int)
{
	return 0.0f;
}





double cEntity::GetKnockbackAmountAgainst(cEntity const&)
{
	return 0.0;
}





Vector3d cEntity::GetLookVector() const
{
	return Vector3d();
}





const char * cEntity::GetParentClass() const
{
	return "";
}





int cEntity::GetRawDamageAgainst(cEntity const&)
{
	return 0;
}





void cEntity::HandleAir()
{
}





void cEntity::HandlePhysics(std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000l> >, cChunk&)
{
}





void cEntity::HandleSpeedFromAttachee(float, float)
{
}





void cEntity::Heal(int)
{
}





bool cEntity::Initialize(std::unique_ptr<cEntity>, cWorld&)
{
	return false;
}





bool cEntity::IsA(char const*) const
{
	return false;
}





void cEntity::KilledBy(TakeDamageInfo&)
{
}





void cEntity::OnAddToWorld(cWorld&)
{
}





void cEntity::OnDetach()
{
}





void cEntity::OnFinishedBurning()
{
}





void cEntity::OnRemoveFromWorld(cWorld&)
{
}





void cEntity::OnStartedBurning()
{
}





void cEntity::ResetPosition(Vector3<double>)
{
}





void cEntity::SetSwimState(cChunk&)
{
}





void cEntity::StartBurning(int)
{
}





void cEntity::TeleportToCoords(double, double, double)
{
}





void cEntity::TeleportToEntity(cEntity&)
{
}





void cEntity::Tick(std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000l> >, cChunk&)
{
}





void cEntity::TickBurning(cChunk&)
{
}





void cEntity::TickInVoid(cChunk&)
{
}





cEntity::cEntity(cEntity::eEntityType, Vector3<double>, float, float)
{
}





int cEntityEffect::GetPotionEffectDuration(short)
{
	return 0;
}





short int cEntityEffect::GetPotionEffectIntensity(short)
{
	return 0;
}





bool cEntityEffect::IsPotionDrinkable(short)
{
	return false;
}





cEntityEffect::eType cEntityEffect::GetPotionEffectType(short)
{
	return cEntityEffect::eType::effAbsorption;
}





void cFireworkItem::ColoursFromString(AString const &, cFireworkItem &)
{
}





AString cFireworkItem::ColoursToString(cFireworkItem const&)
{
	return "";
}





void cFireworkItem::FadeColoursFromString(AString const&, cFireworkItem&)
{
}





AString cFireworkItem::FadeColoursToString(cFireworkItem const&)
{
	return "";
}





void cFloater::SpawnOn(cClientHandle & a_Client)
{
}





void cFloater::Tick(std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000l> >, cChunk&)
{
}





cFloater::cFloater(Vector3<double>, Vector3<double>, unsigned int, int): cEntity(etFloater, Vector3d(), 0.0, 0.0)
{
}





bool cFluidSimulator::CanWashAway(unsigned char)
{
	return false;
}





bool cHangingEntity::IsValidSupportBlock(unsigned char)
{
	return false;
}





void cHangingEntity::KilledBy(TakeDamageInfo&)
{
}





void cHangingEntity::SpawnOn(cClientHandle&)
{
}





void cHangingEntity::Tick(std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000l> >, cChunk&)
{
}





cHangingEntity::cHangingEntity(cEntity::eEntityType, eBlockFace, Vector3<double>): cEntity(eEntityType::etItemFrame, Vector3d(), 0.0, 0.0)
{
}





const cItem & cInventory::GetArmorSlot(int) const
{
	return cItem();
}





const cItem & cInventory::GetEquippedItem() const
{
	return cItem();
}





bool cInventory::HasItems(cItem const&)
{
	return false;
}





int cInventory::RemoveItem(cItem const&)
{
	return 0;
}





bool cInventory::RemoveOneEquippedItem()
{
	return false;
}





void cInventory::SendEquippedSlot()
{
}





void cInventory::SetArmorSlot(int, cItem const&)
{
}





bool cItemFrame::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	return false;
}





void cItemFrame::GetDrops(cItems&, cEntity*)
{
}





void cItemFrame::OnRightClicked(cPlayer&)
{
}





void cItemFrame::SpawnOn(cClientHandle&)
{
}





cItemFrame::cItemFrame(eBlockFace a_Face, Vector3<double> a_Pos) : cHangingEntity(etItemFrame, a_Face, a_Pos)
{

}





const cItem & cItemGrid::GetSlot(int) const
{
	return cItem();
}





bool cLineBlockTracer::Trace(Vector3<double>, Vector3<double>)
{
	return false;
}





bool cLineBlockTracer::Trace(cWorld&, cBlockTracer::cCallbacks&, Vector3<double>, Vector3<double>)
{
	return false;
}





cLineBlockTracer::cLineBlockTracer(cWorld& a_World, cBlockTracer::cCallbacks& a_Callbacks) : cBlockTracer(a_World, a_Callbacks)
{
}





void cMap::UpdateClient(cPlayer*)
{
}





void cMap::UpdateRadius(cPlayer&, unsigned int)
{
}





cMap * cMapManager::CreateMap(int, int, unsigned int)
{
	return nullptr;
}





cMap * cMapManager::GetMapData(unsigned int)
{
	return nullptr;
}





void cMobHeadEntity::SetRotation(eMobHeadRotation)
{
}





void cMobHeadEntity::SetType(eMobHeadType const&)
{
}





cNoise::cNoise(int)
{
}





void cPainting::GetDrops(cItems&, cEntity*)
{
}





void cPainting::KilledBy(TakeDamageInfo&)
{
}





void cPainting::SpawnOn(cClientHandle & a_Client)
{
}





cPainting::cPainting(AString const&, eBlockFace, Vector3<double>): cHangingEntity(etPainting, eBlockFace::BLOCK_FACE_BOTTOM, Vector3d())
{
}





void cPawn::AddEntityEffect(cEntityEffect::eType, int, short, double)
{
}





void cPawn::ClearEntityEffects()
{
}





bool cPawn::FindTeleportDestination(cWorld&, int, unsigned int, Vector3<double>&, Vector3<int>, int)
{
	return false;
}





void cPlayer::AwardAchievement(CustomStatistic)
{
}





bool cPlayer::Feed(int, double)
{
	return false;
}





int cPlayer::FinishChargingBow()
{
	return 0;
}





Vector3d cPlayer::GetEyePosition() const
{
	return Vector3d();
}





Vector3d cPlayer::GetThrowStartPos() const
{
	return Vector3d();
}





bool cPlayer::IsGameModeAdventure() const
{
	return false;
}





bool cPlayer::IsGameModeCreative() const
{
	return false;
}





bool cPlayer::IsGameModeSurvival() const
{
	return false;
}





bool cPlayer::PlaceBlock(Vector3<int>, unsigned char, unsigned char)
{
	return false;
}





bool cPlayer::PlaceBlocks(std::initializer_list<sSetBlock>)
{
	return false;
}





void cPlayer::ReplaceOneEquippedItemTossRest(cItem const&)
{
}





void cPlayer::SendBlocksAround(Vector3<int>, int)
{
}





void cPlayer::StartChargingBow()
{
}





void cPlayer::UseEquippedItem(cItemHandler::eDurabilityLostAction)
{
}





void cPlayer::UseEquippedItem(short)
{
}





bool cPluginManager::CallHookPlayerFished(cPlayer&, cItems const&, int)
{
	return false;
}





bool cPluginManager::CallHookPlayerFishing(cPlayer&, cItems&, int&)
{
	return false;
}





void cProjectileEntity::CollectedBy(cPlayer&)
{
}





void cProjectileEntity::HandlePhysics(std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000l> >, cChunk&)
{
}





void cProjectileEntity::OnHitEntity(cEntity&, Vector3<double>)
{
}





void cProjectileEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
}





void cProjectileEntity::SpawnOn(cClientHandle&)
{
}





void cProjectileEntity::Tick(std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000l> >, cChunk&)
{
}





cProjectileEntity::cProjectileEntity(cProjectileEntity::eKind, cEntity*, Vector3<double>, float, float): cEntity(etProjectile, Vector3d(), 0.0, 0.0), m_CreatorData(0, "", cEnchantments())
{
}





void cWorld::BroadcastSoundEffect(AString const&, Vector3<double>, float, float, cClientHandle const*)
{
}





void cWorld::BroadcastSoundParticleEffect(EffectID, Vector3<int>, int, cClientHandle const*)
{
}





UInt32 cWorld::CreateProjectile(Vector3<double>, cProjectileEntity::eKind, cEntity*, cItem const*, Vector3<double> const*)
{
	return 0;
}





UInt32 cWorld::CreateProjectile(double, double, double, cProjectileEntity::eKind, cEntity*, cItem const*, Vector3<double> const*)
{
	return 0;
}





bool cWorld::DigBlock(Vector3<int>, cEntity const*)
{
	return false;
}





bool cWorld::DoWithBlockEntityAt(Vector3<int>, cFunctionRef<bool (cBlockEntity&)>)
{
	return false;
}





bool cWorld::DoWithEntityByID(unsigned int, cFunctionRef<bool (cEntity&)>)
{
	return false;
}





bool cWorld::DropBlockAsPickups(Vector3<int>, cEntity const*, cItem const*)
{
	return false;
}





bool cWorld::ForEachEntityInBox(cBoundingBox const&, cFunctionRef<bool (cEntity&)>)
{
	return false;
}





bool cWorld::ForEachPlayer(cFunctionRef<bool (cPlayer&)>)
{
	return false;
}





EMCSBiome cWorld::GetBiomeAt(int, int)
{
	return biBeach;
}





bool cWorld::GetBlockTypeMeta(Vector3<int>, unsigned char&, unsigned char&) const
{
	return false;
}





int cWorld::GetTickRandomNumber(int)
{
	return 0;
}





int cWorld::GrowPlantAt(Vector3<int>, int)
{
	return 0;
}





void cWorld::PlaceBlock(Vector3<int>, unsigned char, unsigned char)
{
}





void cWorld::SetBlock(Vector3<int>, unsigned char, unsigned char)
{
}





UInt32 cWorld::SpawnBoat(Vector3<double>, cBoat::eMaterial)
{
	return 0;
}





UInt32 cWorld::SpawnEnderCrystal(Vector3<double>, bool)
{
	return 0;
}





UInt32 cWorld::SpawnExperienceOrb(Vector3<double>, int)
{
	return 0;
}





void cWorld::SpawnItemPickups(cItems const&, Vector3<double>, Vector3<double>, bool)
{
}





void cWorld::SpawnItemPickups(cItems const&, Vector3<int>, double, bool)
{
}





UInt32 cWorld::SpawnMinecart(Vector3<double>, int, cItem const&, int)
{
	return 0;
}





UInt32 cWorld::SpawnMob(double, double, double, eMonsterType, bool)
{
	return 0;
}





UInt32 cWorld::SpawnPrimedTNT(Vector3<double>, int, double, bool)
{
	return 0;
}




