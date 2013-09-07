
#pragma once

#include <vector>
#include "Mobs/Monster.h" // this is a side effect of declaring cMonster::eType inside cMonster MG TODO : make a namespace

class cFastRandom;

// this aggregate static functionnalities about mob types (some could call it helper)
// functionnalities are (in the first version) :
// - create a mob from its type (as enum) (in that way it is a compiler-proxy for mobs)
// - can transform MobTypes from enums to string and reciprocal
class cMobTypesManager
{
public:
	static const std::string& fromMobTypeToString(cMonster::eType a_MobType);
	static cMonster::eType fromStringToMobType(const std::string&);

public:
	class NotAMonsterException : public std::exception {}; //MG TODO : check if this is this project way to do it

protected : 
	typedef const std::map<cMonster::eType,std::string> tMobTypes2Names;
	static tMobTypes2Names& m_MobsTypes2Names();
	static tMobTypes2Names MobTypes2NamesInitializerBeforeCx11();

	static cFastRandom& m_Random();
	
public :
	/** create a new object of the specified mob.
		Warning, new without delete here;
	a_MobType is the type of the mob to be created
	a_Size is the size (for mobs with size)
	if a_Size is let to -1 for entities that need size, size will be random
	assert or return null if mob type is not specified
	assert if size < 1 or > 3 for entities that need size
	*/
	static cMonster* NewMonsterFromType(cMonster::eType a_MobType, int a_Size=-1);

};  // tolua_export

