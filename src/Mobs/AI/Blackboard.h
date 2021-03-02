
#pragma once

#include "json/json.h"
class cMonster;

using cBlackboardKey = AString;

class cBlackboard
{
public:
	cBlackboard(cMonster & a_Self):
		m_Self(&a_Self)
	{
	}

	Json::Value & operator [] (const cBlackboardKey & a_Key)
	{
		return m_Json[a_Key];
	}

	cMonster & Self()
	{
		return *m_Self;
	}

	const Json::Value & All() const
	{
		return m_Json;
	}

private:

	Json::Value m_Json;
	cMonster * m_Self;
};
