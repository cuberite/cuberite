
#pragma once

// fwd:
class cBroadcastInterface;
class cChunkInterface;
class cForEachChunkProvider;
class cWorld;
class cWorldInterface;

/** Utilities to allow casting a cWorld to one of its interfaces without including World.h. */
namespace World
{
	// Defined in World.cpp
	cBroadcastInterface *   GetBroadcastInterface(cWorld * a_World);
	cForEachChunkProvider * GetFECProvider       (cWorld * a_World);
	cWorldInterface *       GetWorldInterface    (cWorld * a_World);

	inline cBroadcastInterface &   GetBroadcastInterface(cWorld & a_World) { return *GetBroadcastInterface(&a_World); }
	inline cForEachChunkProvider & GetFECProvider       (cWorld & a_World) { return *GetFECProvider(&a_World); }
	inline cWorldInterface &       GetWorldInterface    (cWorld & a_World) { return *GetWorldInterface(&a_World); }

	// cChunkInterface is more like a pimpl for cChunkMap than an interface so it needs to be returned by value
	cChunkInterface GetChunkInterface(cWorld & a_World);
}
