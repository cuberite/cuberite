
#pragma once

#include "ChunkStay.h"


class cEntity;
class cWorld;





// This is the chunk stay which finds nearby nether portals
class cNetherPortalScanner : public cChunkStay
{
public:
	cNetherPortalScanner(cEntity & a_MovingEntity, cWorld & a_DestinationWorld, Vector3d a_DestPosition, int a_MaxY);
	virtual void OnChunkAvailable(int a_ChunkX, int a_ChunkY) override;
	virtual bool OnAllChunksAvailable(void) override;
	virtual void OnDisabled(void) override;

	enum class Direction
	{
		X,
		Y
	};

private:

	/** Length and height, including the obsidian. */
	static const int PortalLength = 4;
	static const int PortalHeight = 5;

	static const int SearchRadius = 128;
	static const int BuildSearchRadius = 16;

	/** The width of a solid base to search for when building. */
	static const int SearchSolidBaseWidth = 3;

	/** Where to place the player out from the face and across the face */
	static const double OutOffset;
	static const double AcrossOffset;

	/** Builds a portal. */
	void BuildNetherPortal(Vector3i a_Location, Direction a_Direction, bool a_IncludePlatform);

	/** Whether the given location is a valid location to build a portal. */
	bool IsValidBuildLocation(Vector3i a_BlockPosition);

	/** The ID of the entity that's being moved. */
	UInt32 m_EntityID;

	/** The world we're moving the entity from, used to query the entity ID. */
	cWorld & m_SourceWorld;

	/** The world we're moving the entity to. */
	cWorld & m_World;

	/** Whether we found a portal during the loading of the chunks. */
	bool m_FoundPortal;

	/** Whether to build a platform. True if we couldn't build the portal on solid ground */
	bool m_BuildPlatform;

	/** The direction of the portal. */
	Direction m_Dir;

	/** The position of the pre-existing portal. */
	Vector3i m_PortalLoc;

	/** The center of the search area */
	Vector3d m_Position;

	/** The maximum Y to scan to */
	int m_MaxY;
};
