#pragma once
#include "Protocol_1_19.h"





class cProtocol_1_20
	: public cProtocol_1_19_4
{
	using Super = cProtocol_1_19_4;
public:

	using Super::Super;

protected:
	virtual void    SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void    SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_20_2
	: public cProtocol_1_20
{
	using Super = cProtocol_1_20;
public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_20_3
	: public cProtocol_1_20_2
{
	using Super = cProtocol_1_20_2;
public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_20_5
	: public cProtocol_1_20_3
{
	using Super = cProtocol_1_20_3;
public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};
