
#pragma once

#include "PassiveAggressiveMonster.h"




/** Used by enderman and chorus fruit.
Checks for valid destinations in a cube of length 2 * a_HalfCubeWidth centred at a_Centre.
Returns true and places destination in a_Destination if successful.
Returns false if destination could be found after a_NumTries attempts.
Details at: https://minecraft.fandom.com/wiki/Enderman#Teleportation. */
bool FindTeleportDestination(cWorld *a_World, Vector3i a_Centre, const int a_HalfCubeWidth, const int a_HeightRequired, const unsigned int a_NumTries, Vector3i &a_Destination);





class cEnderman:
	public cPassiveAggressiveMonster
{
	using Super = cPassiveAggressiveMonster;

public:

	cEnderman();

	CLASS_PROTODEF(cEnderman)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void CheckEventSeePlayer(cChunk & a_Chunk) override;
	virtual void EventLosePlayer(void) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	bool IsScreaming(void) const {return m_bIsScreaming; }
	BLOCKTYPE GetCarriedBlock(void) const {return m_CarriedBlock; }
	NIBBLETYPE GetCarriedMeta(void) const {return m_CarriedMeta; }

private:

	bool m_bIsScreaming;
	BLOCKTYPE m_CarriedBlock;
	NIBBLETYPE m_CarriedMeta;

} ;
