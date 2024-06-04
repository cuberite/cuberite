
#pragma once

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler_1_8.h"

class cWSSAnvilHandler_1_9_0 : public cWSSAnvilHandler_1_8
{
	using Super = cWSSAnvilHandler_1_8;

public:
	using Super::Super;
	~cWSSAnvilHandler_1_9_0() override = default;
	static const eDataVersion m_DataVersion = eDataVersion::v1_9_0;

protected:
	void LoadBoat            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override;

	// Todo: Entities
	void LoadAreaEffectCloud (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadAreaEffectCloud"); }

	// Todo: new metadata was added in 1.9
	void LoadArmorStand      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadArmorStand"); }
	void LoadArrow           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadArrow"); }
	void LoadBat             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadBat"); }
	void LoadBlaze		     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadBlaze"); }
	void LoadCreeper         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadCreeper"); }

	// Todo: new
	void LoadDragonFireball  (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadDragonFireball"); }

	// Todo: new metadata was added in 1.9
	void LoadEndCrystal      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override;
	void LoadEnderDragon	 (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadEnderDragon"); }
	void LoadEnderman        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadEnderman"); }
	void LoadHorse		     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadHorse"); }
	void LoadFallingBlock    (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadFallingBlock"); }
	void LoadFireworkRocket  (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadFireworkRocket"); }
	void LoadGhast           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadGhast"); }
	void LoadGuardian        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadGuardian"); }
	void LoadPickup          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadPickup"); }
	void LoadItemFrame       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadItemFrame"); }
	void LoadCommandBlockMinecart (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadCommandBlockMinecart"); }
	void LoadFurnaceMinecart (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadFurnaceMinecart"); }
	void LoadOcelot          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadOcelot"); }
	void LoadPig             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadPig"); }
	void LoadTNT             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadTnt"); }
	void LoadRabbit  	     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadRabbit"); }
	void LoadSheep           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadSheep"); }

	// Todo: new
	void LoadShulker         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadShulker"); }
	void LoadShulkerBullet   (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadShulkerBullet"); }

	// Todo: new metadata was added in 1.9
	void LoadSkeleton        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadSkeleton"); }
	void LoadSlime           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadSlime"); }
	void LoadSnowball        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadSnowball"); }
	void LoadSpectralArrow   (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadSpectralArrow"); }
	void LoadSpider          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadSpider"); }
	void LoadSplashPotion    (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadThrownPotion"); }
	void LoadVillager        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadVillager"); }
	void LoadWitch           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadWitch"); }
	void LoadWither          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadWither"); }
	void LoadWitherSkull     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadWitherSkull"); }
	void LoadWolf            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadWolf"); }
	void LoadZombie          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const override { Unimplemented("LoadZombie"); }

	// Todo: Block Entity
	OwnedBlockEntity LoadCommandBlock          (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const override { Unimplemented("LoadCommandBlock"); return nullptr; }
	OwnedBlockEntity LoadRepeatingCommandBlock (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const override { Unimplemented("LoadRepeatingCommandBlock"); return nullptr; }
	OwnedBlockEntity LoadChainCommandBlock     (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const override { Unimplemented("LoadChainCommandBlock"); return nullptr; }
	OwnedBlockEntity LoadEndGateway            (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const override { Unimplemented("LoadEndGateway"); return nullptr; }
	OwnedBlockEntity LoadStructureBlock        (const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) const override { Unimplemented("LoadStructureBlock"); return nullptr; }
};




class cWSSAnvilHandler_1_9_1 : public cWSSAnvilHandler_1_9_0
{
	using Super = cWSSAnvilHandler_1_9_0;

public:
	using Super::Super;
	eDataVersion m_DataVersion = eDataVersion::v1_9_1;

	~cWSSAnvilHandler_1_9_1() override = default;
};





class cWSSAnvilHandler_1_9_2 : public cWSSAnvilHandler_1_9_1
{
	using Super = cWSSAnvilHandler_1_9_1;

public:
	using Super::Super;
	static const eDataVersion m_DataVersion = eDataVersion::v1_9_2;

	~cWSSAnvilHandler_1_9_2() override = default;
};





class cWSSAnvilHandler_1_9_3 : public cWSSAnvilHandler_1_9_2
{
	using Super = cWSSAnvilHandler_1_9_2;

public:
	using Super::Super;
	static const eDataVersion m_DataVersion = eDataVersion::v1_9_3;

	~cWSSAnvilHandler_1_9_3() override = default;
};





class cWSSAnvilHandler_1_9_4 : public cWSSAnvilHandler_1_9_3
{
	using Super = cWSSAnvilHandler_1_9_3;

public:
	using Super::Super;
	static const eDataVersion m_DataVersion = eDataVersion::v1_9_4;

	~cWSSAnvilHandler_1_9_4() override = default;
};
