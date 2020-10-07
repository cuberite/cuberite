
#pragma once





class cWorld;
class cEntity;




namespace Explodinator
{
	/** Creates an explosion of Power, centred at Position, with ability to set fires as provided.
	For maximum efficiency, Position should be in the centre of the entity or block that exploded.
	Kaboom indeed, you drunken wretch.
	The entity pointer is used while trigger OnBreak in the destroyed blocks. */
	void Kaboom(cWorld & World, Vector3f Position, unsigned Power, bool Fiery, const cEntity * a_ExplodingEntity);
}
