
// PointOfInterest.h

// Points Of Interest (POI) are used to store special blocks in memory to interact with entities. See https://minecraft.fandom.com/wiki/Java_Edition_level_format#poi_format





#pragma once

#include "Registries/PoiTypes.h"




class cPointOfInterest
{
public:

	cPointOfInterest(Vector3i a_BlockPosition, ePoiType a_PoiType) :
		m_BlockPosition(a_BlockPosition), m_Type(a_PoiType),
		m_FreeTickets(GetMaxFreeTickets(m_Type)) {}

	cPointOfInterest(Vector3i a_BlockPosition, ePoiType a_PoiType, int a_FreeTickets) :
		m_BlockPosition(a_BlockPosition), m_Type(a_PoiType),
		m_FreeTickets(a_FreeTickets) {}

	static ePoiType GetPointOnInterestType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	static unsigned char GetMaxFreeTickets(ePoiType a_Type);

	// Getters
	const Vector3i & GetBlockPosition() const { return m_BlockPosition; }
	ePoiType GetPoiType() const { return m_Type; }
	unsigned char GetTicketsRemaining() const { return m_FreeTickets; }

	void RemoveTicket() { m_FreeTickets = std::max<unsigned char>(0, m_FreeTickets - 1); }
	void AddTicket() { m_FreeTickets = std::min<unsigned char>(GetMaxFreeTickets(m_Type), m_FreeTickets + 1); }

private:

	Vector3i m_BlockPosition;
	ePoiType m_Type;
	unsigned char m_FreeTickets;

};
