//
// Created by marc_ on 10.04.2023.
//

#include "WSSAnvilHandler_1_9_0.h"

void cWSSAnvilHandler_1_9_0::LoadBoat(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	auto Boat = std::make_unique<cBoat>(Vector3d(), cBoat::bmOak);
	if (!LoadEntityBase(*Boat.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "Type");
	if (TypeIdx > 0)
	{
		Boat->SetMaterial(cBoat::StringToMaterial(a_NBT.GetString(TypeIdx)));
	}
	a_Entities.emplace_back(std::move(Boat));
}





void cWSSAnvilHandler_1_9_0::LoadEndCrystal(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx) const
{
	bool DisplayBeam = false;
	bool ShowBottom = false;
	Vector3i BeamTarget;
	int CurrentLine = a_NBT.FindChildByName(a_TagIdx, "BeamTarget");
	if (CurrentLine > 0)
	{
		DisplayBeam = true;
		if (a_NBT.GetType(CurrentLine) == TAG_Compound)
		{
			int CoordinateLine = a_NBT.FindChildByName(CurrentLine, "X");
			if (CoordinateLine > 0)
			{
				BeamTarget.x = a_NBT.GetInt(CoordinateLine);
			}
			CoordinateLine = a_NBT.FindChildByName(CurrentLine, "Y");
			if (CoordinateLine > 0)
			{
				BeamTarget.y = a_NBT.GetInt(CoordinateLine);
			}
			CoordinateLine = a_NBT.FindChildByName(CurrentLine, "Z");
			if (CoordinateLine > 0)
			{
				BeamTarget.z = a_NBT.GetInt(CoordinateLine);
			}
		}
	}
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "ShowBottom");
	if (CurrentLine > 0)
	{
		ShowBottom = a_NBT.GetByte(CurrentLine) == 1;
	}

	auto EnderCrystal = std::make_unique<cEnderCrystal>(Vector3d(), BeamTarget, DisplayBeam, ShowBottom);
	if (!LoadEntityBase(*EnderCrystal.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(EnderCrystal));
}
