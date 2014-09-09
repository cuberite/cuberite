
#include "Globals.h"
#include "Mobs/Components/BurningComponent.h"

class cMockEntity {} Entity;
class cMockChunk {};
int main(int argc, char** argv)
{

	cBurningComponent<cMockEntity, cMockChunk> component(Entity);
	return 0;
}
