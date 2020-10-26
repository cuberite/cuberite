
#include "Globals.h"
#include "FireworksSerializer.h"
#include "../WorldStorage/FastNBT.h"
#include "../BlockType.h"





void cFireworkItem::WriteToNBTCompound(const cFireworkItem & a_FireworkItem, cFastNBTWriter & a_Writer, const ENUM_ITEM_TYPE a_Type)
{
	switch (a_Type)
	{
		case E_ITEM_FIREWORK_ROCKET:
		{
			a_Writer.BeginCompound("Fireworks");
			a_Writer.AddByte("Flight", static_cast<Byte>(a_FireworkItem.m_FlightTimeInTicks / 20));
			a_Writer.BeginList("Explosions", TAG_Compound);
			a_Writer.BeginCompound("");
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





void cFireworkItem::ParseFromNBT(cFireworkItem & a_FireworkItem, const cParsedNBT & a_NBT, int a_TagIdx, const ENUM_ITEM_TYPE a_Type)
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
				if (TagType == TAG_Byte)  // Custon name tag
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
						// Divide by four as data length returned in bytes
						size_t DataLength = a_NBT.GetDataLength(explosiontag);
						// round to the next highest multiple of four
						DataLength -= DataLength % 4;
						if (DataLength == 0)
						{
							continue;
						}

						const char * ColourData = (a_NBT.GetData(explosiontag));
						for (size_t i = 0; i < DataLength; i += 4)
						{
							a_FireworkItem.m_Colours.push_back(GetBEInt(ColourData + i));
						}
					}
					else if (ExplosionName == "FadeColors")
					{
						size_t DataLength = a_NBT.GetDataLength(explosiontag) / 4;
						// round to the next highest multiple of four
						DataLength -= DataLength % 4;
						if (DataLength == 0)
						{
							continue;
						}

						const char * FadeColourData = (a_NBT.GetData(explosiontag));
						for (size_t i = 0; i < DataLength; i += 4)
						{
							a_FireworkItem.m_FadeColours.push_back(GetBEInt(FadeColourData + i));
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
				if (TagType == TAG_Byte)  // Custon name tag
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





int cFireworkItem::GetVanillaColourCodeFromDye(NIBBLETYPE a_DyeMeta)
{
	/*
	Colours are supposed to be calculated via: R << 16 + G << 8 + B
	However, the RGB values fireworks use aren't the same as the ones for dyes (the ones listed in the MC Wiki)
	Therefore, here is a list of numbers gotten via the Protocol Proxy
	*/

	switch (a_DyeMeta)
	{
		case E_META_DYE_BLACK: return 0x1E1B1B;
		case E_META_DYE_RED: return 0xB3312C;
		case E_META_DYE_GREEN: return 0x3B511A;
		case E_META_DYE_BROWN:  return 0x51301A;
		case E_META_DYE_BLUE: return 0x253192;
		case E_META_DYE_PURPLE: return 0x7B2FBE;
		case E_META_DYE_CYAN: return 0x287697;
		case E_META_DYE_LIGHTGRAY: return 0xABABAB;
		case E_META_DYE_GRAY: return 0x434343;
		case E_META_DYE_PINK: return 0xD88198;
		case E_META_DYE_LIGHTGREEN: return 0x41CD34;
		case E_META_DYE_YELLOW: return 0xDECF2A;
		case E_META_DYE_LIGHTBLUE: return 0x6689D3;
		case E_META_DYE_MAGENTA: return 0xC354CD;
		case E_META_DYE_ORANGE: return 0xEB8844;
		case E_META_DYE_WHITE: return 0xF0F0F0;
		default: ASSERT(!"Unhandled dye meta whilst trying to get colour code for fireworks!"); return 0;
	}
}
