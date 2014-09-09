
#pragma once

class cChunk;

// This class is paramerised to allow mocking
template <class EntityType, class ChunkType>
class cBurningComponent
{
private:
	EntityType & m_Self;
public:

	cBurningComponent(EntityType & a_Self)
	: m_Self(a_Self)
	{}

	void Tick(float a_Dt, ChunkType & a_Chunk);
};

#include "BurningComponent.inc"
