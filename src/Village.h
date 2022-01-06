
#pragma once

#include "UUID.h"



class cWorld;
class cVillage;





class cVillageHouse
{
public:

	static const int HOUSE_VALID_TIME = 20 * 60;

	cVillageHouse(cVillage * a_Village, const Vector3i & a_HousePosition) :
		m_Village(a_Village), m_InsideDirectionX(0), m_InsideDirectionZ(0),
		m_LastVillagerTime(0), m_Position(a_HousePosition) {}


	cVillage * GetVillage() { return m_Village; }

	int GetInsideDirectionX() const { return m_InsideDirectionX; }
	int GetInsideDirectionZ() const { return m_InsideDirectionZ; }
	int GetLastVillagerTime() const { return m_LastVillagerTime; }

	const Vector3i & GetPosition() const { return m_Position; }

	bool IsValidHouse();  // Updates the inside of the house and returns true if the house is valid.
	bool IsDeserted();  // Returns true if the house has been left for too long.

	void UpdateHouse();  // A villager was nearby recently so we update the timer.

private:

	cVillage * m_Village;  // The associated village

	int m_InsideDirectionX;  // Specifies the inside direction along x.
	int m_InsideDirectionZ;  // Specifies the inside direction along z.
	int m_LastVillagerTime;  // Last time a villager was nearby.

	Vector3i m_Position;  // Coordinates of the door. It can be the upper or the bottom part of the door.
};




class cPlayerVillageReputation
{
public:

	cPlayerVillageReputation(const cUUID & a_PlayerUUID) : m_PlayerUUID(a_PlayerUUID), m_SocialRank(0) {}

	const cUUID & GetPlayerUUID() const { return m_PlayerUUID; }
	int GetPlayerSocialRank() const { return m_SocialRank; }

	void AddPlayerSocialRank(int a_SocialRankBonus) { m_SocialRank += a_SocialRankBonus; }

private:

	cUUID m_PlayerUUID;  // The UUID of the player.
	int m_SocialRank;  // The social rank of the player. Can be negative. Goes up with trading and down with harming villagers.
};





class cVillage
{
public:

	static const int VILLAGE_MAXIMUM_RADIUS = 32;

	cVillage(cWorld * a_World) : m_World(a_World),
		m_GolemCount(0), m_LastVillagerDeath(0), m_VillagerCount(0),
		m_Radius(0), m_LastHouseAdd(0), m_Tick(0) {}

	void Tick(std::chrono::milliseconds a_Dt);

	// Getters
	cWorld * GetWorld() { return m_World; }

	const Vector3i & GetAggregateCenter() const { return m_AggregateCenter; }
	const Vector3i & GetCenter() const { return m_Center; }

	int GetGolemCount() const { return m_GolemCount; }
	int GetLastVillagerDeathTime() const { return m_LastVillagerDeath; }
	int GetVillagerCount() const { return m_VillagerCount; }
	int GetRadius() const { return m_Radius; }
	int GetLastHouseAddTime() const { return m_LastHouseAdd; }
	int GetHouseCount() const { return m_Houses.size(); }
	int GetVillageTime() const { return m_Tick; }

	const std::vector<cVillageHouse> & GetHouses() const { return m_Houses; }
	const std::vector<cPlayerVillageReputation> & GetPlayersReputation() const { return m_PlayersReputation; }

	/** Returns false if the village should be destroyed. */
	bool IsValid() const { return GetHouseCount() > 0; }

	/** Adds a house into the village.
	Returns true if the door was a valid house. */
	bool AddHouse(const Vector3i & a_DoorPosition);

private:

	cWorld * m_World;

	// From https://minecraft.fandom.com/wiki/Villages.dat_format

	Vector3i m_AggregateCenter;  // Aggregate of the coordinates of all houses.
	Vector3i m_Center;  // Coordinate of the village center.

	int m_GolemCount;  // The number of Iron Golems.
	int m_LastVillagerDeath;  // Last time a villager was killed by a mob, or by a damage source that's not related to an entity while a player was nearby.
	int m_VillagerCount;  // The number of Villagers.
	int m_Radius;  // Radius of the village.
	int m_LastHouseAdd;  // Last time a house was added to the village.
	int m_Tick;  // Village time.

	std::vector<cVillageHouse> m_Houses;  // The doors in the village.

	std::vector<cPlayerVillageReputation> m_PlayersReputation;  // List of players who visited the village.

	/** Updates the village center and radius. */
	void UpdateBoundaries();

	/** Removes a house from the village. If there are no more houses, the village is destroyed. */
	void RemoveHouse(std::size_t a_HouseIndex);
};
