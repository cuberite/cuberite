
#pragma once





class cEntity;
class cWorld;




namespace Explodinator
{
	/** Creates an explosion of Power, centred at Position, with ability to set fires as provided.
	For maximum efficiency, Position should be in the centre of the entity or block that exploded.
	The entity pointer is used to trigger OnBreak for the destroyed blocks.
	Kaboom indeed, you drunken wretch. */
	void Kaboom(cWorld & World, Vector3f Position, unsigned Power, bool Fiery, const cEntity * a_ExplodingEntity);
}
