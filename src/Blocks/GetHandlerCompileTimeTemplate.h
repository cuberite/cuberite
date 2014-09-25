
#pragma once

class cBlockTorchHandler;
class cBlockLeverHandler;
class cBlockButtonHandler;
class cBlockTripwireHookHandler;
class cBlockDoorHandler;
class cBlockPistonHandler;






template<BLOCKTYPE T>
class GetHandlerCompileTime;







template<>
class GetHandlerCompileTime<E_BLOCK_TORCH>
{
public:
	typedef cBlockTorchHandler type;
};






template<>
class GetHandlerCompileTime<E_BLOCK_LEVER>
{
public:
	typedef cBlockLeverHandler type;
};






template<>
class GetHandlerCompileTime<E_BLOCK_STONE_BUTTON>
{
public:
	typedef cBlockButtonHandler type;
};






template<>
class GetHandlerCompileTime<E_BLOCK_TRIPWIRE_HOOK>
{
public:
	typedef cBlockTripwireHookHandler type;
};






template<>
class GetHandlerCompileTime<E_BLOCK_WOODEN_DOOR>
{
public:
	typedef cBlockDoorHandler type;
};






template<>
class GetHandlerCompileTime<E_BLOCK_PISTON>
{
public:
	typedef cBlockPistonHandler type;
};

