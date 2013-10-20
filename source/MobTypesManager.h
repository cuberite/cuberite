
#pragma once

#include "Mobs/Monster.h" // this is a side effect of declaring cMonster::eType inside cMonster MG TODO : make a namespace




// fwd:
class cFastRandom;





/**
This class aggregates static functions about mob types:
 - create a mob from its type (as enum) (in that way it is a compiler-proxy for mobs)
 - transform MobTypes from enums to string and vice versa
 - return mob family from given type
*/
class cMobTypesManager
{
public:
	static AString MobTypeToString(cMonster::eType a_MobType);
	static cMonster::eType StringToMobType(const AString& a_MobTypeName);
	static cMonster::eFamily FamilyFromType(cMonster::eType a_MobType);

	/** create a new object of the specified mob.
	a_MobType is the type of the mob to be created
	a_Size is the size (for mobs with size)
	if a_Size is let to -1 for entities that need size, size will be random
	asserts and returns null if mob type is not specified
	asserts if invalid size for mobs that need size
	*/
	static cMonster * NewMonsterFromType(cMonster::eType a_MobType, int a_Size = -1);

protected : 
	typedef const std::map<cMonster::eType,std::string> tMobTypes2Names;
	static tMobTypes2Names& m_MobsTypes2Names(void);
	static tMobTypes2Names MobTypes2NamesInitializerBeforeCx11(void);

	typedef const std::map<cMonster::eType,cMonster::eFamily> tMobType2Family;
	static tMobType2Family& m_MobsType2Family(void);
	static tMobType2Family MobType2FamilyInitializerBeforeCx11(void);

	static cFastRandom & m_Random(void);
	
public :
} ;




