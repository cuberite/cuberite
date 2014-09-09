
#include "Globals.h"
#include "Mobs/Components/BurningComponent.h"

int main(int argc, char** argv)
{
	class cMockEntity {} Entity;
	class cMockChunk {};
	cBurningComponent<cMockEntity, cMockChunk> component(Entity);
	return 0;
}
