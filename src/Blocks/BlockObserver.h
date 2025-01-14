
#pragma once

#include "BlockHandler.h"
#include "Mixins/Mixins.h"


class cBlockObserverHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	inline static Vector3i GetObservingFaceOffset(BlockState a_Block)
	{
		return -GetSignalOutputOffset(a_Block);
	}

	inline static Vector3i GetSignalOutputOffset(BlockState a_Block)
	{
		using namespace Block;
		switch (Observer::Facing(a_Block))
		{
			case BLOCK_FACE_XM:   return { -1, 0, 0 };
			case BLOCK_FACE_XP:   return { 1, 0, 0 };
			case BLOCK_FACE_YM:   return { 0, -1, 0 };
			case BLOCK_FACE_YP:   return { 0, 1, 0 };
			case BLOCK_FACE_ZM:   return { 0, 0, -1 };
			case BLOCK_FACE_ZP:   return { 0, 0, 1 };
			default: return Vector3i();
		}
	}

	inline static void Toggle(cChunkInterface & a_ChunkInterface, Vector3i a_Position)
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_Position);
		a_ChunkInterface.FastSetBlock(a_Position, Observer::Observer(Observer::Facing(Self), Observer::Powered(Self)));
	}
};
