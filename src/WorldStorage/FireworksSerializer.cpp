
#include "Globals.h"
#include "FireworksSerializer.h"
#include "WorldStorage/FastNBT.h"





void cFireworkItem::WriteToNBTCompound(const cFireworkItem & a_FireworkItem, cFastNBTWriter & a_Writer, const ENUM_ITEM_ID a_Type)
{
	switch (a_Type)
	{
		case E_ITEM_FIREWORK_ROCKET:
		{
			a_Writer.BeginCompound("Fireworks");
			a_Writer.AddByte("Flight", a_FireworkItem.m_FlightTimeInTicks / 20);
			a_Writer.BeginList("Explosions", TAG_Compound);
			a_Writer.BeginCompound("");
			a_Writer.AddByte("Flicker", a_FireworkItem.m_HasFlicker);
			a_Writer.AddByte("Trail", a_FireworkItem.m_HasTrail);
			a_Writer.AddByte("Type", a_FireworkItem.m_Type);
			a_Writer.AddIntArray("Colors", a_FireworkItem.m_Colours.data(), a_FireworkItem.m_Colours.size());
			a_Writer.AddIntArray("FadeColors", a_FireworkItem.m_FadeColours.data(), a_FireworkItem.m_FadeColours.size());
			a_Writer.EndCompound();
			a_Writer.EndList();
			a_Writer.EndCompound();
			break;
		}
		case E_ITEM_FIREWORK_STAR:
		{
			a_Writer.BeginCompound("Explosion");
			a_Writer.AddByte("Flicker", a_FireworkItem.m_HasFlicker);
			a_Writer.AddByte("Trail", a_FireworkItem.m_HasTrail);
			a_Writer.AddByte("Type", a_FireworkItem.m_Type);
			if (!a_FireworkItem.m_Colours.empty())
			{
				a_Writer.AddIntArray("Colors", a_FireworkItem.m_Colours.data(), a_FireworkItem.m_Colours.size());
			}
			if (!a_FireworkItem.m_FadeColours.empty())
			{
				a_Writer.AddIntArray("FadeColors", a_FireworkItem.m_FadeColours.data(), a_FireworkItem.m_FadeColours.size());
			}
			a_Writer.EndCompound();
			break;
		}
		default: ASSERT(!"Unhandled firework item!"); break;
	}
}





void cFireworkItem::ParseFromNBT(cFireworkItem & a_FireworkItem, const cParsedNBT & a_NBT, int a_TagIdx, const ENUM_ITEM_ID a_Type)
{
	if (a_TagIdx < 0)
	{
		return;
	}

	switch (a_Type)
	{
		case E_ITEM_FIREWORK_STAR:
		{
			for (int explosiontag = a_NBT.GetFirstChild(a_TagIdx); explosiontag >= 0; explosiontag = a_NBT.GetNextSibling(explosiontag))
			{
				eTagType TagType = a_NBT.GetType(explosiontag);
				if (TagType == TAG_Byte) // Custon name tag
				{
					AString ExplosionName = a_NBT.GetName(explosiontag);

					if (ExplosionName == "Flicker")
					{
						a_FireworkItem.m_HasFlicker = (a_NBT.GetByte(explosiontag) == 1);
					}
					else if (ExplosionName == "Trail")
					{
						a_FireworkItem.m_HasTrail = (a_NBT.GetByte(explosiontag) == 1);
					}
					else if (ExplosionName == "Type")
					{
						a_FireworkItem.m_Type = a_NBT.GetByte(explosiontag);
					}
				}
				else if (TagType == TAG_IntArray)
				{
					AString ExplosionName = a_NBT.GetName(explosiontag);

					if (ExplosionName == "Colors")
					{
						if (a_NBT.GetDataLength(explosiontag) == 0)
						{
							continue;
						}

						const int * ColourData = (const int *)(a_NBT.GetData(explosiontag));
						for (int i = 0; i < ARRAYCOUNT(ColourData); i++)
						{
							a_FireworkItem.m_Colours.push_back(ntohl(ColourData[i]));
						}
					}
					else if (ExplosionName == "FadeColors")
					{
						if (a_NBT.GetDataLength(explosiontag) == 0)
						{
							continue;
						}

						const int * FadeColourData = (const int *)(a_NBT.GetData(explosiontag));
						for (int i = 0; i < ARRAYCOUNT(FadeColourData); i++)
						{
							a_FireworkItem.m_FadeColours.push_back(ntohl(FadeColourData[i]));
						}
					}
				}
			}
			break;
		}
		case E_ITEM_FIREWORK_ROCKET:
		{
			for (int fireworkstag = a_NBT.GetFirstChild(a_TagIdx); fireworkstag >= 0; fireworkstag = a_NBT.GetNextSibling(fireworkstag))
			{
				eTagType TagType = a_NBT.GetType(fireworkstag);
				if (TagType == TAG_Byte) // Custon name tag
				{
					if (a_NBT.GetName(fireworkstag) == "Flight")
					{
						a_FireworkItem.m_FlightTimeInTicks = a_NBT.GetByte(fireworkstag) * 20;
					}
				}
				else if ((TagType == TAG_List) && (a_NBT.GetName(fireworkstag) == "Explosions"))
				{
					int ExplosionsChild = a_NBT.GetFirstChild(fireworkstag);
					if ((a_NBT.GetType(ExplosionsChild) == TAG_Compound) && (a_NBT.GetName(ExplosionsChild).empty()))
					{
						ParseFromNBT(a_FireworkItem, a_NBT, ExplosionsChild, E_ITEM_FIREWORK_STAR);
					}
				}
			}
			break;
		}
		default: ASSERT(!"Unhandled firework item!"); break;
	}
}





