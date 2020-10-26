
// FireworksSerializer.h

// Declares the cFireworkItem class representing a firework or firework star





#pragma once


#include "../BlockType.h"





class cFastNBTWriter;
class cParsedNBT;





class cFireworkItem
{
public:
	cFireworkItem(void) :
		m_HasFlicker(false),
		m_HasTrail(false),
		m_Type(0),
		m_FlightTimeInTicks(0)
	{
	}

	inline void CopyFrom(const cFireworkItem & a_Item)
	{
		m_FlightTimeInTicks = a_Item.m_FlightTimeInTicks;
		m_HasFlicker = a_Item.m_HasFlicker;
		m_HasTrail = a_Item.m_HasTrail;
		m_Type = a_Item.m_Type;
		m_Colours = a_Item.m_Colours;
		m_FadeColours = a_Item.m_FadeColours;
	}

	inline void EmptyData(void)
	{
		m_FlightTimeInTicks = 0;
		m_HasFlicker = false;
		m_Type = 0;
		m_HasTrail = false;
		m_Colours.clear();
		m_FadeColours.clear();
	}

	inline bool IsEqualTo(const cFireworkItem & a_Item) const
	{
		return
			(
			(m_FlightTimeInTicks == a_Item.m_FlightTimeInTicks) &&
			(m_HasFlicker == a_Item.m_HasFlicker) &&
			(m_HasTrail == a_Item.m_HasTrail) &&
			(m_Type == a_Item.m_Type) &&
			(m_Colours == a_Item.m_Colours) &&
			(m_FadeColours == a_Item.m_FadeColours)
			);
	}

	/** Writes firework NBT data to a Writer object */
	static void WriteToNBTCompound(const cFireworkItem & a_FireworkItem, cFastNBTWriter & a_Writer, const ENUM_ITEM_TYPE a_Type);

	/** Reads NBT data from a NBT object and populates a FireworkItem with it */
	static void ParseFromNBT(cFireworkItem & a_FireworkItem, const cParsedNBT & a_NBT, int a_TagIdx, const ENUM_ITEM_TYPE a_Type);

	/** Converts the firework's vector of colours into a string of values separated by a semicolon */
	static AString ColoursToString(const cFireworkItem & a_FireworkItem);

	/** Parses a string containing encoded firework colours and populates a FireworkItem with it */
	static void ColoursFromString(const AString & a_String, cFireworkItem & a_FireworkItem);

	/** Converts the firework's vector of fade colours into a string of values separated by a semicolon */
	static AString FadeColoursToString(const cFireworkItem & a_FireworkItem);

	/** Parses a string containing encoded firework fade colours and populates a FireworkItem with it */
	static void FadeColoursFromString(const AString & a_String, cFireworkItem & a_FireworkItem);

	/** Returns a colour code for fireworks used by the network code */
	static int GetVanillaColourCodeFromDye(NIBBLETYPE a_DyeMeta);

	bool m_HasFlicker;
	bool m_HasTrail;
	NIBBLETYPE m_Type;
	short m_FlightTimeInTicks;
	std::vector<int> m_Colours;
	std::vector<int> m_FadeColours;
};
