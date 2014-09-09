#include "Globals.h"
#include "EnvironmentComponent.h"
#include "../Monster.h"
#include "../../World.h"
#include "../../Chunk.h"

cEnvironmentComponent::cEnvironmentComponent(cMonster * a_Entity, int a_SightDistance) : m_Self(a_Entity), m_SightDistance(a_SightDistance){}


