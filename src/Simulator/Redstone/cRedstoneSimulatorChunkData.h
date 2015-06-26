
#pragma once
#include <unordered_map>
#include "Vector3.h"
#include "Component.h"

using namespace Redstone;

class cRedstoneSimulatorChunkData
{

public:
	cRedstoneSimulatorChunkData() {}

	~cRedstoneSimulatorChunkData() {}

	void WakeUp(Vector3i coords);

	ComponentPtr GetComponent(Vector3i coords);
	void SetComponent(ComponentPtr component);
	void RemoveComponent(Vector3i location);
	cVector3iArray & GetActiveBlocks() 
	{ 
		return m_ActiveBlocks; 
	}

private:
	std::unordered_map<Vector3i, ComponentPtr, VectorHasher<int>> m_ChunkData;
	cVector3iArray m_ActiveBlocks;

};