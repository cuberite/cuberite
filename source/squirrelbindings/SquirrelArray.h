#pragma once
#include <string>

template <typename T>
class SquirrelArray
{
public:
	SquirrelArray()
	{
	}

	unsigned int Size()
	{
		return m_Values.size();
	}

	T Get(unsigned int a_Index)
	{
		if(m_Values.size() < a_Index)
		{
			return T();
		}
		return m_Values.at(a_Index);
	}

	void Add(T a_Value)
	{
		m_Values.push_back(a_Value);
	}

protected:
	std::vector<T> m_Values;

};

class SquirrelStringArray : public SquirrelArray<std::string> { };