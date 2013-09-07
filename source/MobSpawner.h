
#pragma once

#include <set>
#include "BlockID.h"
#include "ChunkDef.h"
#include "FastRandom.h"
#include "Mobs/Monster.h" //this is a side-effect of keeping Mobfamily inside Monster class. I'd prefer to keep both (Mobfamily and Monster) inside a "Monster" namespace MG TODO : do it

class cChunk;
class cEntity;


// This class is used to determine wich monster can be spawned on wich place
// it is essentially static (f.i. Squids spawn in water, Zombie spawn in dark places)
// but it also has dynamic part depending on the world.ini
class cMobSpawner
{
public :
	// constructor
	// a_MobFamily is the mega type of mobs that this spawner will spawn
	// a_AllowedTypes is the set of types allowed for mobs it will spawn. Empty set
	// would result in no spawn at all
	// Allowed mobs thah are not of the right Megatype will not be include (no warning)
	cMobSpawner(cMonster::eFamily MobFamily, const std::set<cMonster::eType>& a_AllowedTypes);


};