AString cFireworkItem::ColoursToString(const cFireworkItem & a_FireworkItem)
{
	AString Result;

	for (std::vector<int>::const_iterator itr = a_FireworkItem.m_Colours.begin(); itr != a_FireworkItem.m_Colours.end(); ++itr)
	{
		AppendPrintf(Result, "%i;", *itr);
	}

	return Result;
}





void cFireworkItem::ColoursFromString(const AString & a_String, cFireworkItem & a_FireworkItem)
{
	AStringVector Split = StringSplit(a_String, ";");

	for (size_t itr = 0; itr < Split.size(); ++itr)
	{
		if (Split[itr].empty())
		{
			continue;
		}

		a_FireworkItem.m_Colours.push_back(atoi(Split[itr].c_str()));
	}
}





AString cFireworkItem::FadeColoursToString(const cFireworkItem & a_FireworkItem)
{
	AString Result;

	for (std::vector<int>::const_iterator itr = a_FireworkItem.m_FadeColours.begin(); itr != a_FireworkItem.m_FadeColours.end(); ++itr)
	{
		AppendPrintf(Result, "%i;", *itr);
	}

	return Result;
}





void cFireworkItem::FadeColoursFromString(const AString & a_String, cFireworkItem & a_FireworkItem)
{
	AStringVector Split = StringSplit(a_String, ";");

	for (size_t itr = 0; itr < Split.size(); ++itr)
	{
		if (Split[itr].empty())
		{
			continue;
		}

		a_FireworkItem.m_FadeColours.push_back(atoi(Split[itr].c_str()));
	}
}





int cFireworkItem::GetVanillaColourCodeFromDye(short a_DyeMeta)
{
	/*
	Colours are supposed to be calculated via: R << 16 + G << 8 + B
	However, the RGB values fireworks use aren't the same as the ones for dyes (the ones listed in the MC Wiki)
	Therefore, here is a list of numbers gotten via the Protocol Proxy
	*/

	switch (a_DyeMeta)
	{
		case E_META_DYE_BLACK: return 1973019;
		case E_META_DYE_RED: return 11743532;
		case E_META_DYE_GREEN: return 3887386;
		case E_META_DYE_BROWN:  return 5320730;
		case E_META_DYE_BLUE: return 2437522;
		case E_META_DYE_PURPLE: return 8073150;
		case E_META_DYE_CYAN: return 2651799;
		case E_META_DYE_LIGHTGRAY: return 11250603;
		case E_META_DYE_GRAY: return 4408131;
		case E_META_DYE_PINK: return 14188952;
		case E_META_DYE_LIGHTGREEN: return 4312372;
		case E_META_DYE_YELLOW: return 14602026;
		case E_META_DYE_LIGHTBLUE: return 6719955;
		case E_META_DYE_MAGENTA: return 12801229;
		case E_META_DYE_ORANGE: return 15435844;
		case E_META_DYE_WHITE: return 15790320;
		default: ASSERT(!"Unhandled dye meta whilst trying to get colour code for fireworks!"); break;
	}
}
