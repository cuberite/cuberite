
#pragma once

#include <set>

class cChunk;

class cBlockDataReader
{
public : 
	virtual void read(const cChunk&, int a_BlockIdx) = 0;
};

template<typename T>
class cBlockValueReader :  public cBlockDataReader
{
protected : 
	T m_Value;
	bool m_ValueFounded;
	void setValue(T newValue);

public :
	cBlockValueReader();
	T getValue();
};

class cBlockTypeReader : public cBlockValueReader<BLOCKTYPE>
{
public  : 
	virtual void read(const cChunk&, int a_BlockIdx) override;
};

class cBlockMetaReader : public cBlockValueReader<NIBBLETYPE>
{
public : 
	virtual void read(const cChunk&, int a_BlockIdx) override;
};

class cBlockLightReader : public cBlockValueReader<NIBBLETYPE>
{
public : 
	virtual void read(const cChunk&, int a_BlockIdx) override;
};

class cBlockSkyLightReader : public cBlockValueReader<NIBBLETYPE>
{
public : 
	virtual void read(const cChunk&, int a_BlockIdx) override;
};

class cBlockMultipleReader : public cBlockDataReader
{
public : 
	cBlockMultipleReader(cBlockDataReader&, cBlockDataReader&);
	cBlockMultipleReader(cBlockDataReader&, cBlockDataReader&, cBlockDataReader&);
	cBlockMultipleReader(cBlockDataReader&, cBlockDataReader&, cBlockDataReader&, cBlockDataReader&);
	virtual void read(const cChunk&, int a_BlockIdx) override;
protected :
	void addReader(cBlockDataReader& toAdd);
	std::set<cBlockDataReader*> m_Readers;
};



template<typename T>
cBlockValueReader<T>::cBlockValueReader()
{
	m_ValueFounded = false;
}
template<typename T>
T cBlockValueReader<T>::getValue()
{
	if (m_ValueFounded)
	{
		return m_Value;
	}
	else
	{
		return -1; // MG TODO
	}
}
template<typename T>
void cBlockValueReader<T>::setValue(T newValue)
{
	m_ValueFounded = true;
	m_Value = newValue;
}
